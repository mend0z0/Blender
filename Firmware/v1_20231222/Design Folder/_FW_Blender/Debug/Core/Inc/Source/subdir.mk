################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/Source/croutine.c \
../Core/Inc/Source/event_groups.c \
../Core/Inc/Source/list.c \
../Core/Inc/Source/queue.c \
../Core/Inc/Source/stream_buffer.c \
../Core/Inc/Source/tasks.c \
../Core/Inc/Source/timers.c 

OBJS += \
./Core/Inc/Source/croutine.o \
./Core/Inc/Source/event_groups.o \
./Core/Inc/Source/list.o \
./Core/Inc/Source/queue.o \
./Core/Inc/Source/stream_buffer.o \
./Core/Inc/Source/tasks.o \
./Core/Inc/Source/timers.o 

C_DEPS += \
./Core/Inc/Source/croutine.d \
./Core/Inc/Source/event_groups.d \
./Core/Inc/Source/list.d \
./Core/Inc/Source/queue.d \
./Core/Inc/Source/stream_buffer.d \
./Core/Inc/Source/tasks.d \
./Core/Inc/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/Source/%.o Core/Inc/Source/%.su Core/Inc/Source/%.cyclo: ../Core/Inc/Source/%.c Core/Inc/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source/portable/Common" -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source/include" -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source/portable/GCC/ARM_CM4_MPU" -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source/portable/MemMang" -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source" -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source/portable" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-Source

clean-Core-2f-Inc-2f-Source:
	-$(RM) ./Core/Inc/Source/croutine.cyclo ./Core/Inc/Source/croutine.d ./Core/Inc/Source/croutine.o ./Core/Inc/Source/croutine.su ./Core/Inc/Source/event_groups.cyclo ./Core/Inc/Source/event_groups.d ./Core/Inc/Source/event_groups.o ./Core/Inc/Source/event_groups.su ./Core/Inc/Source/list.cyclo ./Core/Inc/Source/list.d ./Core/Inc/Source/list.o ./Core/Inc/Source/list.su ./Core/Inc/Source/queue.cyclo ./Core/Inc/Source/queue.d ./Core/Inc/Source/queue.o ./Core/Inc/Source/queue.su ./Core/Inc/Source/stream_buffer.cyclo ./Core/Inc/Source/stream_buffer.d ./Core/Inc/Source/stream_buffer.o ./Core/Inc/Source/stream_buffer.su ./Core/Inc/Source/tasks.cyclo ./Core/Inc/Source/tasks.d ./Core/Inc/Source/tasks.o ./Core/Inc/Source/tasks.su ./Core/Inc/Source/timers.cyclo ./Core/Inc/Source/timers.d ./Core/Inc/Source/timers.o ./Core/Inc/Source/timers.su

.PHONY: clean-Core-2f-Inc-2f-Source

