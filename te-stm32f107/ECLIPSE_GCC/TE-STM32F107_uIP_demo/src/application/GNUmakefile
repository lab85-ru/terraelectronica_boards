SRC_DIR=./..

APPNAME = image
FULLNAME = $(OUT_DIR)/$(APPNAME).elf
BINNAME = $(OUT_DIR)/$(APPNAME).bin

include $(SRC_DIR)/rules.mk

CFLAGS += -I./ \
		-I$(SRC_DIR)/include \
		-I$(SRC_DIR)/libhardware \
		-I$(SRC_DIR)/libcmsis \
		-I$(SRC_DIR)/libethernet \
		-I$(SRC_DIR)/libuip-1.0

ifeq  ($(USE_FREERTOS),1)
  CFLAGS += -I$(SRC_DIR)/libfreertos-6.0.2
endif	
		
CFLAGS += -DUSE_STDPERIPH_DRIVER \
		  $(APP_DEFS) \
		  $(FREERTOS_DEFS) \
		  $(HARDWARE_DEFS)

LIBS=    -luip-1.0          \
         -lethernet

ifeq ($(USE_FREERTOS),1)
  LIBS += -lfreertos-6.0.2
endif

LIBS +=  -lhardware         \
         -lcmsis            \
         -lm



CUSTOM_RM_FILES=$(OUT_DIR)/image.*






