#ifndef SERVER_SWITCH_H
#define SERVER_SWITCH_H

/* Needed for `popen ()` function. */ 
//#define POSIX_C_SOURCE >= 2

#include <sys/types.h>
#include <signal.h>
#include <jack/jack.h>
#include <gtk/gtk.h>
#include <glib.h>

#include "drivers.h"
#include "jack_server_init.h"
//#include "jack_log.h"
#include "get_jack_gpid.h"
#include "main.h"

void
server_switch (GtkPassedMainData *pdata);

#endif
