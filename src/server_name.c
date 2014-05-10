#include "server_name.h"

void
server_name (GtkWidget *grid)
{
	GtkWidget *label_name;
	GtkWidget *entry;
	GtkWidget *separator;
	GtkWidget *space;
	gint i;
 
	space = gtk_label_new (" ");
	label_name = gtk_label_new ("Server Name");
	entry = gtk_entry_new ();
	separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);

	for (i = 0; i <= 30; i++)
	{
		gtk_grid_attach (GTK_GRID (grid), space, i, 0, 1, 1);
		space = gtk_label_new (" ");
	}
	
	gtk_grid_attach (GTK_GRID (grid), label_name, 7, 0, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), entry, label_name, GTK_POS_RIGHT, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), separator, label_name, GTK_POS_BOTTOM, 2, 1);
}
