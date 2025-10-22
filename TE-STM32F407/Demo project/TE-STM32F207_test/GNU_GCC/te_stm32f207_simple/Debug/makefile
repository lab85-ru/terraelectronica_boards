################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include system/subdir.mk
-include sources/subdir.mk
-include STM32F2xx_StdPeriph_Driver/src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
te_stm32f207_simple.hex \

SECONDARY_LIST += \
te_stm32f207_simple.lst \

SECONDARY_SIZE += \
te_stm32f207_simple.siz \


# All Target
all: te_stm32f207_simple.elf secondary-outputs

# Tool invocations
te_stm32f207_simple.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Yagarto Windows GCC C Linker'
	arm-none-eabi-gcc -T"C:\workspace\te_stm32f207_simple\stm32_flash.ld" -nostartfiles -Wl,-Map,te_stm32f207_simple.map -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"te_stm32f207_simple.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

te_stm32f207_simple.hex: te_stm32f207_simple.elf
	@echo 'Invoking: ARM Yagarto Windows GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex -j .text -j .data te_stm32f207_simple.elf "te_stm32f207_simple.hex"
	@echo 'Finished building: $@'
	@echo ' '

te_stm32f207_simple.lst: te_stm32f207_simple.elf
	@echo 'Invoking: ARM Yagarto Windows GNU Create Listing'
	arm-none-eabi-objdump -h -S te_stm32f207_simple.elf >"te_stm32f207_simple.lst"
	@echo 'Finished building: $@'
	@echo ' '

te_stm32f207_simple.siz: te_stm32f207_simple.elf
	@echo 'Invoking: ARM Yagarto Windows GNU Print Size'
	arm-none-eabi-size  --format=berkeley te_stm32f207_simple.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(EXECUTABLES)$(SECONDARY_LIST)$(S_UPPER_DEPS) te_stm32f207_simple.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
