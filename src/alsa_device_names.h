#ifndef ALSA_DEVICE_NAMES_H
#define ALSA_DEVICE_NAMES_H

#include <gtk/gtk.h>
#include <alsa/asoundlib.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "drivers.h"
#include "server_switch.h"
#include "jack_server_init.h"

GMenu *submenu;

void
alsa_device_names ();

void
print_alsa_driver_activate_cb (GSimpleAction *action,
							GVariant *parameter,
							gpointer data);

#endif
