# установка файловой структуры проъекта
ROOT_DIR=$(SRC_DIR)/..
OUT_DIR=$(ROOT_DIR)/out
LIB_DIR=$(ROOT_DIR)/lib
DOC_DIR=$(ROOT_DIR)/doc
PC_DIR=$(ROOT_DIR)/pc
SCRIPT_DIR=$(ROOT_DIR)/scripts

# настройка путевой системы проекта
ifeq ($(origin LIB_SRC_DIR), undefined)
  LIB_SRC_DIR=.
endif

# вычисление целей сборки
ifdef LIBNAME
	FULLNAME = $(LIB_DIR)/$(LIBNAME).a
endif

ifdef APPNAME
	FULLNAME = $(OUT_DIR)/$(APPNAME).elf
	BINNAME = $(OUT_DIR)/$(APPNAME).bin
endif

# установка m4 шаблона генерации ld скрипта по умолчанию
ifndef LD_SCRIPT_M4
   LD_SCRIPT_M4 = $(SDK_DIR)/scripts/ld/ld.m4
endif

#вычисление командной строки m4 для генерации ld скрипта 
LDGENFLAGS+=-DFLASH_ORIGIN=$(FLASH_ORIGIN)	\
			-DFLASH_SIZE=$(FLASH_SIZE)		\
			-DRAM_ORIGIN=$(RAM_ORIGIN)		\
			-DRAM_SIZE=$(RAM_SIZE) 

#имя файла скрипта ld и опции с ним связанные 
LD_SCRIPT=$(SCRIPT_DIR)/$(PRJ_NAME).ld
LD_SCRIPT_OPT=-T $(LD_SCRIPT)

# инструменты сборки
CC      = $(TOOLS_VARIIANT)-gcc
CXX     = $(TOOLS_VARIIANT)-g++
FC      = $(TOOLS_VARIIANT)-gfortran
LD      = $(TOOLS_VARIIANT)-ld
AR      = $(TOOLS_VARIIANT)-ar
AS      = $(TOOLS_VARIIANT)-as
CP      = $(TOOLS_VARIIANT)-objcopy
OD		= $(TOOLS_VARIIANT)-objdump
SZ      = $(TOOLS_VARIIANT)-size
SR      = $(TOOLS_VARIIANT)-strip
GDB     = $(TOOLS_VARIIANT)-gdb
RM		= rm
TAR     = tar
TOUCH   = touch
M4 		= m4
MEMUTZ	= memutz

# название пакета
DATE		= $$(date +%Y%m%d)
PKG_NAME	= $(LIBNAME)$(PRJNAME)

# опции библиотекаря
ARFLAGS = -rcs


CPFLAGS = -O ihex
ODFLAGS	= -x --syms
