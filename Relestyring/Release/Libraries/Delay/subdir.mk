################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/Delay/Delay.c 

OBJS += \
./Libraries/Delay/Delay.o 

C_DEPS += \
./Libraries/Delay/Delay.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/Delay/%.o: ../Libraries/Delay/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER -DUSE_STM32_DISCOVERY -DSTM32F40_41xxx -I"/home/arch/Code/workspace/Relestyring/include" -I"/home/arch/Code/workspace/Relestyring/stlib/inc" -I"/home/arch/Code/workspace/Relestyring/CMSIS/Include" -I"/home/arch/Code/workspace/Relestyring/CMSIS/STM32F4xx/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


