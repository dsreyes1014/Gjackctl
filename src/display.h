#ifndef DISPLAY_H
#define DISPLAY_H

#include <gtk/gtk.h>
#include <libconfig.h>

#include "dsp_load.h"
#include "create_jack_client.h"

void
display (GtkWidget *stack, GtkWidget *sw);

#endif
