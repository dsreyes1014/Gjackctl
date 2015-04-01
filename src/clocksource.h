#ifndef CLOCKSOURCE_H
#define CLOCKSOURCE_H

#include <gtk/gtk.h>
#include <libconfig.h>

#include "config_file_input.h"
#include "label_toggle_attr.h"

void
clocksource (GtkWidget *box, GtkWidget *button);

const gchar *
get_clocksource ();

#endif
