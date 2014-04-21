#include <gtk/gtk.h>

#include "rt_box.h"

GtkWidget *checkbox;

/* `rt_status` checks the status of `checkbox` and returns a gchar used for a jackd argument. */
gchar *
rt_arg ()
{
	gchar *arg;	

	gboolean check;

	check = real_time (GTK_TOGGLE_BUTTON (checkbox), NULL);

	if (check == TRUE)
	{
		arg = "-R";

		return arg;
	}
	else
	{
		arg = "-r";

		return arg;
	}
}

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

	gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 6);
	gtk_box_pack_start (GTK_BOX (vbox), checkbox, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (box), vbox, FALSE, FALSE, 4);

	g_signal_connect (checkbox, "toggled", G_CALLBACK (real_time), NULL);	

	/* Initiate tooltip for `checkbox` here or else it won't show when app first starts. */
	if (check == TRUE)
	{
		gtk_widget_set_tooltip_text (checkbox, "Disable Realtime Audio");		
	}
	else
	{	
		gtk_widget_set_tooltip_text (checkbox, "Enable Realtime Audio");
	}
}




