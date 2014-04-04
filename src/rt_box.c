#include <gtk/gtk.h>

#include "rt_box.h"
#include "server_switch.h"

GtkWidget *checkbox;

void
rt_box (GtkWidget *box)
{
	GtkWidget *vbox;
	GtkWidget *label;

	checkbox = gtk_check_button_new ();
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);	
	label = gtk_label_new ("Realtime");

	gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (checkbox), TRUE);
	gtk_widget_set_tooltip_text (checkbox, "Enable/Disable Realtime Audio");

	gtk_container_add (GTK_CONTAINER (vbox), label);
	gtk_container_add (GTK_CONTAINER (vbox), checkbox);
	gtk_container_add (GTK_CONTAINER (box), vbox);

	g_signal_connect (checkbox, "toggled", G_CALLBACK (real_time), NULL);	
}

gboolean
real_time (GtkToggleButton *button, gpointer data)
{
	gboolean check;
	
	check = gtk_toggle_button_get_active (button);

	if (check == TRUE)
	{
		return check;
	}
	else
	{
		return check;
	}
}


