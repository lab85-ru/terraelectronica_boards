SRC_DIR=./..

LIBNAME = libusbapp
FULLNAME = $(LIB_DIR)/$(LIBNAME).a

include $(SRC_DIR)/rules.mk

CFLAGS += -I./                      \
		-I$(STM32_USB_DRV_DIR)		\
		-I$(STM32_CMSIS_DIR)		\
		-I$(STM32_PERIFERIAL_DRV_DIR)	\
        -I$(SRC_DIR)/include		\
        -I$(SRC_DIR)/application	\
        $(HARDWARE_DEFS)





