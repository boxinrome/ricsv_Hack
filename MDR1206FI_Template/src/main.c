/**
 ******************************************************************************
 * @file    main.c
 * @author  Milandr Application Team
 * @version V0.1.0
 * @date    12/02/2024
 * @brief   Main program body.
 ******************************************************************************
 * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
 * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
 */

/* Includes ------------------------------------------------------------------*/
#include "MDR32VF0xI_adc.h"
#include "MDR32VF0xI_port.h"
#include "MDR32VF0xI_rst_clk.h"
#include "MDR32VF0xI_uart.h"
#include "MDR32VF0xI_utils.h"

/*
 * First use POWER_CONTROL_RELAY_IDENTIFICATION. It toggles PD11 once per
 * second, so an oscilloscope can show which level connects the capacitor to
 * the generator (K1 closed) and which level connects the working UART (K2
 * closed).
 *
 * After the relay polarity is known:
 *   1. Set RELAY_CHARGE_ACTIVE_LEVEL to SET or RESET.
 *   2. Change POWER_CONTROL_MODE to POWER_CONTROL_AUTOMATIC.
 */
#define POWER_CONTROL_RELAY_IDENTIFICATION 0U
#define POWER_CONTROL_AUTOMATIC            1U
#define POWER_CONTROL_MODE                 POWER_CONTROL_RELAY_IDENTIFICATION

/* Placeholder until the first oscilloscope test is complete. */
#define RELAY_CHARGE_ACTIVE_LEVEL SET

/* The two voltage dividers are both 220 kOhm / 220 kOhm. */
#define VOLTAGE_DIVIDER_NUMERATOR   2U
#define VOLTAGE_DIVIDER_DENOMINATOR 1U

#define ADC_REFERENCE_MV 3300U
#define ADC_MAX_CODE     1023U

/* AIN1: rectified generator voltage. AIN2: storage-capacitor voltage. */
#define GENERATOR_ADC_CHANNEL ADC_CH_ADC1_SAR_AIN1
#define CAPACITOR_ADC_CHANNEL ADC_CH_ADC2_SAR_AIN2

/* Slow ADC clock and repeated conversions help with the 110 kOhm source. */
#define ADC_DISCARD_SAMPLES 2U
#define ADC_AVERAGE_SAMPLES 8U

/* Main loop timing. */
#define CONTROL_PERIOD_MS              2U
#define UART_REPORT_PERIOD_MS          100U
#define RELAY_IDENTIFY_HALF_PERIOD_MS 1000U

/* Automatic-mode hysteresis, in raw ADC codes. */
#define CHARGE_ON_MARGIN_CODES  12U
#define CHARGE_OFF_MARGIN_CODES 4U
#define MIN_RELAY_HOLD_TIME_MS  6U

/* Relay control: PD11. */
#define RELAY_PORT     MDR_PORTD
#define RELAY_PORT_CLK RST_CLK_PCLK_PORTD
#define RELAY_PIN      PORT_PIN_11

/* PC link: PA10 = UART3_RXD, PA11 = UART3_TXD. */
#define PC_UART       MDR_UART3
#define PC_UART_PCLK  RST_CLK_PCLK_UART3
#define PC_UART_CLOCK RST_CLK_PER1_C2_UART3
#define PC_UART_BAUD  115200U

#define PC_UART_PORT     MDR_PORTA
#define PC_UART_PORT_CLK RST_CLK_PCLK_PORTA
#define PC_UART_RX_PIN   PORT_PIN_10
#define PC_UART_TX_PIN   PORT_PIN_11
#define PC_UART_FUNCTION PORT_FUNCTION_ALTERNATIVE

typedef enum {
    POWER_STATE_WORK = 0,
    POWER_STATE_CHARGE
} PowerState;

static PowerState PowerStateCurrent = POWER_STATE_WORK;
static uint32_t   SamplesSinceRelayChange;

static void ClockConfig(void);
static void RelayConfig(void);
static void UARTConfig(void);
static void ADCConfig(void);

static void RelayWriteLevel(BitStatus Level);
static void RelaySetPowerState(PowerState State);

static uint16_t ADCReadChannel(ADC_ChannelNumber_TypeDef Channel);
static uint32_t ADCCodeToInputMillivolts(uint16_t Code);

static void UARTWriteChar(char Character);
static void UARTWriteString(const char* Text);
static void UARTWriteUnsigned(uint32_t Value);
static void UARTReport(uint16_t GeneratorCode,
                       uint16_t CapacitorCode,
                       BitStatus RelayLevel,
                       PowerState State);

