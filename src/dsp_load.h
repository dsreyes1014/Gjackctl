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

gboolean
dsp_load (gpointer user_data);

void
dsp_init (GtkWidget *sw, GPid pid);

#endif
