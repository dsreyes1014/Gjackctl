#ifndef JACK_SERVER_INIT_H
#define JACK_SERVER_INIT_H

#include <gtk/gtk.h>

#define CMD jackd

typedef struct _jack_arg
{
	gchar rt;
	gchar no_rt;
	gchar server_name;
	gchar load;
	gchar timeout;
	gchar 
} jack_arg;

gint 
jack_server_init ();

#endif
