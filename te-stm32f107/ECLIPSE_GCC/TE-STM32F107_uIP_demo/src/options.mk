# инструменты сборки
TOOLS_VARIIANT=-kgp-eabi

include $(SRC_DIR)/../../SDK/scripts/make/common.mk

TARGET=TE_STM32F107

F_OCS=25000000
LD_SCRIPT=stm32f107vct6_rom.ld
FLASHSIZE=262144 
RAMSIZE=65536


COMMON_DEFS=-DSTM32F10X_CL

#HARDWARE_DEFS=-D$(TARGET) -DF_OCS=$(F_OCS) 

# процессор
CPU=cortex-m3

#задержка в crt коде для gdb
DELAY_FOR_GDB=1
USE_FREERTOS=1
APP_DEFS=-DDELAY_FOR_GDB=$(DELAY_FOR_GDB) -DUSE_FREERTOS=$(USE_FREERTOS)

USE_DHCP=1
UIP_DEFS+=-DUSE_DHCP=$(USE_DHCP)





# название пакета
DATE		= $$(date +%Y%m%d)
PKG_NAME	= $(LIBNAME)$(PRJNAME)

# опции библиотекаря
ARFLAGS = -rcs

CPFLAGS = -O ihex
ODFLAGS	= -x --syms

# общие определения
COMPILE_FLAGS=$(COMMON_DEFS)

# предупреждения
COMPILE_FLAGS+=-W -Wall -Wno-unused-parameter 
# оптимизация
#--combine -fwhole-program
COMPILE_FLAGS+= -Os -finline-functions -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-unroll-loops -fno-graphite
# отладка 
COMPILE_DEBUG_FLAGS=-ggdb3
COMPILE_FLAGS+=$(COMPILE_DEBUG_FLAGS)

# процессор
COMPILE_CPU_FLAGS=-mcpu=$(CPU) -mtune=$(CPU) -mfloat-abi=soft -mlittle-endian
COMPILE_FLAGS+=$(COMPILE_CPU_FLAGS)
# прочие флаги
#COMPILE_FLAGS+=-mlong-calls
# режим
COMPILE_FLAGS+=-mthumb
# флаги ассемблера
COMPILE_FLAGS+=-Wa,-adhlns=$(<:.c=.lst)
# генерация зависимостей
COMPILE_FLAGS+=-Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).dep

# флаги компиляции исходников на языке C
CFLAGS =   $(COMPILE_FLAGS) -Wa,-adhlns=$(<:.c=.lst) 
# версия стандарт языка С
CFLAGS+= -std=gnu99


LDFLAGS= -T $(SCRIPT_DIR)/$(LD_SCRIPT)  -nostartfiles  -mcpu=$(CPU) -mthumb  -mfpu=fpa -Wl,-gc-sections -L$(LIB_DIR)
#--warn-common

# флаги ассемблирования
ASFLAGS = $(COMPILE_CPU_FLAGS) $(COMPILE_DEBUG_FLAGS) -mapcs-32 -adhlns=$(<:.s=.lst) $(CRT_CONFIG)

