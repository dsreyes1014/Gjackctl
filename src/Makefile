CC=gcc
CFLAGS=-Wall -std=gnu99 `pkg-config --cflags gtk+-3.0 jack` -c
PROGRAM=gjackctl
OBJECTS=main.o server_switch.o rt_box.o drivers.o alsa_device_names.o \
		dsp_load.o jack_server_init.o sample_rate.o gjackctl_settings.o \
		connections.o rt_priority.o server_name.o
HEADERS=server_switch.h rt_box.h drivers.h alsa_device_names.h dsp_load.h \
		jack_server_init.h sample_rate.h gjackctl_settings.h connections.h \
		rt_priority.h server_name.h
LIBS= `pkg-config --libs gtk+-3.0 alsa jack`

all:$(PROGRAM)
$(PROGRAM):$(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(PROGRAM)
main.o:main.c $(HEADERS)
	$(CC) $(CFLAGS) main.c 
server_switch.o: server_switch.c rt_box.h drivers.h alsa_device_names.h dsp_load.h \
					jack_server_init.h gjackctl_settings.h connections.h
	$(CC) $(CFLAGS) server_switch.c 
rt_box.o: rt_box.c 
	$(CC) $(CFLAGS) rt_box.c
drivers.o: drivers.c alsa_device_names.h
	$(CC) $(CFLAGS) drivers.c
alsa_device_names.o: alsa_device_names.c server_switch.h jack_server_init.h
	$(CC) $(CFLAGS) alsa_device_names.c
dsp_load.o: dsp_load.c
	$(CC) $(CFLAGS) dsp_load.c
jack_server_init.o: jack_server_init.c
	$(CC) $(CFLAGS) jack_server_init.c
sample_rate.o: sample_rate.c jack_server_init.h
	$(CC) $(CFLAGS) sample_rate.c
gjackctl_settings.o: gjackctl_settings.c sample_rate.h rt_box.h rt_priority.h \
						server_name.h
	$(CC) $(CFLAGS) gjackctl_settings.c
connections.o: connections.c
	$(CC) $(CFLAGS) connections.c
rt_priority.o: rt_priority.c jack_server_init.h
	$(CC) $(CFLAGS) rt_priority.c
server_name.o: server_name.c
	$(CC) $(CFLAGS) server_name.c

clean:
	rm -v *~ *.o $(PROGRAM)
