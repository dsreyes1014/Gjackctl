#ifndef RADIO_RT_H
#define RADIO_RT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "jack_server_init.h"

GtkWidget *label_dsp;
GtkWidget *checkbox;
gchar *rt_arg;

void
rt_box (GtkWidget *box);

void
real_time (GtkToggleButton *radio, gpointer data);

#endif
