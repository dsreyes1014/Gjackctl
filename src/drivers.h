#ifndef DRIVERS_H
#define DRIVERS_H

#include <gtk/gtk.h>

#include "alsa_device_names.h"

GtkWidget *label_driver;

void
drivers (GtkWidget *box);

void
activate_popover_cb (GtkWidget *button, gpointer data);

#endif