static void AutomaticPowerControl(uint16_t GeneratorCode,
                                  uint16_t CapacitorCode);

int main(void)
{
    uint16_t  generatorCode;
    uint16_t  capacitorCode;
    uint32_t  reportCounter   = 0U;
    uint32_t  identifyCounter = 0U;
    BitStatus relayLevel      = RESET;

    ClockConfig();
    DELAY_Init(DELAY_MODE_CYCLE_COUNTER);

    RelayConfig();
    UARTConfig();
    ADCConfig();

    UART_Cmd(PC_UART, ENABLE);
    ADC_Cmd(ENABLE);

    UARTWriteString("\r\nMDR1206 power test started\r\n");
    UARTWriteString("AIN1=generator, AIN2=capacitor, PD11=relay control\r\n");

#if (POWER_CONTROL_MODE == POWER_CONTROL_AUTOMATIC)
    RelaySetPowerState(POWER_STATE_WORK);
    UARTWriteString("mode=automatic\r\n");
#else
    RelayWriteLevel(relayLevel);
    UARTWriteString("mode=relay identification\r\n");
#endif

    while (1) {
        generatorCode = ADCReadChannel(GENERATOR_ADC_CHANNEL);
        capacitorCode = ADCReadChannel(CAPACITOR_ADC_CHANNEL);

#if (POWER_CONTROL_MODE == POWER_CONTROL_AUTOMATIC)
        AutomaticPowerControl(generatorCode, capacitorCode);
        relayLevel = PORT_ReadInputDataBit(RELAY_PORT, RELAY_PIN);
#else
        identifyCounter++;
        if (identifyCounter >=
            (RELAY_IDENTIFY_HALF_PERIOD_MS / CONTROL_PERIOD_MS)) {
            identifyCounter = 0U;
            relayLevel = (relayLevel == RESET) ? SET : RESET;
            RelayWriteLevel(relayLevel);
        }
#endif

        reportCounter++;
        if (reportCounter >= (UART_REPORT_PERIOD_MS / CONTROL_PERIOD_MS)) {
            reportCounter = 0U;
            UARTReport(generatorCode,
                       capacitorCode,
                       relayLevel,
                       PowerStateCurrent);
        }

        DELAY_WaitMs(CONTROL_PERIOD_MS);
    }
}

static void ClockConfig(void)
{
    /* HSI at 8 MHz is enough for ADC control and 115200-baud UART. */
    RST_CLK_DeInit();
    RST_CLK_HCLK_ClkSelection(RST_CLK_CPU_HCLK_CLK_SRC_HSI);
    SystemCoreClockUpdate();
}

static void RelayConfig(void)
{
    static const PORT_InitTypeDef RelayPin = {
        .PORT_Pin       = RELAY_PIN,
        .PORT_Mode      = PORT_MODE_DIGITAL,
        .PORT_Direction = PORT_DIRECTION_OUTPUT,
        .PORT_Function  = PORT_FUNCTION_PORT,
        .PORT_Power     = PORT_POWER_NOMINAL_UPTO_2mA,
        .PORT_PullUp    = PORT_PULL_UP_OFF,
        .PORT_PullDown  = PORT_PULL_DOWN_OFF,
    };

    RST_CLK_PCLKCmd(RELAY_PORT_CLK, ENABLE);

    /* Define the output value before enabling the pin as an output. */
    PORT_WriteBit(RELAY_PORT, RELAY_PIN, RESET);
    PORT_Init(RELAY_PORT, &RelayPin);
}

