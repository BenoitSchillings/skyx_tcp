all: skyx

./obj/sky_ip.o: sky_ip.cpp sky_ip.h 
	gcc -c sky_ip.cpp -o ./obj/sky_ip.o

skyx: ./obj/sky_ip.o sky_ip.h skyx.cpp
	gcc skyx.cpp ./obj/sky_ip.o -o skyx


