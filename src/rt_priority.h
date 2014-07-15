#ifndef RT_PRIORITY_H
#define RT_PRIORITY_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "jack_server_init.h"
#include "get_arg_vector.h"
#include "config_file_input.h"

void
rt_priority (GtkWidget *grid, GtkWidget *button);

#endif
