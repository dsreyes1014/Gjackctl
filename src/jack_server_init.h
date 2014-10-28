#ifndef JACK_SERVER_INIT_H
#define JACK_SERVER_INIT_H 

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "server_switch.h"
#include "dsp_load.h"

gint 
jack_server_init (GtkWidget *sw, GtkWidget *text, GtkWidget *window);

#endif
