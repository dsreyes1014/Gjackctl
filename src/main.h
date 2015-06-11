#ifndef MAIN_H
#define MAIN_H

#include <config.h>
#include <gtk/gtk.h>
#include <jack/jack.h>

typedef struct _GtkPassedMainData GtkPassedMainData;

#include "jack_log.h"
#include "config_file_init.h"
#include "server_switch.h"
#include "gjackctl_settings.h"
#include "display.h"

struct _GtkPassedMainData {
    GtkWidget *window;
    GtkWidget *text;
    GtkWidget *sw;
    GtkWidget *button_box;
    GtkWidget *vbox;
    GtkWidget *sswitcher;
    GtkWidget *header_bar;
};

gint
main (gint argc, gchar *argv[]);

#endif
