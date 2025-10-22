all:

	make -j2 all -C libcmsis
	make -j2 all -C libhardware
	make -j2 all -C libusbcore
	make -j2 all -C libusbapp
	make -j2 all -C libfatfs
	make -j2 all -C libdiskio
	make -j2 all -C application


clean:
	
	make clean -C libcmsis
	make clean -C libhardware
	make clean -C libusbcore
	make clean -C libusbapp
	make clean -C libfatfs
	make clean -C libdiskio
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

# управление библиотекой libusbcore
usbcore_all:
	make all -C libusbcore

usbcore_clean:
	make clean -C libusbcore

# управление библиотекой libusbapp
usbapp_all:
	make all -C libusbapp

usbapp_clean:
	make clean -C libusbapp

# управление библиотекой libfatfs
fatfs_all:
	make all -C libfatfs

fatfs_clean:
	make clean -C libfatfs

# управление библиотекой libdiskio
diskio_all:
	make all -C libdiskio

diskio_clean:
	make clean -C libdiskio				