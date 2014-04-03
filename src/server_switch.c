#include <gtk/gtk.h>

#include "server_switch.h"

void 
server_switch (GtkWidget *box)
{
	GtkWidget *jack_switch;
	GtkWidget *vbox;	

	jack_switch = gtk_switch_new ();
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);

	gtk_container_add (GTK_CONTAINER (vbox), jack_switch);
	gtk_container_add (GTK_CONTAINER (box), vbox);
}
