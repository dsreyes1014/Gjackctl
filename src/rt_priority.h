#ifndef RT_PRIORITY_H
#define RT_PRIORITY_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "jack_server_init.h"

void
rt_priority (GtkWidget *grid);

void
value_cb (GtkSpinButton *spin_button, gpointer data);

#endif