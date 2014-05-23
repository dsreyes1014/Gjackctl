#ifndef JACK_SERVER_INIT_H
#define JACK_SERVER_INIT_H 

#include <gtk/gtk.h>

#include "server_switch.h"
#include "rt_box.h"
#include "dsp_load.h"

//extern GPid pid;
extern gchar *jack_start[];

gint 
jack_server_init (GtkSwitch *sw);

void
child_watch (GPid pid, gint status, gpointer data);

void
err_msg_box ();

#endif
