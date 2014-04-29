#ifndef ALSA_DEVICE_NAMES_H
#define ALSA_DEVICE_NAMES_H

#include <stdio.h>
#include <gtk/gtk.h>
#include <alsa/asoundlib.h>

#include "drivers.h"
#include "server_switch.h"

void
alsa_device_names ();

void
print_alsa_driver_activate (GSimpleAction *action,
							GVariant *parameter,
							gpointer data);

#endif
