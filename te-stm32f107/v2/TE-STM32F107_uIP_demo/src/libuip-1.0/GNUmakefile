SRC_DIR=./..

LIBNAME = libuip-1.0
FULLNAME = $(LIB_DIR)/$(LIBNAME).a

include $(SRC_DIR)/rules.mk

CFLAGS +=	-I./						\
			-I../						\
			-Itelnetd					\
			-Iwebserver					\
			-Idhcpc						\
			-Isntp						\
			-I$(SRC_DIR)/include		\
			-I$(SRC_DIR)/libcmsis		\
			-I$(SRC_DIR)/libhardware	\
			-I$(SRC_DIR)/application	\
			-I$(SRC_DIR)/libethernet

ifeq  ($(USE_FREERTOS),1)
  CFLAGS += -I$(SRC_DIR)/libfreertos-6.0.2
endif	


CFLAGS +=  $(UIP_DEFS)                 \
			$(APP_DEFS)


update_httpd_fsdata:
	make update_httpd_fsdata -C webserver

