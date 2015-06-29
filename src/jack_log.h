#ifndef JACK_LOG_H
#define JACK_LOG_H

#include <gtk/gtk.h>

#include "main.h"

void
timestamp_newline (GtkTextBuffer *buffer);

void
jack_log (GtkPassedMainData *pdata);

#endif
