################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../sources/TE_STM32F207.c \
../sources/dataflash.c \
../sources/stm32f2xx_it.c 

OBJS += \
./sources/TE_STM32F207.o \
./sources/dataflash.o \
./sources/stm32f2xx_it.o 

C_DEPS += \
./sources/TE_STM32F207.d \
./sources/dataflash.d \
./sources/stm32f2xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
sources/%.o: ../sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -I"C:\workspace_gcc\te_stm32f207_dataflash" -I"C:\workspace_gcc\te_stm32f207_dataflash\sources" -I"C:\workspace_gcc\te_stm32f207_dataflash\system" -I"C:\workspace_gcc\te_stm32f207_dataflash\STM32F2xx_StdPeriph_Driver\inc" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


