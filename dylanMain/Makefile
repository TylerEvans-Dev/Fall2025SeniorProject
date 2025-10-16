CC = gcc
CFLAGS= -Wall -O2
LIB = libtof.a


all: main stop

$(LIB): tof.o
	ar rcs $(LIB) tof.o

tof.o: ./tofFiles/tof.c ./tofFiles/tof.h
	$(CC) $(CFLAGS) -c ./tofFiles/tof.c

main: main.c $(LIB)
	$(CC) $(CFLAGS) main.c -L. -ltof -o main -lwiringPi

stop: stop.c
	$(CC) $(CFLAGS) stop.c -o stop -lwiringPi

clean:
	rm -f *.o $(LIB) main stop

