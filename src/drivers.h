#ifndef DRIVERS_H
#define DRIVERS_H

#include <gtk/gtk.h>

#include "alsa_device_names.h"

extern GtkWidget *label_driver;

void
drivers (GtkWidget *box);

void
driver_popover_cb (GtkWidget *button, gpointer data);

#endif
