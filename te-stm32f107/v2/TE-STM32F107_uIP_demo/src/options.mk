# ����������� ������
TOOLS_VARIIANT=-kgp-eabi

include $(SRC_DIR)/../../SDK/scripts/make/common.mk

TARGET=TE_STM32F107

F_OCS=25000000
LD_SCRIPT=stm32f107vct6_rom.ld
FLASHSIZE=262144 
RAMSIZE=65536


COMMON_DEFS=-DSTM32F10X_CL

#HARDWARE_DEFS=-D$(TARGET) -DF_OCS=$(F_OCS) 

# ���������
CPU=cortex-m3

#�������� � crt ���� ��� gdb
DELAY_FOR_GDB=1
USE_FREERTOS=1
APP_DEFS=-DDELAY_FOR_GDB=$(DELAY_FOR_GDB) -DUSE_FREERTOS=$(USE_FREERTOS)

USE_DHCP=1
UIP_DEFS+=-DUSE_DHCP=$(USE_DHCP)





# �������� ������
DATE		= $$(date +%Y%m%d)
PKG_NAME	= $(LIBNAME)$(PRJNAME)

# ����� ������������
ARFLAGS = -rcs

CPFLAGS = -O ihex
ODFLAGS	= -x --syms

# ����� �����������
COMPILE_FLAGS=$(COMMON_DEFS)

# ��������������
COMPILE_FLAGS+=-W -Wall -Wno-unused-parameter 
# �����������
#--combine -fwhole-program
COMPILE_FLAGS+= -Os -finline-functions -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-unroll-loops -fno-graphite
# ������� 
COMPILE_DEBUG_FLAGS=-ggdb3
COMPILE_FLAGS+=$(COMPILE_DEBUG_FLAGS)

# ���������
COMPILE_CPU_FLAGS=-mcpu=$(CPU) -mtune=$(CPU) -mfloat-abi=soft -mlittle-endian
COMPILE_FLAGS+=$(COMPILE_CPU_FLAGS)
# ������ �����
#COMPILE_FLAGS+=-mlong-calls
# �����
COMPILE_FLAGS+=-mthumb
# ����� ����������
COMPILE_FLAGS+=-Wa,-adhlns=$(<:.c=.lst)
# ��������� ������������
COMPILE_FLAGS+=-Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).dep

# ����� ���������� ���������� �� ����� C
CFLAGS =   $(COMPILE_FLAGS) -Wa,-adhlns=$(<:.c=.lst) 
# ������ �������� ����� �
CFLAGS+= -std=gnu99


LDFLAGS= -T $(SCRIPT_DIR)/$(LD_SCRIPT)  -nostartfiles  -mcpu=$(CPU) -mthumb  -mfpu=fpa -Wl,-gc-sections -L$(LIB_DIR)
#--warn-common

# ����� ���������������
ASFLAGS = $(COMPILE_CPU_FLAGS) $(COMPILE_DEBUG_FLAGS) -mapcs-32 -adhlns=$(<:.s=.lst) $(CRT_CONFIG)

