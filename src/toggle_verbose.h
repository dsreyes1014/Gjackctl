#ifndef TOGGLE_VERBOSE_H
#define TOGGLE_VERBOSE_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>

#include "jack_server_init.h"
#include "config_file_input.h"
#include "label_toggle_attr.h"

void
toggle_verbose (GtkWidget *box, GtkWidget *button);

#endif
