################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Test/_app_test.c 

OBJS += \
./App/Test/_app_test.o 

C_DEPS += \
./App/Test/_app_test.d 


# Each subdirectory must supply rules for building sources it contributes
App/Test/%.o App/Test/%.su App/Test/%.cyclo: ../App/Test/%.c App/Test/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../OS/FreeRTOS/Source/include -I../OS/FreeRTOS/Source/portable/GCC/ARM_CM4_MPU -I../OS/FreeRTOS/Source/portable/MemMang -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/WS2812 -I../Drivers/VGA -I../Drivers/Toslink -I../Drivers/SSD1306 -I../Drivers/Peripherals -I../Drivers/PCM1754 -I../Drivers/PAM8003 -I../Drivers/DMX -I../App/Test -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Test

clean-App-2f-Test:
	-$(RM) ./App/Test/_app_test.cyclo ./App/Test/_app_test.d ./App/Test/_app_test.o ./App/Test/_app_test.su

.PHONY: clean-App-2f-Test

