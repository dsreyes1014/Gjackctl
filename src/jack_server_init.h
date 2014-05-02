#ifndef JACK_SERVER_INIT_H
#define JACK_SERVER_INIT_H 

#include <gtk/gtk.h>

#include "server_switch.h"

#define COMMAND "jackd"

GPid pid;

gint 
jack_server_init ();

void
child_watch (GPid pid, gint status, gpointer data);

#endif
