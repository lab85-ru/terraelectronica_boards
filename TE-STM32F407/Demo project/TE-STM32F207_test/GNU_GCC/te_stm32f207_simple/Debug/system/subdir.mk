################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/core_cm3.c \
../system/syscalls.c \
../system/system_stm32f2xx.c 

S_UPPER_SRCS += \
../system/startup_stm32f2xx.S 

OBJS += \
./system/core_cm3.o \
./system/startup_stm32f2xx.o \
./system/syscalls.o \
./system/system_stm32f2xx.o 

C_DEPS += \
./system/core_cm3.d \
./system/syscalls.d \
./system/system_stm32f2xx.d 

S_UPPER_DEPS += \
./system/startup_stm32f2xx.d 


# Each subdirectory must supply rules for building sources it contributes
system/%.o: ../system/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -I"C:\workspace\te_stm32f207_simple" -I"C:\workspace\te_stm32f207_simple\sources" -I"C:\workspace\te_stm32f207_simple\system" -I"C:\workspace\te_stm32f207_simple\STM32F2xx_StdPeriph_Driver\inc" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/%.o: ../system/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


