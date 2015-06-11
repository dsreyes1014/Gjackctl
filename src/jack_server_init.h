#ifndef JACK_SERVER_INIT_H
#define JACK_SERVER_INIT_H 

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "dsp_load.h"
#include "get_jack_gpid.h"
#include "main.h"

gint
jack_server_init (GtkPassedMainData *pdata);

#endif
