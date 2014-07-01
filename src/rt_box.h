#ifndef RADIO_RT_H
#define RADIO_RT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>

#include "jack_server_init.h"
#include "get_arg_vector.h"
#include "config_file_input.h"

void
rt_box (GtkWidget *grid);

#endif
