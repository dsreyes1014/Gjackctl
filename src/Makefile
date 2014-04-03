CC=gcc
CFLAGS=-Wall -std=c99 `pkg-config --cflags gtk+-3.0` -c
PROGRAM=gjackctl
OBJECTS=main.o server_switch.o
HEADERS=server_switch.h
LINK= `pkg-config --libs gtk+-3.0`

all:$(PROGRAM)
$(PROGRAM):$(OBJECTS)
	$(CC) $(OBJECTS) $(LINK) -o $(PROGRAM)
main.o:main.c $(HEADERS)
	$(CC) $(CFLAGS) main.c 
server_switch.o: server_switch.c
	$(CC) $(CFLAGS) server_switch.c 

clean:
	rm -v *~ *.o $(PROGRAM)
