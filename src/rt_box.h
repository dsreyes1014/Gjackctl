#ifndef RADIO_RT_H
#define RADIO_RT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>

#include "jack_server_init.h"

void
rt_box (GtkWidget *grid);

void
real_time_cb (GtkToggleButton *button, gpointer data);

void
rt_arg_create (GtkToggleButton *button, gchar *arg);

void
file_input (gchar **string, gint argc);

#endif
