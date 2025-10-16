CC = gcc
CFLAGS= -Wall -O2


all: mainPCA 

tof.o: tof.c tof.h
	$(CC) $(CFLAGS) -c tof.c

mainPCA: mainPCA.c tof.o
	$(CC) $(CFLAGS) mainPCA.c tof.o -o mainPCA -lwiringPi

clean:
	rm -f *.o mainPCA

