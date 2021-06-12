################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Telemetry/subdir.mk
-include Core/State_Space_Model/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/Sensors/subdir.mk
-include Core/Regulation/subdir.mk
-include Core/OS/tasks/subdir.mk
-include Core/OS/scheduler/subdir.mk
-include Core/OS/events/subdir.mk
-include Core/OS/debug/subdir.mk
-include Core/OS/subdir.mk
-include Core/High_lvl/subdir.mk
-include Core/Estimators/subdir.mk
-include Core/Data_Logger/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

BUILD_ARTIFACT_NAME := Kalman_Project
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX := 
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME).$(BUILD_ARTIFACT_EXTENSION)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Kalman_Project.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Kalman_Project.list \

OBJCOPY_BIN += \
Kalman_Project.bin \


# All Target
all: main-build

# Main-build Target
main-build: Kalman_Project.elf secondary-outputs

# Tool invocations
Kalman_Project.elf: $(OBJS) $(USER_OBJS) C:\WorkSpace\GitHub\STM32_Test_Area\F407_Kalman\STM32F407VGTX_FLASH.ld
	arm-none-eabi-gcc -o "Kalman_Project.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\WorkSpace\GitHub\STM32_Test_Area\F407_Kalman\STM32F407VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Kalman_Project.map" -Wl,--gc-sections -static -L"../Drivers/CMSIS/Lib/GCC" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Kalman_Project.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Kalman_Project.list"
	@echo 'Finished building: $@'
	@echo ' '

Kalman_Project.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Kalman_Project.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified
.SECONDARY:

-include ../makefile.targets
