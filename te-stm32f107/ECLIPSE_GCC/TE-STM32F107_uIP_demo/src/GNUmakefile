all:
	make -j2 all -C libcmsis
	make -j2 all -C libhardware
	make -j2 all -C libethernet
	make -j2 all -C libuip-1.0
	make -j2 all -C libfreertos-6.0.2
	make -j2 all -C application

clean:
	make clean -C libcmsis
	make clean -C libhardware
	make clean -C libethernet
	make clean -C libuip-1.0
	make clean -C libfreertos-6.0.2
	make clean -C application

app_all:
	make all -C application

app_clean:
	make clean -C application

# управление библиотекой cmsis
cmsis_all:
	make all -C libcmsis

cmsis_clean:
	make clean -C libcmsis

# управление библиотекой hardware
hardware_all:
	make all -C libhardware

hardware_clean:
	make clean -C libhardware

# управление библиотекой libethernet
ethernet_all:
	make all -C libethernet

ethernet_clean:
	make clean -C libethernet

# управление библиотекой libfreertos
freertos-6.0.2_all:
	make all -C libfreertos-6.0.2

freertos-6.0.2_clean:
	make clean -C libfreertos-6.0.2

# управление библиотекой libuip-1.0
uip-1.0_all:
	make all -C libuip-1.0

uip-1.0_clean:
	make clean -C libuip-1.0
	
uip-1.0_update_httpd_fsdata:
	make update_httpd_fsdata -C libuip-1.0
		