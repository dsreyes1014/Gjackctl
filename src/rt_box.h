#ifndef RADIO_RT_H
#define RADIO_RT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "jack_server_init.h"

extern GtkWidget *checkbox;

void
rt_box (GtkWidget *grid);

void
real_time (GtkToggleButton *radio, gpointer data);

#endif
