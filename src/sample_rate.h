#ifndef SAMPLE_RATE_H
#define SAMPLE_RATE_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "jack_server_init.h"
//#include "get_arg_vector.h"
#include "config_file_input.h"

//extern GtkWidget *srate_button;
//extern GtkWidget *popover;

void
sample_rate (GtkWidget *box, GtkWidget *button);

const gchar *
get_sample_rate ();

#endif
