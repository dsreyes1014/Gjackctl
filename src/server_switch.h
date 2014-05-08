#ifndef SERVER_SWITCH_H
#define SERVER_SWITCH_H

/* Needed for `popen ()` function. */ 
#define POSIX_C_SOURCE >= 2

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <jack/jack.h>
#include <gtk/gtk.h>

#include "rt_box.h"
#include "drivers.h"
#include "dsp_load.h"
#include "jack_server_init.h"
#include "gjackctl_settings.h"
#include "connections.h"

extern GtkWidget *jack_switch;

void 
server_switch (GtkWidget *box, GtkApplication *app);

void
switch_pos_cb (GtkSwitch *sw, jack_client_t *client);

#endif
