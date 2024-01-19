################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/Source/portable/GCC/ARM_CM4_MPU/port.c 

OBJS += \
./Core/Inc/Source/portable/GCC/ARM_CM4_MPU/port.o 

C_DEPS += \
./Core/Inc/Source/portable/GCC/ARM_CM4_MPU/port.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/Source/portable/GCC/ARM_CM4_MPU/%.o Core/Inc/Source/portable/GCC/ARM_CM4_MPU/%.su Core/Inc/Source/portable/GCC/ARM_CM4_MPU/%.cyclo: ../Core/Inc/Source/portable/GCC/ARM_CM4_MPU/%.c Core/Inc/Source/portable/GCC/ARM_CM4_MPU/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source/portable/Common" -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source/include" -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source/portable/GCC/ARM_CM4_MPU" -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source/portable/MemMang" -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source" -I"D:/Projects/STP/Blender/Firmware/v1_20231222/Design Folder/_FW_Blender/Core/Inc/Source/portable" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4_MPU

clean-Core-2f-Inc-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4_MPU:
	-$(RM) ./Core/Inc/Source/portable/GCC/ARM_CM4_MPU/port.cyclo ./Core/Inc/Source/portable/GCC/ARM_CM4_MPU/port.d ./Core/Inc/Source/portable/GCC/ARM_CM4_MPU/port.o ./Core/Inc/Source/portable/GCC/ARM_CM4_MPU/port.su

.PHONY: clean-Core-2f-Inc-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4_MPU

