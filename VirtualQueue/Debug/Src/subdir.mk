################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/json.c \
../Src/main.c \
../Src/retarget.c \
../Src/stm32l476g_discovery.c \
../Src/stm32l476g_discovery_glass_lcd.c \
../Src/stm32l4xx_hal_msp.c \
../Src/stm32l4xx_it.c \
../Src/sysmem.c \
../Src/system_stm32l4xx.c 

OBJS += \
./Src/json.o \
./Src/main.o \
./Src/retarget.o \
./Src/stm32l476g_discovery.o \
./Src/stm32l476g_discovery_glass_lcd.o \
./Src/stm32l4xx_hal_msp.o \
./Src/stm32l4xx_it.o \
./Src/sysmem.o \
./Src/system_stm32l4xx.o 

C_DEPS += \
./Src/json.d \
./Src/main.d \
./Src/retarget.d \
./Src/stm32l476g_discovery.d \
./Src/stm32l476g_discovery_glass_lcd.d \
./Src/stm32l4xx_hal_msp.d \
./Src/stm32l4xx_it.d \
./Src/sysmem.d \
./Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/json.o: ../Src/json.c
<<<<<<< HEAD
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/json.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/retarget.o: ../Src/retarget.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/retarget.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l476g_discovery.o: ../Src/stm32l476g_discovery.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l476g_discovery.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l476g_discovery_glass_lcd.o: ../Src/stm32l476g_discovery_glass_lcd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l476g_discovery_glass_lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l4xx_hal_msp.o: ../Src/stm32l4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l4xx_it.o: ../Src/stm32l4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32l4xx.o: ../Src/system_stm32l4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32l4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
=======
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/json.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l476g_discovery.o: ../Src/stm32l476g_discovery.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l476g_discovery.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l476g_discovery_glass_lcd.o: ../Src/stm32l476g_discovery_glass_lcd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l476g_discovery_glass_lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l4xx_hal_msp.o: ../Src/stm32l4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l4xx_it.o: ../Src/stm32l4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32l4xx.o: ../Src/system_stm32l4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32l4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
>>>>>>> origin/nate

