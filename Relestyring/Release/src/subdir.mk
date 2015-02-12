################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc_setup.c \
../src/main.c 

OBJS += \
./src/adc_setup.o \
./src/main.o 

C_DEPS += \
./src/adc_setup.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER -DUSE_STM32_DISCOVERY -DSTM32F40_41xxx -I"/home/arch/Code/workspace/Relestyring/include" -I"/home/arch/Code/workspace/Relestyring/stlib/inc" -I"/home/arch/Code/workspace/Relestyring/CMSIS/Include" -I"/home/arch/Code/workspace/Relestyring/CMSIS/STM32F4xx/Include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


