#include "server_name.h"

void
server_name (GtkWidget *grid)
{
	GtkWidget *label_name;
	GtkWidget *entry;
 
	label_name = gtk_label_new ("Server Name");
	entry = gtk_entry_new ();	
	
	gtk_grid_attach (GTK_GRID (grid), label_name, 1, 7, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), entry, label_name, GTK_POS_RIGHT, 1, 1);
}
