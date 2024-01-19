################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/WS2812/_drv_WS2812.c 

OBJS += \
./Drivers/WS2812/_drv_WS2812.o 

C_DEPS += \
./Drivers/WS2812/_drv_WS2812.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/WS2812/%.o Drivers/WS2812/%.su Drivers/WS2812/%.cyclo: ../Drivers/WS2812/%.c Drivers/WS2812/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../OS/FreeRTOS/Source/include -I../OS/FreeRTOS/Source/portable/GCC/ARM_CM4_MPU -I../OS/FreeRTOS/Source/portable/MemMang -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/WS2812 -I../Drivers/VGA -I../Drivers/Toslink -I../Drivers/SSD1306 -I../Drivers/Peripherals -I../Drivers/PCM1754 -I../Drivers/PAM8003 -I../Drivers/DMX -I../App/Test -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-WS2812

clean-Drivers-2f-WS2812:
	-$(RM) ./Drivers/WS2812/_drv_WS2812.cyclo ./Drivers/WS2812/_drv_WS2812.d ./Drivers/WS2812/_drv_WS2812.o ./Drivers/WS2812/_drv_WS2812.su

.PHONY: clean-Drivers-2f-WS2812

