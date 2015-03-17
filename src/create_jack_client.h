#ifndef CREATE_JACK_CLIENT_H
#define CREATE_JACK_CLIENT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <jack/jack.h>
#include <stdlib.h>
#include <string.h>

#include "jack_ports.h"

gint
jack_client_init (GtkWidget *sw,
                  GtkWidget *stack,
                  GtkWidget *label,
                  GtkWidget *label2,
                  GtkWidget *level_bar);

#endif
