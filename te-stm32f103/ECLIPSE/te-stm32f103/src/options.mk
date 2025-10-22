PRJ_NAME=te-stm32f103

TOOLS_VARIIANT=arm-kgp-eabi

SDK_DIR=../../../SDK_embedded
#FREERTOS_KERNEL_SRC_DIR=$(SDK_DIR)/libs/FreeRTOS_v6.0.4/Source
#FREERTOS_PORTABLE_SRC_DIR=$(FREERTOS_KERNEL_SRC_DIR)/portable/GCC/ARM_CM3
STM32_PERIFERIAL_DRV_DIR=$(SDK_DIR)/libs/stm32/STM32F10x_StdPeriph_Driver
STM32_CMSIS_DIR=$(SDK_DIR)/libs/stm32/CMSIS
#STM32_DSP_DIR=$(SDK_DIR)/libs/stm32/STM32F10x_DSP_Lib
STM32_USB_DRV_DIR=$(SDK_DIR)/libs/stm32/STM32_USB_FS_Device_Driver
FATFS_DIR=$(SDK_DIR)/libs/fatfs/src

#����� ��������� ������� ������� � ������� ������� ������������� ������
FLASH_ORIGIN=0x08000000
RAM_ORIGIN=0x20000000
FLASH_SIZE=512K
RAM_SIZE=64K

#��� ����������   
CPU=cortex-m3

COMMON_DEFS=-DSTM32F10X_HD

# ������� ���������� ������� ����������
F_OCS=8000000
# ����������� ������� ������� � RAM
RAM_VEC_TABLE=1
# ������������ �����
HARDWARE_DEFS= -DF_OCS=$(F_OCS) -DRAM_VEC_TABLE=$(RAM_VEC_TABLE)


# ������������� FreeRTOS
TICK_RATE_HZ_DEFAULT=1000UL
MAX_PRIORITIES=4UL
MINIMAL_STACK_SIZE=128UL
TOTAL_HEAP_SIZE="(4*1024UL)"
MAX_TASK_NAME_LEN=32UL

FREERTOS_DEFS= -DF_OCS=$(F_OCS)									\
               -DTICK_RATE_HZ_DEFAULT=$(TICK_RATE_HZ_DEFAULT)	\
               -DMAX_PRIORITIES=$(MAX_PRIORITIES)				\
               -DMINIMAL_STACK_SIZE=$(MINIMAL_STACK_SIZE)		\
               -DTOTAL_HEAP_SIZE=$(TOTAL_HEAP_SIZE)				\
               -DMAX_TASK_NAME_LEN=$(MAX_TASK_NAME_LEN)

#�������� � crt ���� ��� gdb
DELAY_FOR_GDB=1
USE_FREERTOS=1
APP_DEFS=-DDELAY_FOR_GDB=$(DELAY_FOR_GDB) -DUSE_FREERTOS=$(USE_FREERTOS)

# ����� �����������
COMPILE_FLAGS=$(COMMON_DEFS)

# ��������������
COMPILE_FLAGS+=-W -Wall -Wno-unused-parameter 
# �����������
#--combine -fwhole-program
COMPILE_FLAGS+= -Os -finline-functions -fomit-frame-pointer -ffunction-sections -fdata-sections -funroll-loops
# -fgraphite
# ������� 
COMPILE_DEBUG_FLAGS=-ggdb3
COMPILE_FLAGS+=$(COMPILE_DEBUG_FLAGS)

# ���������
COMPILE_CPU_FLAGS=-mcpu=$(CPU)  -mfloat-abi=soft -mlittle-endian
#-mtune=$(CPU)
COMPILE_FLAGS+=$(COMPILE_CPU_FLAGS)
# ������ �����
COMPILE_FLAGS+=-mlong-calls
# �����
COMPILE_FLAGS+=-mthumb
# ��������� ������������
COMPILE_FLAGS+=-Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).dep

# ����� ���������� ���������� �� ����� C
CFLAGS =   $(COMPILE_FLAGS) -Wa,-adhlns=$@.lst 
# ������ �������� ����� �
CFLAGS+= -std=gnu99

#����� �������
LDFLAGS=  -nostartfiles  -mcpu=$(CPU) -mthumb  -mfpu=fpa -Wl,-gc-sections -L$(LIB_DIR)
#--warn-common

# ����� ���������������
ASFLAGS = $(COMPILE_CPU_FLAGS) $(COMPILE_DEBUG_FLAGS) -mapcs-32 -adhlns=$@.lst $(CRT_CONFIG)

# ����� ���������� ���������� �� ����� �++
CXXFLAGS = $(COMPILE_FLAGS) -Wa,-adhlns=$@.lst 

# ����� ���������� ���������� �� ����� �������
FFLAGS = $(COMPILE_FLAGS) -Wa,-adhlns=$@.lst 
# ����� �����
FFLAGS += -ffree-form

