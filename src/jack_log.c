#include "jack_log.h"

void
jack_log (GtkWidget *stack,
          GtkWidget *text)
{
    GtkWidget *box;
    GtkWidget *scwindow;
    GdkRGBA bg_color;

    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    scwindow = gtk_scrolled_window_new (NULL, NULL);
    bg_color.red = 0.0;
	bg_color.green = 0.0;
	bg_color.blue = 0.0;
	bg_color.alpha = 0.5;

    //gtk_widget_override_background_color (text, GTK_STATE_FLAG_NORMAL, &bg_color);

    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scwindow), 
									GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_container_add (GTK_CONTAINER (scwindow), text);
    gtk_box_pack_start (GTK_BOX (box), scwindow, TRUE, TRUE, 2);

    gtk_stack_add_titled (GTK_STACK (stack), box, "log", "Log");
}
