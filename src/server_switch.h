#include <stdio.h>

#ifndef SERVER_SWITCH_H
#define SERVER_SWITCH_H

void 
server_switch (GtkWidget *box);

void
switch_pos_cb (GtkSwitch *sw, gpointer data);

gchar 
*rt_arg ();

#endif
