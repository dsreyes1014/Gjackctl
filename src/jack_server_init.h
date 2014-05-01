#ifndef JACK_SERVER_INIT_H
#define JACK_SERVER_INIT_H 

#include <gtk/gtk.h>

typedef struct _jack_arg
{
	gchar *rt;
	gchar driver;
	gchar device;
} jack_arg;

GPid pid;

gint 
jack_server_init ();

#endif
