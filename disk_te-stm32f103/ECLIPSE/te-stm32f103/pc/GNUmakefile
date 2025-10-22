all: libusb0.cpp libusb0.h machaon_tester.cpp
	g++ -O2 -o libusb0.o     -c  libusb0.cpp
	g++ -O2 -o mach_tester.o -c  machaon_tester.cpp
	g++ -o mach_tester  libusb0.o mach_tester.o -lusb

clean:
	rm -fr *.o  mach_tester*
