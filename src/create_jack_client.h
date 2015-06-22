#ifndef CREATE_JACK_CLIENT_H
#define CREATE_JACK_CLIENT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <jack/jack.h>
#include <stdlib.h>
#include <string.h>

#include "jack_ports.h"
#include "dsp_load.h"
#include "main.h"

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
    gpointer client;
    GtkListStore *list_store;
    GtkListStore *list_store2;
    GtkWidget *pwindow;
    GtkWidget *view;
    GtkWidget *view2;
    GtkWidget *sc_window;
    GtkWidget *sc_window2;
    GtkWidget *audio_from_button;
    GtkWidget *audio_to_button;
    GtkWidget *midi_from_button;
    GtkWidget *midi_to_button;
    gint col_num;
    GtkWidget *from_audio;  /* for a GtkLabel */
    GtkWidget *to_audio;    /* for a GtkLabel */
    GtkWidget *from_midi;   /* for a GtkLabel */
    GtkWidget *to_midi;     /* for a GtkLabel */
    enum GtkJackPortType port_type;
    GSimpleAction *audio_action;
    GSimpleAction *midi_action;
    GSimpleActionGroup *group;
    GtkWidget *button;
    gint handler_id;
};

gint
jack_client_init (jack_client_t *client,
                  GtkWidget     *sw,
                  GtkWidget     *button_box,
                  GtkWidget     *label,
                  GtkWidget     *label2,
                  GtkWidget     *level_bar,
                  GtkWidget     *view);

#endif
