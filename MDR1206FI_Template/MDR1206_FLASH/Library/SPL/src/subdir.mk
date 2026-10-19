################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Library/SPL/src/MDR32VF0xI_asm.S 

C_SRCS += \
../Library/SPL/src/MDR32VF0xI_adc.c \
../Library/SPL/src/MDR32VF0xI_adcui.c \
../Library/SPL/src/MDR32VF0xI_bkp.c \
../Library/SPL/src/MDR32VF0xI_clk_measure.c \
../Library/SPL/src/MDR32VF0xI_crc.c \
../Library/SPL/src/MDR32VF0xI_dma.c \
../Library/SPL/src/MDR32VF0xI_flash.c \
../Library/SPL/src/MDR32VF0xI_i2c.c \
../Library/SPL/src/MDR32VF0xI_iwdg.c \
../Library/SPL/src/MDR32VF0xI_l_block.c \
../Library/SPL/src/MDR32VF0xI_otp.c \
../Library/SPL/src/MDR32VF0xI_port.c \
../Library/SPL/src/MDR32VF0xI_power.c \
../Library/SPL/src/MDR32VF0xI_random.c \
../Library/SPL/src/MDR32VF0xI_rst_clk.c \
../Library/SPL/src/MDR32VF0xI_sensors.c \
../Library/SPL/src/MDR32VF0xI_ssp.c \
../Library/SPL/src/MDR32VF0xI_timer.c \
../Library/SPL/src/MDR32VF0xI_uart.c \
../Library/SPL/src/MDR32VF0xI_usart.c \
../Library/SPL/src/MDR32VF0xI_utils.c \
../Library/SPL/src/MDR32VF0xI_wwdg.c 

C_DEPS += \
./Library/SPL/src/MDR32VF0xI_adc.d \
./Library/SPL/src/MDR32VF0xI_adcui.d \
./Library/SPL/src/MDR32VF0xI_bkp.d \
./Library/SPL/src/MDR32VF0xI_clk_measure.d \
./Library/SPL/src/MDR32VF0xI_crc.d \
./Library/SPL/src/MDR32VF0xI_dma.d \
./Library/SPL/src/MDR32VF0xI_flash.d \
./Library/SPL/src/MDR32VF0xI_i2c.d \
./Library/SPL/src/MDR32VF0xI_iwdg.d \
./Library/SPL/src/MDR32VF0xI_l_block.d \
./Library/SPL/src/MDR32VF0xI_otp.d \
./Library/SPL/src/MDR32VF0xI_port.d \
./Library/SPL/src/MDR32VF0xI_power.d \
./Library/SPL/src/MDR32VF0xI_random.d \
./Library/SPL/src/MDR32VF0xI_rst_clk.d \
./Library/SPL/src/MDR32VF0xI_sensors.d \
./Library/SPL/src/MDR32VF0xI_ssp.d \
./Library/SPL/src/MDR32VF0xI_timer.d \
./Library/SPL/src/MDR32VF0xI_uart.d \
./Library/SPL/src/MDR32VF0xI_usart.d \
./Library/SPL/src/MDR32VF0xI_utils.d \
./Library/SPL/src/MDR32VF0xI_wwdg.d 

OBJS += \
./Library/SPL/src/MDR32VF0xI_adc.o \
./Library/SPL/src/MDR32VF0xI_adcui.o \
./Library/SPL/src/MDR32VF0xI_asm.o \
./Library/SPL/src/MDR32VF0xI_bkp.o \
./Library/SPL/src/MDR32VF0xI_clk_measure.o \
./Library/SPL/src/MDR32VF0xI_crc.o \
./Library/SPL/src/MDR32VF0xI_dma.o \
./Library/SPL/src/MDR32VF0xI_flash.o \
./Library/SPL/src/MDR32VF0xI_i2c.o \
./Library/SPL/src/MDR32VF0xI_iwdg.o \
./Library/SPL/src/MDR32VF0xI_l_block.o \
./Library/SPL/src/MDR32VF0xI_otp.o \
./Library/SPL/src/MDR32VF0xI_port.o \
./Library/SPL/src/MDR32VF0xI_power.o \
./Library/SPL/src/MDR32VF0xI_random.o \
./Library/SPL/src/MDR32VF0xI_rst_clk.o \
./Library/SPL/src/MDR32VF0xI_sensors.o \
./Library/SPL/src/MDR32VF0xI_ssp.o \
./Library/SPL/src/MDR32VF0xI_timer.o \
./Library/SPL/src/MDR32VF0xI_uart.o \
./Library/SPL/src/MDR32VF0xI_usart.o \
./Library/SPL/src/MDR32VF0xI_utils.o \
./Library/SPL/src/MDR32VF0xI_wwdg.o 

S_UPPER_DEPS += \
./Library/SPL/src/MDR32VF0xI_asm.d 


# Each subdirectory must supply rules for building sources it contributes
Library/SPL/src/%.o: ../Library/SPL/src/%.c Library/SPL/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv-none-elf-gcc -march=rv32imc_zicsr_zifencei_zicntr_zba_zbb_zbc_zbs_zbkb_zbkc_zbkx_zknd_zkne_zknh_zksed_zksh -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g3 -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\DeviceSupport" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\DeviceSupport\inc" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\DeviceSupport\src" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\CoreSupport\inc" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\SPL\inc" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\SPL" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/SPL/src/%.o: ../Library/SPL/src/%.S Library/SPL/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv-none-elf-gcc -march=rv32imc_zicsr_zifencei_zicntr_zba_zbb_zbc_zbs_zbkb_zbkc_zbkx_zknd_zkne_zknh_zksed_zksh -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g3 -x assembler-with-cpp -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\DeviceSupport" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\DeviceSupport\inc" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\DeviceSupport\src" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\CoreSupport\inc" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\SPL\inc" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\SPL" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


