#ifndef TOGGLE_NO_ZOMBIES_H
#define TOGGLE_NO_ZOMBIES_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>

#include "jack_server_init.h"
//#include "get_arg_vector.h"
#include "config_file_input.h"
#include "label_toggle_attr.h"

void
toggle_no_zombies (GtkWidget *box, GtkWidget *button);

gboolean
get_no_zombies ();

#endif
