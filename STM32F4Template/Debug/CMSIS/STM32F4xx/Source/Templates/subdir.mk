################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/STM32F4xx/Source/Templates/system_stm32f4xx.c 

OBJS += \
./CMSIS/STM32F4xx/Source/Templates/system_stm32f4xx.o 

C_DEPS += \
./CMSIS/STM32F4xx/Source/Templates/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/STM32F4xx/Source/Templates/%.o: ../CMSIS/STM32F4xx/Source/Templates/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DUSE_STDPERIPH_DRIVER -DUSE_STM32_DISCOVERY -DHSE_VALUE=8000000 -I"/home/tmf/ArmToolchain/workspace/STM32F4Template/CMSIS/STM32F4xx/Include" -I"/home/tmf/ArmToolchain/workspace/STM32F4Template/CMSIS/Include" -I"/home/tmf/ArmToolchain/workspace/STM32F4Template/stlib/inc" -I"/home/tmf/ArmToolchain/workspace/STM32F4Template/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


