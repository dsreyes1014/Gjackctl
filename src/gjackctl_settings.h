#ifndef GJACKCTL_SETTINGS_H
#define GJACKCTL_SETTINGS_H

#include <gtk/gtk.h>

#include "sample_rate.h"
#include "rt_box.h"
#include "rt_priority.h"
#include "server_name.h"

/* Declared in `main.c`. */
extern GtkWidget *window;

void
gjackctl_settings (GtkWidget *grid, GtkApplication *app);

void
gjackctl_settings_cb (GtkWidget *widget, gpointer user_data);

void
popup_destroy_cb (GtkWidget *widget, gpointer user_data);

void
popup_clicked_ok_cb (GtkButton *button, gpointer user_data);

#endif
