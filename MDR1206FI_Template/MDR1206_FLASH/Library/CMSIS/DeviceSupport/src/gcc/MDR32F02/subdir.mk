################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Library/CMSIS/DeviceSupport/src/gcc/MDR32F02/startup_MDR32F02.S 

OBJS += \
./Library/CMSIS/DeviceSupport/src/gcc/MDR32F02/startup_MDR32F02.o 

S_UPPER_DEPS += \
./Library/CMSIS/DeviceSupport/src/gcc/MDR32F02/startup_MDR32F02.d 


# Each subdirectory must supply rules for building sources it contributes
Library/CMSIS/DeviceSupport/src/gcc/MDR32F02/%.o: ../Library/CMSIS/DeviceSupport/src/gcc/MDR32F02/%.S Library/CMSIS/DeviceSupport/src/gcc/MDR32F02/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross Assembler'
	riscv-none-elf-gcc -march=rv32imc_zicsr_zifencei_zicntr_zba_zbb_zbc_zbs_zbkb_zbkc_zbkx_zknd_zkne_zknh_zksed_zksh -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g3 -x assembler-with-cpp -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\DeviceSupport" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\DeviceSupport\inc" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\DeviceSupport\src" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\CMSIS\CoreSupport\inc" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\SPL\inc" -I"C:\Users\aleks\Desktop\hack_milandr\mdr1206\ExampleProjects\MDR32VF0xI\MDR1206\Eclipse\MDR1206FI_Template\Library\SPL" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


