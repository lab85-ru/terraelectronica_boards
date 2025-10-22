#
TARGET = demo
LDSCRIPT = lpc2478-rom.ld
GDBINIT = gdbinit.script
OPTIMIZATION = 0
SOURCEDIRS = .
SOURCEFILES = $(wildcard $(addsuffix /*.c,$(SOURCEDIRS)))
OBJECTFILES = Startup.o $(patsubst %.c,%.o,$(SOURCEFILES))
DEPENDSFILES = $(patsubst %.c,%.d,$(SOURCEFILES))
# 
CC = arm-elf-gcc
CFLAGS = -std=gnu99 -O$(OPTIMIZATION)
CFLAGS += -Wall
#CFLAGS = -mcpu=arm7tdmi -mtune=arm7tdmi
CFLAGS += -gdwarf-2 -g3 # Включаем отладочную информацию и информацию о макросах
CFLAGS += $(addprefix -I,$(SOURCEDIRS))
#CFLAGS += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<)
AS = arm-elf-as
AFLAGS = -gdwarf-2
LD = arm-elf-ld
LDFLAGS = -lc -nostartfiles -T$(LDSCRIPT)
AR = arm-elf-ar
CP = arm-elf-objcopy
OD = arm-elf-objdump
GDB = arm-elf-gdb
SIZE = arm-elf-size
CPFLAGS = -O binary
HEXFLAGS = -O ihex
ODFLAGS =
VPATH = $(SOURCEDIRS)

.PHONY:	clean gdb-server gdb

all: $(TARGET) $(TARGET).bin $(TARGET).hex

echo:
	echo $(SOURCEDIRS)
	echo $(addsuffix /*.c,$(SOURCEDIRS))
	echo $(SOURCEFILES)
	echo $(OBJECTFILES)

-include $(DEPENDSFILES)

$(TARGET): $(TARGET).elf
	$(SIZE) $(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	$(CP) $(CPFLAGS) $(TARGET).elf $(TARGET).bin

$(TARGET).hex: $(TARGET).elf
	$(CP) $(HEXFLAGS) $(TARGET).elf $(TARGET).hex

$(TARGET).elf: $(OBJECTFILES)
	$(CC) $(CFLAGS) -o $(TARGET).elf $(OBJECTFILES) $(LDFLAGS)

Startup.o: Startup.s
	$(AS) $(AFLAGS) -o Startup.o Startup.s

%.d: %.c
	set -e; $(CC) -MM $(addprefix -I,$(SOURCEDIRS)) $< | sed 's/\.o[ :]*/\.o $(subst /,\/,$@) : /' | sed 's/^\(.*\.o\)/$(subst /,\/,$(dir $<))\1/' > $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJECTFILES) $(DEPENDSFILES) $(patsubst %.c,%.lst,$(SOURCEFILES)) $(TARGET).elf $(TARGET).bin $(TARGET).hex

gdb-server:
	openocd -f openocd.cfg

gdb:
	$(GDB) -x $(GDBINIT) $(TARGET).elf

