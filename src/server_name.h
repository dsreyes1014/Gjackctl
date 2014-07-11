#ifndef SERVER_NAME_H
#define SERVER_NAME_H

#include <gtk/gtk.h>
#include <libconfig.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "gjackctl_settings.h"
#include "config_file_input.h"

void
server_name (GtkWidget *grid, GtkWidget *button);

#endif
