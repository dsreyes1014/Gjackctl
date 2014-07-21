CC = gcc
CFLAGS = -Wall -std=gnu99 `pkg-config --cflags gtk+-3.0 jack alsa libconfig` -c
PROGRAM = gjackctl
OBJECTS = main.o server_switch.o rt_box.o drivers.o alsa_device_names.o \
		  dsp_load.o jack_server_init.o sample_rate.o gjackctl_settings.o \
		  connections.o rt_priority.o server_name.o display.o no_memlock.o \
          get_arg_vector.o config_file_input.o config_file_init.o midi.o\
          label_toggle_attr.o port_max.o
HEADERS = server_switch.h rt_box.h drivers.h alsa_device_names.h dsp_load.h \
		  jack_server_init.h sample_rate.h gjackctl_settings.h connections.h \
		  rt_priority.h server_name.h display.h no_memlock.h get_arg_vector.h \
          config_file_input.h config_file_init.h label_toggle_attr.h midi.h \
          port_max.h
LIBS= `pkg-config --libs gtk+-3.0 alsa jack libconfig`

all: $(PROGRAM)
$(PROGRAM): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(PROGRAM)
main.o: main.c server_switch.h display.h config_file_init.h
	$(CC) $(CFLAGS) main.c 
server_switch.o: server_switch.c dsp_load.h jack_server_init.h \
					gjackctl_settings.h connections.h
	$(CC) $(CFLAGS) server_switch.c
rt_box.o: rt_box.c get_arg_vector.h config_file_input.h label_toggle_attr.h
	$(CC) $(CFLAGS) rt_box.c
drivers.o: drivers.c alsa_device_names.h get_arg_vector.h
	$(CC) $(CFLAGS) drivers.c
alsa_device_names.o: alsa_device_names.c server_switch.h get_arg_vector.h config_file_input.h
	$(CC) $(CFLAGS) alsa_device_names.c
dsp_load.o: dsp_load.c jack_server_init.h display.h
	$(CC) $(CFLAGS) dsp_load.c
jack_server_init.o: jack_server_init.c dsp_load.h
	$(CC) $(CFLAGS) jack_server_init.c
sample_rate.o: sample_rate.c jack_server_init.h get_arg_vector.h config_file_input.h
	$(CC) $(CFLAGS) sample_rate.c
gjackctl_settings.o: gjackctl_settings.c sample_rate.h rt_box.h rt_priority.h \
                     server_name.h no_memlock.h midi.h port_max.h
	$(CC) $(CFLAGS) gjackctl_settings.c
connections.o: connections.c
	$(CC) $(CFLAGS) connections.c
rt_priority.o: rt_priority.c jack_server_init.h get_arg_vector.h config_file_input.h
	$(CC) $(CFLAGS) rt_priority.c
server_name.o: server_name.c
	$(CC) $(CFLAGS) server_name.c
display.o: display.c
	$(CC) $(CFLAGS) display.c
no_memlock.o: no_memlock.c get_arg_vector.h config_file_input.h
	$(CC) $(CFLAGS) no_memlock.c
get_arg_vector.o: get_arg_vector.c
	$(CC) $(CFLAGS) get_arg_vector.c
config_file_input.o: config_file_input.c
	$(CC) $(CFLAGS) config_file_input.c
config_file_init.o: config_file_init.c
	$(CC) $(CFLAGS) config_file_init.c
label_toggle_attr.o: label_toggle_attr.c
	$(CC) $(CFLAGS) label_toggle_attr.c
midi.o: midi.c
	$(CC) $(CFLAGS) midi.c
port_max.o: port_max.c
	$(CC) $(CFLAGS) port_max.c

clean:
	rm -vf *~ *.o $(PROGRAM)
