#include "display.h"

void
display (GtkWidget *grid)
{
	GtkWidget *layout;
	GtkWidget *label;
	GtkWidget *sc_window;
	/* `GdkRGBA color` is a struct with 
	`gdouble variables red`, `green`, `blue`, and `alpha'. */  	
	GdkRGBA color;

	layout = gtk_layout_new (NULL, NULL);
	label = gtk_label_new ("CPU Load");
	sc_window = gtk_scrolled_window_new (NULL, NULL);
	color.red = 0.0;
	color.green = 0.0;
	color.blue = 0.0;
	color.alpha = 1.0;

	gtk_widget_override_background_color (layout, GTK_STATE_FLAG_NORMAL, &color);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sc_window), 
									GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);	

	/* Pack `grid`. */
	gtk_layout_put (GTK_LAYOUT (layout), label, 10, 1);
	gtk_container_add (GTK_CONTAINER (sc_window), layout);
	gtk_grid_attach (GTK_GRID (grid), sc_window, 2, 0, 80, 8);
}
