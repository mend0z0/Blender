################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/_app_test.c \
../Core/Src/_drv_DMX.c \
../Core/Src/_drv_ISR.c \
../Core/Src/_drv_PAM8003.c \
../Core/Src/_drv_PCM1754.c \
../Core/Src/_drv_SSD1306.c \
../Core/Src/_drv_Toslink.c \
../Core/Src/_drv_VGA.c \
../Core/Src/_drv_WS2812.c \
../Core/Src/_drv_peripherals.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/_app_test.o \
./Core/Src/_drv_DMX.o \
./Core/Src/_drv_ISR.o \
./Core/Src/_drv_PAM8003.o \
./Core/Src/_drv_PCM1754.o \
./Core/Src/_drv_SSD1306.o \
./Core/Src/_drv_Toslink.o \
./Core/Src/_drv_VGA.o \
./Core/Src/_drv_WS2812.o \
./Core/Src/_drv_peripherals.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/_app_test.d \
./Core/Src/_drv_DMX.d \
./Core/Src/_drv_ISR.d \
./Core/Src/_drv_PAM8003.d \
./Core/Src/_drv_PCM1754.d \
./Core/Src/_drv_SSD1306.d \
./Core/Src/_drv_Toslink.d \
./Core/Src/_drv_VGA.d \
./Core/Src/_drv_WS2812.d \
./Core/Src/_drv_peripherals.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/_app_test.cyclo ./Core/Src/_app_test.d ./Core/Src/_app_test.o ./Core/Src/_app_test.su ./Core/Src/_drv_DMX.cyclo ./Core/Src/_drv_DMX.d ./Core/Src/_drv_DMX.o ./Core/Src/_drv_DMX.su ./Core/Src/_drv_ISR.cyclo ./Core/Src/_drv_ISR.d ./Core/Src/_drv_ISR.o ./Core/Src/_drv_ISR.su ./Core/Src/_drv_PAM8003.cyclo ./Core/Src/_drv_PAM8003.d ./Core/Src/_drv_PAM8003.o ./Core/Src/_drv_PAM8003.su ./Core/Src/_drv_PCM1754.cyclo ./Core/Src/_drv_PCM1754.d ./Core/Src/_drv_PCM1754.o ./Core/Src/_drv_PCM1754.su ./Core/Src/_drv_SSD1306.cyclo ./Core/Src/_drv_SSD1306.d ./Core/Src/_drv_SSD1306.o ./Core/Src/_drv_SSD1306.su ./Core/Src/_drv_Toslink.cyclo ./Core/Src/_drv_Toslink.d ./Core/Src/_drv_Toslink.o ./Core/Src/_drv_Toslink.su ./Core/Src/_drv_VGA.cyclo ./Core/Src/_drv_VGA.d ./Core/Src/_drv_VGA.o ./Core/Src/_drv_VGA.su ./Core/Src/_drv_WS2812.cyclo ./Core/Src/_drv_WS2812.d ./Core/Src/_drv_WS2812.o ./Core/Src/_drv_WS2812.su ./Core/Src/_drv_peripherals.cyclo ./Core/Src/_drv_peripherals.d ./Core/Src/_drv_peripherals.o ./Core/Src/_drv_peripherals.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

