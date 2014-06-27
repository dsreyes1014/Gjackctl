#ifndef RADIO_RT_H
#define RADIO_RT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>

#include "jack_server_init.h"

void
rt_box (GtkWidget *grid, gint pos_x);

#endif
