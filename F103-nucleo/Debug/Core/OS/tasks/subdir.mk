################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/OS/tasks/task.c 

OBJS += \
./Core/OS/tasks/task.o 

C_DEPS += \
./Core/OS/tasks/task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/OS/tasks/task.o: ../Core/OS/tasks/task.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/OS/tasks/task.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

