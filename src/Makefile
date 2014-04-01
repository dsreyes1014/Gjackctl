CC=gcc
CFLAGS=-Wall -std=c99 `pkg-config --cflags gtk+-3.0` -c
PROGRAM=gjackctl
OBJECTS=main.o
LINK= `pkg-config --libs gtk+-3.0`

all:$(PROGRAM)
$(PROGRAM):$(OBJECTS)
	$(CC) $(OBJECTS) -o $(PROGRAM) $(LINK)
main.o:main.c
	$(CC) $(CFLAGS) main.c  
