################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/display.c \
../Src/gpio.c \
../Src/json.c \
../Src/main.c \
../Src/motion.c \
../Src/qr_scanner.c \
../Src/retarget.c \
../Src/stm32l476g_discovery.c \
../Src/stm32l476g_discovery_glass_lcd.c \
../Src/stm32l4xx_hal_msp.c \
../Src/stm32l4xx_it.c \
../Src/sysmem.c \
../Src/system_stm32l4xx.c \
../Src/utility.c \
../Src/wifi_module.c 

OBJS += \
./Src/display.o \
./Src/gpio.o \
./Src/json.o \
./Src/main.o \
./Src/motion.o \
./Src/qr_scanner.o \
./Src/retarget.o \
./Src/stm32l476g_discovery.o \
./Src/stm32l476g_discovery_glass_lcd.o \
./Src/stm32l4xx_hal_msp.o \
./Src/stm32l4xx_it.o \
./Src/sysmem.o \
./Src/system_stm32l4xx.o \
./Src/utility.o \
./Src/wifi_module.o 

C_DEPS += \
./Src/display.d \
./Src/gpio.d \
./Src/json.d \
./Src/main.d \
./Src/motion.d \
./Src/qr_scanner.d \
./Src/retarget.d \
./Src/stm32l476g_discovery.d \
./Src/stm32l476g_discovery_glass_lcd.d \
./Src/stm32l4xx_hal_msp.d \
./Src/stm32l4xx_it.d \
./Src/sysmem.d \
./Src/system_stm32l4xx.d \
./Src/utility.d \
./Src/wifi_module.d 


# Each subdirectory must supply rules for building sources it contributes
Src/display.o: ../Src/display.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/display.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/gpio.o: ../Src/gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/json.o: ../Src/json.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/json.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/motion.o: ../Src/motion.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/motion.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/qr_scanner.o: ../Src/qr_scanner.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/qr_scanner.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/retarget.o: ../Src/retarget.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/retarget.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l476g_discovery.o: ../Src/stm32l476g_discovery.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l476g_discovery.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l476g_discovery_glass_lcd.o: ../Src/stm32l476g_discovery_glass_lcd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l476g_discovery_glass_lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l4xx_hal_msp.o: ../Src/stm32l4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32l4xx_it.o: ../Src/stm32l4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32l4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32l4xx.o: ../Src/system_stm32l4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32l4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/utility.o: ../Src/utility.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/utility.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/wifi_module.o: ../Src/wifi_module.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/Nate/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/wifi_module.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

