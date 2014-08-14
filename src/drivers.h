#ifndef DRIVERS_H
#define DRIVERS_H

#include <gtk/gtk.h>

#include "alsa_device_names.h"
#include "get_arg_vector.h"
#include "config_file_input.h"

typedef struct _GtkPassedDriverData {
    GtkWidget *pbutton;
    GtkWidget *label;
} GtkPassedDriverData;

void
drivers (GtkWidget *box, GtkApplication *app, GtkWidget *button);

#endif
