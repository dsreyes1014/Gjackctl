#ifndef DSP_LOAD_C
#define DSP_LOAD_C

#define POSIX_SOURCE

#include <gtk/gtk.h>
#include <jack/jack.h>
#include <sys/types.h>
#include <signal.h>
#include <jack/jack.h>

#include "rt_box.h"

gboolean
dsp_load (jack_client_t *client);

void
dsp_init ();

#endif
