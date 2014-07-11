#ifndef CONFIG_FILE_INPUT_H
#define CONFIG_FILE_INPUT_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>
#include <libconfig.h>

void
config_file_input (gchar **argv, gint argc);

gint 
config_file_input1 (const gchar *config_path, 
                    gint type, 
                    gpointer user_data);

#endif
