#ifndef SAMPLE_RATE_H
#define SAMPLE_RATE_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "jack_server_init.h"

extern GtkWidget *srate_button;
extern GtkWidget *popover;

void
sample_rate (GtkWidget *grid);

#endif
