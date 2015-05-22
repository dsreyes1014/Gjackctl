#ifndef JACK_SERVER_INIT_H
#define JACK_SERVER_INIT_H 

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "dsp_load.h"
#include "get_jack_gpid.h"

gint 
jack_server_init (GtkWidget *sw, GtkWidget *text, GtkWidget *window);

#endif
