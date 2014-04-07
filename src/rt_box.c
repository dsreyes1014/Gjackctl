#include <gtk/gtk.h>

#include "rt_box.h"

GtkWidget *checkbox;

gboolean
real_time (GtkToggleButton *button, gpointer data)
{
	gboolean check;
	
	check = gtk_toggle_button_get_active (button);

	if (check == TRUE)
	{
		gtk_widget_set_tooltip_text (checkbox, "Disable Realtime Audio");		

		return check;
	}
	else
	{	
		gtk_widget_set_tooltip_text (checkbox, "Enable Realtime Audio");

		return check;
	}
}

void
rt_box (GtkWidget *box)
{
	GtkWidget *vbox;
	GtkWidget *label;
	gboolean check;

	checkbox = gtk_check_button_new ();
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);	
	label = gtk_label_new ("Realtime");
	check = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbox));
	gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (checkbox), TRUE);	

	gtk_container_add (GTK_CONTAINER (vbox), label);
	gtk_container_add (GTK_CONTAINER (vbox), checkbox);
	gtk_container_add (GTK_CONTAINER (box), vbox);

	g_signal_connect (checkbox, "toggled", G_CALLBACK (real_time), NULL);	

	if (check == TRUE)
	{
		gtk_widget_set_tooltip_text (checkbox, "Disable Realtime Audio");		
	}
	else
	{	
		gtk_widget_set_tooltip_text (checkbox, "Enable Realtime Audio");
	}
}




