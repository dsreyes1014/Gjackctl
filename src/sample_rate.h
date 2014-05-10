#ifndef SAMPLE_RATE_H
#define SAMPLE_RATE_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "jack_server_init.h"

extern GtkWidget *button_rate;
extern GtkWidget *popover;

void
sample_rate (GtkWidget *grid);

void
sample_rate_popover_cb (GtkWidget *button, gpointer data);

void
rates_cb (GtkToggleButton *tb, gpointer user_data);

#endif
