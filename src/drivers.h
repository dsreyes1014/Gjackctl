#ifndef DRIVERS_H
#define DRIVERS_H

#include <gtk/gtk.h>

#include "alsa_device_names.h"

extern GtkWidget *label_driver;

void
drivers (GtkWidget *grid);

void
driver_popover_cb (GtkWidget *button, gpointer data);

#endif
