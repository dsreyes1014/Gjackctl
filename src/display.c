#include "display.h"

GtkWidget *label_dsp;

void
display (GtkWidget *stack, GtkWidget *window)
{
	GtkWidget *layout;
	GtkWidget *label;
	GtkWidget *sc_window;
	GdkRGBA bg_color;
    const gchar *stack_name;	

	layout = gtk_layout_new (NULL, NULL);
	label = gtk_label_new ("CPU Load");
	sc_window = gtk_scrolled_window_new (NULL, NULL);
	label_dsp = gtk_label_new ("--");
	bg_color.red = 0.0;
	bg_color.green = 0.0;
	bg_color.blue = 0.0;
	bg_color.alpha = 1.0;

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sc_window), 
									GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_override_background_color (layout, GTK_STATE_FLAG_NORMAL, &bg_color);

	/* Pack `header_bar` & `sc_window`. */
	gtk_layout_put (GTK_LAYOUT (layout), label, 10, 16);
	gtk_layout_put (GTK_LAYOUT (layout), label_dsp, 80, 16);
	gtk_container_add (GTK_CONTAINER (sc_window), layout);
	gtk_stack_add_titled (GTK_STACK (stack), sc_window, "display", "Display");
}
