################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/OS/tasks/task.c 

OBJS += \
./Core/OS/tasks/task.o 

C_DEPS += \
./Core/OS/tasks/task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/OS/tasks/task.o: ../Core/OS/tasks/task.c Core/OS/tasks/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DARM_MATH_CM4 -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/OS/tasks/task.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

