#ifndef JACK_PORTS_H
#define JACK_PORTS_H

#include <gtk/gtk.h>
#include <string.h>
#include <jack/jack.h>

#include "create_jack_client.h"

gint
jack_ports (GtkWidget     *stack,
            gpointer       user_data);

#endif