static void UARTConfig(void)
{
    static const UART_InitTypeDef UARTInit = {
        .UART_BaudRate            = PC_UART_BAUD,
        .UART_Mode                = UART_MODE_TX_RX,
        .UART_WordLength          = UART_WORD_LENGTH_8BIT,
        .UART_StopBits            = UART_STOP_BITS_1BIT,
        .UART_Parity              = UART_PARITY_NONE,
        .UART_HardwareFlowControl = UART_HARDWARE_FLOW_CONTROL_NONE,
        .UART_LoopbackMode        = UART_LOOPBACK_DISABLE,
        .UART_FIFO_Mode           = UART_FIFO_MODE_DISABLE,
        .UART_FIFO_TxThreshold    = UART_FIFO_TX_THRESHOLD_2WORD,
        .UART_FIFO_RxThreshold    = UART_FIFO_RX_THRESHOLD_2WORD,
        .UART_IRDA_Mode           = UART_IRDA_MODE_DISABLE,
        .UART_IRDA_PowerMode      = UART_IRDA_POWER_NORMAL,
    };

    static const PORT_InitTypeDef RXPin = {
        .PORT_Pin       = PC_UART_RX_PIN,
        .PORT_Mode      = PORT_MODE_DIGITAL,
        .PORT_Direction = PORT_DIRECTION_INPUT,
        .PORT_Function  = PC_UART_FUNCTION,
        .PORT_Power     = PORT_POWER_NOMINAL_UPTO_2mA,
        .PORT_PullUp    = PORT_PULL_UP_OFF,
        .PORT_PullDown  = PORT_PULL_DOWN_OFF,
    };

    static const PORT_InitTypeDef TXPin = {
        .PORT_Pin       = PC_UART_TX_PIN,
        .PORT_Mode      = PORT_MODE_DIGITAL,
        .PORT_Direction = PORT_DIRECTION_INPUT_OUTPUT,
        .PORT_Function  = PC_UART_FUNCTION,
        .PORT_Power     = PORT_POWER_INCREASED_UPTO_4mA,
        .PORT_PullUp    = PORT_PULL_UP_OFF,
        .PORT_PullDown  = PORT_PULL_DOWN_OFF,
    };

    RST_CLK_PER1_C2_ClkSelection(RST_CLK_PER1_C2_CLK_SRC_CPU_C1);
    RST_CLK_PER1_C2_SetPrescaler(PC_UART_CLOCK,
                                 RST_CLK_PER1_PRESCALER_DIV_1);
    RST_CLK_PER1_C2_Cmd(PC_UART_CLOCK, ENABLE);
    RST_CLK_PCLKCmd(PC_UART_PCLK, ENABLE);

    UART_DeInit(PC_UART);
    UART_Init(PC_UART, &UARTInit);

    RST_CLK_PCLKCmd(PC_UART_PORT_CLK, ENABLE);
    PORT_Init(PC_UART_PORT, &RXPin);
    PORT_Init(PC_UART_PORT, &TXPin);
}

static void ADCConfig(void)
{
    static const ADC_InitTypeDef ADCInit = {
        .ADC_ClockSource      = ADC_CLOCK_SOURCE_ADC_CLK,
        .ADC_Prescaler        = ADC_PRESCALER_DIV_1,
        .ADC_VRefSource       = ADC_VREF_SOURCE_INTERNAL,
        .ADC_Scale            = ADC_SCALE_MAX_UCCA,
        .ADC_ChannelSwitching = ADC_CH_SWITCHING_DISABLE,
        .ADC_ChannelNumber    = GENERATOR_ADC_CHANNEL,
        .ADC_Channels         = 0U,
        .ADC_SamplingMode     = ADC_SAMPLING_MODE_SINGLE_CONV,
        .ADC_DelayGo          = 7U,
        .ADC_LevelControl     = ADC_LEVEL_CONTROL_DISABLE,
        .ADC_LowLevel         = 0U,
        .ADC_HighLevel        = ADC_MAX_CODE,
    };

    /* HSI / 8 = 1 MHz ADC clock. */
    RST_CLK_ADC_ClkSelection(RST_CLK_ADC_CLK_SRC_CPU_C1);
    RST_CLK_ADC_SetPrescaler(RST_CLK_ADC_PRESCALER_DIV_8);
    RST_CLK_ADC_ClkCmd(ENABLE);
    RST_CLK_PCLKCmd(RST_CLK_PCLK_ADC, ENABLE);

    ADC_DeInit();
    ADC_Init(&ADCInit);
}

static void RelayWriteLevel(BitStatus Level)
{
    PORT_WriteBit(RELAY_PORT, RELAY_PIN, Level);
}

static void RelaySetPowerState(PowerState State)
{
    BitStatus relayLevel;

    if (State == POWER_STATE_CHARGE) {
        relayLevel = RELAY_CHARGE_ACTIVE_LEVEL;
    } else {
        relayLevel = (RELAY_CHARGE_ACTIVE_LEVEL == SET) ? RESET : SET;
    }

    RelayWriteLevel(relayLevel);
    PowerStateCurrent = State;
    SamplesSinceRelayChange = 0U;
}

