#ifndef JACK_SERVER_INIT_H
#define JACK_SERVER_INIT_H 

#include <gtk/gtk.h>

#include "server_switch.h"
#include "rt_box.h"
#include "dsp_load.h"

gint 
jack_server_init (GtkSwitch *sw, GPid pid, GtkWidget *window);

void
file_input (gchar **string, gint argc);

gchar **
get_arg_vector ();

void
child_watch_cb (GPid pid, gint status, gpointer user_data);

#endif
