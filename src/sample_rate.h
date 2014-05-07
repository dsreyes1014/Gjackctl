#ifndef SAMPLE_RATE_H
#define SAMPLE_RATE_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "jack_server_init.h"

GtkWidget *button_rate;
GtkWidget *popover;

void
sample_rate (GtkWidget *box);

void
sample_rate_popover_cb (GtkWidget *button, gpointer data);

void
rates_cb (GtkToggleButton *tb, gpointer user_data);

#endif
