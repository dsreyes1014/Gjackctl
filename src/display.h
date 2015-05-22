#ifndef DISPLAY_H
#define DISPLAY_H

#include <gtk/gtk.h>
#include <libconfig.h>

#include "dsp_load.h"
#include "create_jack_client.h"
#include "jack_ports.h"
#include "main.h"

void
display (GtkPassedMainData *pdata);

#endif
