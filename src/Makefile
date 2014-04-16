CC=gcc
CFLAGS=-Wall -std=c99 `pkg-config --cflags gtk+-3.0` -c
PROGRAM=gjackctl
OBJECTS=main.o server_switch.o rt_box.o drivers.o driver_alsa_names.o
HEADERS=server_switch.h rt_box.h drivers.h driver_alsa_names.h
LINK= -lasound `pkg-config --libs gtk+-3.0`

all:$(PROGRAM)
$(PROGRAM):$(OBJECTS)
	$(CC) $(OBJECTS) $(LINK) -o $(PROGRAM)
main.o:main.c $(HEADERS)
	$(CC) $(CFLAGS) main.c 
server_switch.o: server_switch.c rt_box.h drivers.h driver_alsa_names.h
	$(CC) $(CFLAGS) server_switch.c 
rt_box.o: rt_box.c 
	$(CC) $(CFLAGS) rt_box.c
drivers.o: drivers.c driver_alsa_names.h
	$(CC) $(CFLAGS) drivers.c
driver_alsa_names.o: driver_alsa_names.c drivers.h
	$(CC) $(CFLAGS) driver_alsa_names.c

clean:
	rm -v *~ *.o $(PROGRAM)
