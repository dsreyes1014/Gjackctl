#ifndef JACK_SERVER_INIT_H
#define JACK_SERVER_INIT_H 

#include <gtk/gtk.h>

#include "server_switch.h"
#include "rt_box.h"

extern GPid pid;
extern gchar *jack_start[];

gint 
jack_server_init ();

void
child_watch (GPid pid, gint status, gpointer data);

#endif
