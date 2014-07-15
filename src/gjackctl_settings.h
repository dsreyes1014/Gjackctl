#ifndef GJACKCTL_SETTINGS_H
#define GJACKCTL_SETTINGS_H

#include <gtk/gtk.h>

#include "sample_rate.h"
#include "rt_box.h"
#include "rt_priority.h"
#include "server_name.h"
#include "no_memlock.h"
#include "midi.h"

void
gjackctl_settings (GtkWidget *window,
                   GtkApplication *app,
                   GtkWidget *header_bar);

#endif
