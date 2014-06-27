#ifndef DRIVERS_H
#define DRIVERS_H

#include <gtk/gtk.h>

#include "alsa_device_names.h"

void
drivers (GtkWidget *grid, GtkApplication *app);

void
driver_popover_cb (GtkWidget *button, gpointer user_data);

#endif
