#ifndef JACK_SERVER_INIT_H
#define JACK_SERVER_INIT_H 

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "server_switch.h"
#include "dsp_load.h"

void 
jack_server_init (GtkSwitch *sw, GtkWidget *text, GPid pid);

#endif
