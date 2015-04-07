#ifndef GJACKCTL_SETTINGS_H
#define GJACKCTL_SETTINGS_H

#include <gtk/gtk.h>

#include "sample_rate.h"
#include "toggle_rt.h"
#include "rt_priority.h"
#include "server_name.h"
#include "toggle_no_memlock.h"
#include "toggle_midi.h"
#include "port_max.h"
#include "timeout.h"
#include "clocksource.h"
#include "toggle_no_zombies.h"
#include "toggle_unlock_libs.h"
#include "period.h"
#include "frames.h"
#include "toggle_verbose.h"
#include "alsa_device_names.h"
    

void
gjackctl_settings (GtkWidget *window,
                   GtkApplication *app,
                   GtkWidget *header_bar);

#endif
