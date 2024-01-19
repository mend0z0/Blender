################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/PAM8003/_drv_PAM8003.c 

OBJS += \
./Drivers/PAM8003/_drv_PAM8003.o 

C_DEPS += \
./Drivers/PAM8003/_drv_PAM8003.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/PAM8003/%.o Drivers/PAM8003/%.su Drivers/PAM8003/%.cyclo: ../Drivers/PAM8003/%.c Drivers/PAM8003/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../OS/FreeRTOS/Source/include -I../OS/FreeRTOS/Source/portable/GCC/ARM_CM4_MPU -I../OS/FreeRTOS/Source/portable/MemMang -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/WS2812 -I../Drivers/VGA -I../Drivers/Toslink -I../Drivers/SSD1306 -I../Drivers/Peripherals -I../Drivers/PCM1754 -I../Drivers/PAM8003 -I../Drivers/DMX -I../App/Test -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-PAM8003

clean-Drivers-2f-PAM8003:
	-$(RM) ./Drivers/PAM8003/_drv_PAM8003.cyclo ./Drivers/PAM8003/_drv_PAM8003.d ./Drivers/PAM8003/_drv_PAM8003.o ./Drivers/PAM8003/_drv_PAM8003.su

.PHONY: clean-Drivers-2f-PAM8003

