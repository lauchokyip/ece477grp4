################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/display.c \
../Core/Src/gpio.c \
../Core/Src/json.c \
../Core/Src/main.c \
../Core/Src/motion.c \
../Core/Src/qr_scanner.c \
../Core/Src/retarget.c \
../Core/Src/stm32l476g_discovery.c \
../Core/Src/stm32l476g_discovery_glass_lcd.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/thermopile.c \
../Core/Src/utility.c \
../Core/Src/wifi_module.c 

OBJS += \
./Core/Src/display.o \
./Core/Src/gpio.o \
./Core/Src/json.o \
./Core/Src/main.o \
./Core/Src/motion.o \
./Core/Src/qr_scanner.o \
./Core/Src/retarget.o \
./Core/Src/stm32l476g_discovery.o \
./Core/Src/stm32l476g_discovery_glass_lcd.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/thermopile.o \
./Core/Src/utility.o \
./Core/Src/wifi_module.o 

C_DEPS += \
./Core/Src/display.d \
./Core/Src/gpio.d \
./Core/Src/json.d \
./Core/Src/main.d \
./Core/Src/motion.d \
./Core/Src/qr_scanner.d \
./Core/Src/retarget.d \
./Core/Src/stm32l476g_discovery.d \
./Core/Src/stm32l476g_discovery_glass_lcd.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/thermopile.d \
./Core/Src/utility.d \
./Core/Src/wifi_module.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/display.o: ../Core/Src/display.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/display.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/gpio.o: ../Core/Src/gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/json.o: ../Core/Src/json.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/json.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/main.o: ../Core/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/motion.o: ../Core/Src/motion.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/motion.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/qr_scanner.o: ../Core/Src/qr_scanner.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/qr_scanner.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/retarget.o: ../Core/Src/retarget.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/retarget.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32l476g_discovery.o: ../Core/Src/stm32l476g_discovery.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32l476g_discovery.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32l476g_discovery_glass_lcd.o: ../Core/Src/stm32l476g_discovery_glass_lcd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32l476g_discovery_glass_lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32l4xx_hal_msp.o: ../Core/Src/stm32l4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32l4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32l4xx_it.o: ../Core/Src/stm32l4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32l4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/sysmem.o: ../Core/Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/system_stm32l4xx.o: ../Core/Src/system_stm32l4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/system_stm32l4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/thermopile.o: ../Core/Src/thermopile.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/thermopile.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/utility.o: ../Core/Src/utility.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/utility.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/wifi_module.o: ../Core/Src/wifi_module.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L476xx -DDEBUG -c -I../Core/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -IC:/Users/acroy/STM32Cube/Repository/STM32Cube_FW_L4_V1.14.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/wifi_module.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

