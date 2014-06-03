#include "display.h"

GtkWidget *label_dsp;

void
display (GtkWidget *grid)
{
	GtkWidget *layout;
	GtkWidget *label;
	GtkWidget *sc_window;
	GtkWidget *display_box;

	layout = gtk_layout_new (NULL, NULL);
	label = gtk_label_new ("CPU Load");
	sc_window = gtk_scrolled_window_new (NULL, NULL);
	display_box = gtk_image_new_from_file ("../img/display-#2.png");
	label_dsp = gtk_label_new ("--");

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sc_window), 
									GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	/* Pack `grid`. */
	gtk_layout_put (GTK_LAYOUT (layout), display_box, 0, 0);
	gtk_layout_put (GTK_LAYOUT (layout), label, 10, 16);
	gtk_layout_put (GTK_LAYOUT (layout), label_dsp, 80, 16);
	gtk_container_add (GTK_CONTAINER (sc_window), layout);
	gtk_grid_attach (GTK_GRID (grid), sc_window, 2, 0, 86, 11);
}
