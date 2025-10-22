include $(SDK_DIR)/scripts/make/options.mk

ASSRCS 		=  $(wildcard $(addsuffix /*.s   , $(LIB_SRC_DIR)))
CSRCS 		=  $(wildcard $(addsuffix /*.c   , $(LIB_SRC_DIR)))
CXXSRCS_CC 	=  $(wildcard $(addsuffix /*.cc  , $(LIB_SRC_DIR)))
CXXSRCS_CPP 	=  $(wildcard $(addsuffix /*.cpp , $(LIB_SRC_DIR)))
FSRCS 		=  $(wildcard $(addsuffix /*.f   , $(LIB_SRC_DIR)))

DEPEND_LIBS = $(wildcard $(LIB_DIR)/*.stamp)

OBJS = $(notdir $(ASSRCS:.s=.o) $(FSRCS:.f=.o) $(CSRCS:.c=.o) $(CXXSRCS_CC:.cc=.o) $(CXXSRCS_CPP:.cpp=.o))    
LSTS = $(OBJS:=.lst)

BUILD_SCRIPTS = $(wildcard $(addsuffix /*.mk   , $(SDK_DIR)/scripts/make))	\
                $(wildcard $(addsuffix /*.mk   , $(SRC_DIR)/options.mk $(SRC_DIR)))

all:$(FULLNAME) $(BINNAME)

%.ld:$(LD_SCRIPT_M4) $(BUILD_SCRIPTS)
	$(M4) $(LDGENFLAGS) $< > $(LD_SCRIPT)

%.a:$(OBJS) $(BUILD_SCRIPTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	$(TOUCH)  $@.stamp

%.o:$(LIB_SRC_DIR)/%.c $(BUILD_SCRIPTS)
	$(CC)  $(CFLAGS) -c -o $@  $<

%.o:$(LIB_SRC_DIR)/%.s $(BUILD_SCRIPTS)
	$(AS) $(ASFLAGS) -o $@  $<

%.o:$(LIB_SRC_DIR)/%.cc $(BUILD_SCRIPTS)
	$(CXX) $(CXXFLAGS) -c -o $@  $<

%.o:$(LIB_SRC_DIR)/%.cpp $(BUILD_SCRIPTS)
	$(CXX) $(CXXFLAGS) -c -o $@  $<

%.o:$(LIB_SRC_DIR)/%.f $(BUILD_SCRIPTS)
	$(FC) $(FFLAGS) -c -o $@  $<

%.elf:$(OBJS) $(DEPEND_LIBS) $(LD_SCRIPT)
	$(CC) $(LD_SCRIPT_OPT) $(LDFLAGS) $(OBJS) $(LIBS) -o $@ -Wl,-Map,$(OUT_DIR)/$(APPNAME).map
	$(MEMUTZ) $@ $(FLASH_SIZE) $(RAM_SIZE)

$(BINNAME):$(FULLNAME)
	$(CP) $(CPFLAGS) $(OUT_DIR)/$(APPNAME).elf $(OUT_DIR)/$(APPNAME).hex
	$(CP) --strip-all -O binary -S $(OUT_DIR)/$(APPNAME).elf $(OUT_DIR)/$(APPNAME).bin
	$(OD) $(ODFLAGS) $(OUT_DIR)/$(APPNAME).elf > $(OUT_DIR)/$(APPNAME).dmp
	$(OD) -h -C -S $(OUT_DIR)/image.elf > $(OUT_DIR)/$(APPNAME).lss

clean:
	$(RM) -fr $(FULLNAME) $(OBJS) $(LSTS) $(CUSTOM_RM_FILES) .dep

dist:clean
	$(TAR) --exclude={CVS,cvs} -cvzf $(PKG_NAME)-$(DATE).tar.gz *

tools_version:
	$(CC)  -v
	$(LD)  -v
	$(GDB) -v

# phony targets
.PHONY:all clean dist tools_version
.PRECIOUS: %.o %.o.lst %.ld

-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
