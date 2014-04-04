#include <stdio.h>

#ifndef SERVER_SWITCH_H
#define SERVER_SWITCH_H

void 
server_switch (GtkWidget *box);

gboolean
switch_pos (GtkSwitch *sw, gpointer data);

gchar 
*rt_status();

#endif
