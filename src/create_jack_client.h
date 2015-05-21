#ifndef CREATE_JACK_CLIENT_H
#define CREATE_JACK_CLIENT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <jack/jack.h>
#include <stdlib.h>
#include <string.h>

#include "jack_ports.h"
#include "dsp_load.h"

/*
 * Declaring struct here to be used in 'jack_ports.c' so that we clean up when
 * we're done using it.
 */
typedef struct _GtkPassedJackPortsData GtkPassedJackPortsData;

enum GtkJackPortType {
    GTK_JACK_PORT_AUDIO_FROM,
    GTK_JACK_PORT_AUDIO_TO,
    GTK_JACK_PORT_MIDI_FROM,
    GTK_JACK_PORT_MIDI_TO
};

struct _GtkPassedJackPortsData {
    jack_client_t *client;
    GtkListStore *list_store;
    GtkListStore *list_store2;
    GtkWidget *view;
    GtkWidget *view2;
    gint col_num;
    GtkWidget *from_audio;  /* GtkLabel */
    GtkWidget *to_audio;    /* GtkLabel */
    GtkWidget *from_midi;   /* GtkLabel */
    GtkWidget *to_midi;     /* GtkLabel */
    enum GtkJackPortType port_type;
    GSimpleAction *audio_action;
	GSimpleAction *midi_action;
    GSimpleActionGroup *group;
};

gint
jack_client_init (GtkWidget *sw,
                  GtkWidget *stack,
                  GtkWidget *label,
                  GtkWidget *label2,
                  GtkWidget *level_bar);

#endif
