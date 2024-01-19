################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../OS/FreeRTOS/Source/croutine.c \
../OS/FreeRTOS/Source/event_groups.c \
../OS/FreeRTOS/Source/list.c \
../OS/FreeRTOS/Source/queue.c \
../OS/FreeRTOS/Source/stream_buffer.c \
../OS/FreeRTOS/Source/task.c \
../OS/FreeRTOS/Source/timers.c 

OBJS += \
./OS/FreeRTOS/Source/croutine.o \
./OS/FreeRTOS/Source/event_groups.o \
./OS/FreeRTOS/Source/list.o \
./OS/FreeRTOS/Source/queue.o \
./OS/FreeRTOS/Source/stream_buffer.o \
./OS/FreeRTOS/Source/task.o \
./OS/FreeRTOS/Source/timers.o 

C_DEPS += \
./OS/FreeRTOS/Source/croutine.d \
./OS/FreeRTOS/Source/event_groups.d \
./OS/FreeRTOS/Source/list.d \
./OS/FreeRTOS/Source/queue.d \
./OS/FreeRTOS/Source/stream_buffer.d \
./OS/FreeRTOS/Source/task.d \
./OS/FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
OS/FreeRTOS/Source/%.o OS/FreeRTOS/Source/%.su OS/FreeRTOS/Source/%.cyclo: ../OS/FreeRTOS/Source/%.c OS/FreeRTOS/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../OS/FreeRTOS/Source/include -I../OS/FreeRTOS/Source/portable/GCC/ARM_CM4_MPU -I../OS/FreeRTOS/Source/portable/MemMang -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/WS2812 -I../Drivers/VGA -I../Drivers/Toslink -I../Drivers/SSD1306 -I../Drivers/Peripherals -I../Drivers/PCM1754 -I../Drivers/PAM8003 -I../Drivers/DMX -I../App/Test -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-OS-2f-FreeRTOS-2f-Source

clean-OS-2f-FreeRTOS-2f-Source:
	-$(RM) ./OS/FreeRTOS/Source/croutine.cyclo ./OS/FreeRTOS/Source/croutine.d ./OS/FreeRTOS/Source/croutine.o ./OS/FreeRTOS/Source/croutine.su ./OS/FreeRTOS/Source/event_groups.cyclo ./OS/FreeRTOS/Source/event_groups.d ./OS/FreeRTOS/Source/event_groups.o ./OS/FreeRTOS/Source/event_groups.su ./OS/FreeRTOS/Source/list.cyclo ./OS/FreeRTOS/Source/list.d ./OS/FreeRTOS/Source/list.o ./OS/FreeRTOS/Source/list.su ./OS/FreeRTOS/Source/queue.cyclo ./OS/FreeRTOS/Source/queue.d ./OS/FreeRTOS/Source/queue.o ./OS/FreeRTOS/Source/queue.su ./OS/FreeRTOS/Source/stream_buffer.cyclo ./OS/FreeRTOS/Source/stream_buffer.d ./OS/FreeRTOS/Source/stream_buffer.o ./OS/FreeRTOS/Source/stream_buffer.su ./OS/FreeRTOS/Source/task.cyclo ./OS/FreeRTOS/Source/task.d ./OS/FreeRTOS/Source/task.o ./OS/FreeRTOS/Source/task.su ./OS/FreeRTOS/Source/timers.cyclo ./OS/FreeRTOS/Source/timers.d ./OS/FreeRTOS/Source/timers.o ./OS/FreeRTOS/Source/timers.su

.PHONY: clean-OS-2f-FreeRTOS-2f-Source

