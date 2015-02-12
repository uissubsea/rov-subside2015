################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/STM32F4xx/src/system_stm32f4xx.c 

S_UPPER_SRCS += \
../CMSIS/STM32F4xx/src/startup_stm32f40_41xxx.S 

OBJS += \
./CMSIS/STM32F4xx/src/startup_stm32f40_41xxx.o \
./CMSIS/STM32F4xx/src/system_stm32f4xx.o 

C_DEPS += \
./CMSIS/STM32F4xx/src/system_stm32f4xx.d 

S_UPPER_DEPS += \
./CMSIS/STM32F4xx/src/startup_stm32f40_41xxx.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/STM32F4xx/src/%.o: ../CMSIS/STM32F4xx/src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/STM32F4xx/src/%.o: ../CMSIS/STM32F4xx/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DUSE_STDPERIPH_DRIVER -DUSE_STM32_DISCOVERY -DHSE_VALUE=8000000 -DSTM32F40_41xxx -I"/home/arch/Code/workspace/STM32F4Template/CMSIS/STM32F4xx/Include" -I"/home/arch/Code/workspace/STM32F4Template/CMSIS/Include" -I"/home/arch/Code/workspace/STM32F4Template/stlib/inc" -I"/home/arch/Code/workspace/STM32F4Template/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