static uint16_t ADCReadChannel(ADC_ChannelNumber_TypeDef Channel)
{
    uint32_t sampleIndex;
    uint32_t sampleSum = 0U;
    uint16_t result    = 0U;

    ADC_SetChannel(Channel);

    for (sampleIndex = 0U;
         sampleIndex < (ADC_DISCARD_SAMPLES + ADC_AVERAGE_SAMPLES);
         sampleIndex++) {
        ADC_Start();
        while (ADC_GetFlagStatus(ADC_FLAG_END_OF_CONVERSION) != SET) { }
        result = (uint16_t)(ADC_GetResult() & ADC_MAX_CODE);

        if (sampleIndex >= ADC_DISCARD_SAMPLES) {
            sampleSum += result;
        }
    }

    return (uint16_t)((sampleSum + (ADC_AVERAGE_SAMPLES / 2U)) /
                      ADC_AVERAGE_SAMPLES);
}

static uint32_t ADCCodeToInputMillivolts(uint16_t Code)
{
    uint32_t numerator;

    numerator = (uint32_t)Code * ADC_REFERENCE_MV *
                VOLTAGE_DIVIDER_NUMERATOR;

    return (numerator + ((ADC_MAX_CODE * VOLTAGE_DIVIDER_DENOMINATOR) / 2U)) /
           (ADC_MAX_CODE * VOLTAGE_DIVIDER_DENOMINATOR);
}

static void UARTWriteChar(char Character)
{
    while (UART_GetFlagStatus(PC_UART, UART_FLAG_TXFF) == SET) { }
    UART_SendData(PC_UART, (uint16_t)(uint8_t)Character);
}

static void UARTWriteString(const char* Text)
{
    while (*Text != '\0') {
        UARTWriteChar(*Text);
        Text++;
    }
}

static void UARTWriteUnsigned(uint32_t Value)
{
    char     digits[10];
    uint32_t digitCount = 0U;

    if (Value == 0U) {
        UARTWriteChar('0');
        return;
    }

    while (Value != 0U) {
        digits[digitCount] = (char)('0' + (Value % 10U));
        digitCount++;
        Value /= 10U;
    }

    while (digitCount != 0U) {
        digitCount--;
        UARTWriteChar(digits[digitCount]);
    }
}

static void UARTReport(uint16_t GeneratorCode,
                       uint16_t CapacitorCode,
                       BitStatus RelayLevel,
                       PowerState State)
{
    UARTWriteString("ain1_gen=");
    UARTWriteUnsigned(GeneratorCode);
    UARTWriteString(" (");
    UARTWriteUnsigned(ADCCodeToInputMillivolts(GeneratorCode));
    UARTWriteString("mV), ain2_cap=");
    UARTWriteUnsigned(CapacitorCode);
    UARTWriteString(" (");
    UARTWriteUnsigned(ADCCodeToInputMillivolts(CapacitorCode));
    UARTWriteString("mV), pd11=");
    UARTWriteChar((RelayLevel == SET) ? '1' : '0');

#if (POWER_CONTROL_MODE == POWER_CONTROL_AUTOMATIC)
    UARTWriteString(", state=");
    UARTWriteString((State == POWER_STATE_CHARGE) ? "CHARGE" : "WORK");
#else
    (void)State;
#endif

    UARTWriteString("\r\n");
}

static void AutomaticPowerControl(uint16_t GeneratorCode,
                                  uint16_t CapacitorCode)
{
    SamplesSinceRelayChange++;

    if (SamplesSinceRelayChange <
        (MIN_RELAY_HOLD_TIME_MS / CONTROL_PERIOD_MS)) {
        return;
    }

    if (PowerStateCurrent == POWER_STATE_WORK) {
        if ((uint32_t)GeneratorCode >
            ((uint32_t)CapacitorCode + CHARGE_ON_MARGIN_CODES)) {
            RelaySetPowerState(POWER_STATE_CHARGE);
        }
    } else {
        if (((uint32_t)GeneratorCode + CHARGE_OFF_MARGIN_CODES) <
            (uint32_t)CapacitorCode) {
            RelaySetPowerState(POWER_STATE_WORK);
        }
    }
}

#if (USE_ASSERT_INFO == 1)
void assert_failed(uint8_t* file, uint32_t line)
{
    (void)file;
    (void)line;
    while (1) { }
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr)
{
    (void)file;
    (void)line;
    (void)expr;
    while (1) { }
}
#endif

