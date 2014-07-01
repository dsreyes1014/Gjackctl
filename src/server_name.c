#include "server_name.h"

void
server_name (GtkWidget *grid)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */

	GtkWidget *label_name;
	GtkWidget *entry;
 
	label_name = gtk_label_new ("Server Name");
	entry = gtk_entry_new ();	
	
	gtk_grid_attach (GTK_GRID (grid), label_name, 0, 0, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), entry, label_name, GTK_POS_RIGHT, 1, 1);

	gtk_widget_set_margin_start (label_name, 10);
	gtk_widget_set_margin_top (entry, 10);
	gtk_widget_set_margin_top (label_name, 5);
}
