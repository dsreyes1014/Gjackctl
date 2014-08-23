#ifndef DISPLAY_H
#define DISPLAY_H

#include <gtk/gtk.h>
#include <libconfig.h>

#include "dsp_load.h"

extern GtkWidget *label_dsp;

void
display (GtkWidget *stack, GtkWidget *sw);

#endif
