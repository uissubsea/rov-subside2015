################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../CMSIS/STM32F4xx/Source/Templates/TASKING/cstart_thumb2.asm 

OBJS += \
./CMSIS/STM32F4xx/Source/Templates/TASKING/cstart_thumb2.o 

ASM_DEPS += \
./CMSIS/STM32F4xx/Source/Templates/TASKING/cstart_thumb2.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/STM32F4xx/Source/Templates/TASKING/%.o: ../CMSIS/STM32F4xx/Source/Templates/TASKING/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


