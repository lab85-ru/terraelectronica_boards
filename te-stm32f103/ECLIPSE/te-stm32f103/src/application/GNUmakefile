SRC_DIR=./..

APPNAME = image

include $(SRC_DIR)/rules.mk

CFLAGS += -I./ \
		-I$(SRC_DIR)/include \
		-I$(STM32_PERIFERIAL_DRV_DIR)  \
        -I$(STM32_CMSIS_DIR)	\
		-I$(STM32_USB_DRV_DIR) \
		-I$(FATFS_DIR) \
		-I$(SRC_DIR)/libusbapp \
		-DUSE_STDPERIPH_DRIVER \
		-I$(SRC_DIR)/libdiskio	\
		$(HARDWARE_DEFS)		\
		$(APP_DEFS)

LIBS =  -lusbapp  -lfatfs -ldiskio -lusbcore -lhardware -lcmsis -lm

CUSTOM_RM_FILES=$(OUT_DIR)/image.*








