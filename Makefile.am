AUTOMAKE_OPTIONS = foreign

# This creates m4 directory in source directory
ACLOCAL_AMFLAGS = -I m4 --install

# Adding 'NOTES' to m4 directory
EXTRA_DIST = \
	m4/NOTES

bin_PROGRAMS = gjackctl
gjackctl_SOURCES = \
    src/main.c \
    src/server_switch.c \
    src/server_switch.h \
    src/alsa_device_names.c \
    src/alsa_device_names.h \
    src/drivers.c \
    src/drivers.h \
    src/jack_server_init.c \
    src/jack_server_init.h \
    src/gjackctl_settings.c \
    src/gjackctl_settings.h \
    src/config_file_input.c \
    src/config_file_input.h \
    src/label_toggle_attr.c \
    src/label_toggle_attr.h \
    src/sample_rate.c \
    src/sample_rate.h \
    src/toggle_rt.c \
    src/toggle_rt.h \
    src/rt_priority.c \
    src/rt_priority.h \
    src/server_name.c \
    src/server_name.h \
    src/toggle_no_memlock.c \
    src/toggle_no_memlock.h \
    src/toggle_midi.c \
    src/toggle_midi.h \
    src/port_max.c \
    src/port_max.h \
    src/timeout.c \
    src/timeout.h \
    src/clocksource.c \
    src/clocksource.h \
    src/toggle_no_zombies.c \
    src/toggle_no_zombies.h \
    src/toggle_unlock_libs.c \
    src/toggle_unlock_libs.h \
    src/period.c \
    src/period.h \
    src/frames.c \
    src/frames.h \
    src/toggle_verbose.c \
    src/toggle_verbose.h \
    src/connections.c \
    src/connections.h \
    src/get_jack_gpid.c \
    src/get_jack_gpid.h \
    src/display.c \
    src/display.h \
    src/dsp_load.c \
    src/dsp_load.h \
    src/create_jack_client.c \
    src/create_jack_client.h \
    src/jack_ports.c \
    src/jack_ports.h \
    src/jack_log.c \
    src/jack_log.h \
    src/config_file_init.c \
    src/config_file_init.h 

gjackctl_CFLAGS = \
    $(GTK3_CFLAGS) \
    $(CLUTTER_CFLAGS) \
    $(CLUTTER_GTK_CFLAGS) \
    $(JACK1_CFLAGS) \
    $(LIBCONFIG_CFLAGS) \
    $(ALSA_CFLAGS)

gjackctl_LDADD = \
    $(GTK3_LIBS) \
    $(CLUTTER_LIBS) \
    $(CLUTTER_GTK_LIBS) \
    $(JACK1_LIBS) \
    $(LIBCONFIG_LIBS) \
    $(ALSA_LIBS)

desktopdir = $(datadir)/applications
desktop_DATA = src/gjackctl.desktop
