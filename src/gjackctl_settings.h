#ifndef GJACKCTL_SETTINGS_H
#define GJACKCTL_SETTINGS_H

#include <gtk/gtk.h>

#include "sample_rate.h"
#include "rt_box.h"
#include "rt_priority.h"
#include "server_name.h"

typedef struct 
{
	GtkWidget *data1;
	GtkApplication *data2;
}pass_data;

typedef struct
{
	GtkWidget *data1;
	GtkWidget *data2;
}pass_data_2;

void
gjackctl_settings (GtkWidget *grid, GtkWidget *window, GtkApplication *app);

#endif
