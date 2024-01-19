################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/VGA/_drv_VGA.c 

OBJS += \
./Drivers/VGA/_drv_VGA.o 

C_DEPS += \
./Drivers/VGA/_drv_VGA.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/VGA/%.o Drivers/VGA/%.su Drivers/VGA/%.cyclo: ../Drivers/VGA/%.c Drivers/VGA/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../OS/FreeRTOS/Source/include -I../OS/FreeRTOS/Source/portable/GCC/ARM_CM4_MPU -I../OS/FreeRTOS/Source/portable/MemMang -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/WS2812 -I../Drivers/VGA -I../Drivers/Toslink -I../Drivers/SSD1306 -I../Drivers/Peripherals -I../Drivers/PCM1754 -I../Drivers/PAM8003 -I../Drivers/DMX -I../App/Test -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-VGA

clean-Drivers-2f-VGA:
	-$(RM) ./Drivers/VGA/_drv_VGA.cyclo ./Drivers/VGA/_drv_VGA.d ./Drivers/VGA/_drv_VGA.o ./Drivers/VGA/_drv_VGA.su

.PHONY: clean-Drivers-2f-VGA

