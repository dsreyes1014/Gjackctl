#ifndef ALSA_DEVICE_NAMES_H
#define ALSA_DEVICE_NAMES_H

void
alsa_device_names ();

void
print_alsa_driver_activate (GSimpleAction *action,
							GVariant *parameter,
							gpointer data);

#endif
