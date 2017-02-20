all: skyx

sky_ip.o: sky_ip.cpp sky_ip.h 
	gcc -c sky_ip.cpp -o sky_ip.o

skyx: sky_ip.o sky_ip.h skyx.cpp
	gcc skyx.cpp sky_ip.o -o skyx


