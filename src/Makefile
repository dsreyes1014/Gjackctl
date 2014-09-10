CC = gcc
CFLAGS = -Wall -std=gnu99 `pkg-config --cflags gtk+-3.0 jack alsa libconfig` -c
PROGRAM = gjackctl
OBJECTS = main.o server_switch.o toggle_rt.o drivers.o alsa_device_names.o \
		  dsp_load.o jack_server_init.o sample_rate.o gjackctl_settings.o \
		  connections.o rt_priority.o server_name.o display.o toggle_no_memlock.o \
          get_arg_vector.o config_file_input.o config_file_init.o toggle_midi.o\
          label_toggle_attr.o port_max.o timeout.o toggle_unlock_libs.o \
          toggle_no_zombies.o jack_log.o get_jack_gpid.o period.o \
		  clocksource.o frames.o create_jack_client.o
HEADERS = server_switch.h toggle_rt.h drivers.h alsa_device_names.h dsp_load.h \
		  jack_server_init.h sample_rate.h gjackctl_settings.h connections.h \
		  rt_priority.h server_name.h display.h toggle_no_memlock.h get_arg_vector.h \
          config_file_input.h config_file_init.h label_toggle_attr.h midi.h \
          port_max.h timeout.h get_jack_gpid.h jack_log.h toggle_unlock_libs.h \
          toggle_midi.h toggle_no_memlock.h toggle_no_zombies.h period.h \
		  clocksource.h frames.h create_jack_client.h
LIBS= `pkg-config --libs gtk+-3.0 alsa jack libconfig`

all: $(PROGRAM)
$(PROGRAM): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(PROGRAM)
main.o: main.c server_switch.h display.h config_file_init.h
	$(CC) $(CFLAGS) main.c 
server_switch.o: server_switch.c dsp_load.h jack_server_init.h \
					gjackctl_settings.h connections.h
	$(CC) $(CFLAGS) server_switch.c
toggle_rt.o: toggle_rt.c get_arg_vector.h config_file_input.h label_toggle_attr.h
	$(CC) $(CFLAGS) toggle_rt.c
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
gjackctl_settings.o: gjackctl_settings.c sample_rate.h toggle_rt.h rt_priority.h \
                     server_name.h toggle_no_memlock.h toggle_midi.h \
					 port_max.h timeout.h toggle_no_zombies.h toggle_unlock_libs.h
	$(CC) $(CFLAGS) gjackctl_settings.c
connections.o: connections.c
	$(CC) $(CFLAGS) connections.c
rt_priority.o: rt_priority.c jack_server_init.h get_arg_vector.h config_file_input.h
	$(CC) $(CFLAGS) rt_priority.c
server_name.o: server_name.c
	$(CC) $(CFLAGS) server_name.c
display.o: display.c
	$(CC) $(CFLAGS) display.c
toggle_no_memlock.o: toggle_no_memlock.c get_arg_vector.h config_file_input.h
	$(CC) $(CFLAGS) toggle_no_memlock.c
get_arg_vector.o: get_arg_vector.c
	$(CC) $(CFLAGS) get_arg_vector.c
config_file_input.o: config_file_input.c
	$(CC) $(CFLAGS) config_file_input.c
config_file_init.o: config_file_init.c
	$(CC) $(CFLAGS) config_file_init.c
label_toggle_attr.o: label_toggle_attr.c
	$(CC) $(CFLAGS) label_toggle_attr.c
toggle_midi.o: toggle_midi.c
	$(CC) $(CFLAGS) toggle_midi.c
port_max.o: port_max.c
	$(CC) $(CFLAGS) port_max.c
timeout.o: timeout.c config_file_input.h
	$(CC) $(CFLAGS) timeout.c
jack_log.o: jack_log.c
	$(CC) $(CFLAGS) jack_log.c
toggle_no_zombies.o: toggle_no_zombies.c config_file_input.h
	$(CC) $(CFLAGS) toggle_no_zombies.c
toggle_unlock_libs.o: toggle_unlock_libs.c config_file_input.h
	$(CC) $(CFLAGS) toggle_unlock_libs.c
get_jack_gpid.o: get_jack_gpid.c
	$(CC) $(CFLAGS) get_jack_gpid.c
clocksource.o: clocksource.c
	$(CC) $(CFLAGS) clocksource.c
frames.o: frames.c
	$(CC) $(CFLAGS) frames.c
period.o: period.c
	$(CC) $(CFLAGS) period.c
create_jack_client.o: create_jack_client.c
	$(CC) $(CFLAGS) create_jack_client.c

clean:
	rm -vf *~ *.o $(PROGRAM)
