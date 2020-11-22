################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/Sequence_led.c 

OBJS += \
./Drivers/Src/Sequence_led.o 

C_DEPS += \
./Drivers/Src/Sequence_led.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/Sequence_led.o: ../Drivers/Src/Sequence_led.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Src/Sequence_led.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

