#ifndef JACK_PORTS_H
#define JACK_PORTS_H

#include <gtk/gtk.h>
#include <jack/jack.h>
#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>

gint 
jack_ports (GtkWidget *stack, jack_client_t *client);

#endif

