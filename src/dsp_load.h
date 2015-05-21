#ifndef DSP_LOAD_C
#define DSP_LOAD_C

#define POSIX_SOURCE

#include <gtk/gtk.h>
#include <jack/jack.h>
#include <sys/types.h>
#include <signal.h>
#include <jack/jack.h>

#include "display.h"
#include "jack_server_init.h"

void
dsp_load (GtkWidget     *sw,
          GtkWidget     *label,
          jack_client_t *client,
          GtkWidget     *level_bar);

#endif
