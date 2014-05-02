#include "rt_box.h"

GtkWidget *checkbox;
GtkWidget *label_dsp;

extern gchar *jack_init[];

void
real_time (GtkToggleButton *button, gpointer data)
{
	gboolean check;
	
	check = gtk_toggle_button_get_active (button);

	if (check == TRUE)
	{
		gtk_widget_set_tooltip_text (checkbox, "Disable Realtime Audio");
		jack_init[1] = "-R";
		g_print ("%s\n", jack_init[1]);
	}
	else
	{	
		gtk_widget_set_tooltip_text (checkbox, "Enable Realtime Audio");
		jack_init[1] = "-r";
		g_print ("%s\n", jack_init[1]);
	}
}

void
rt_box (GtkWidget *box)
{
	GtkWidget *vbox;
	GtkWidget *vbox2;
	GtkWidget *separator;
	GtkWidget *label;
	gboolean check;

	checkbox = gtk_check_button_new ();
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	vbox2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	separator = gtk_separator_new (GTK_ORIENTATION_VERTICAL);	
	label = gtk_label_new ("RT");
	label_dsp = gtk_label_new ("DSP");
	check = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbox));
		
	gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (checkbox), TRUE);	

	gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 6);
	gtk_box_pack_start (GTK_BOX (vbox), checkbox, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox2), label_dsp, FALSE, TRUE, 6);
	gtk_box_pack_start (GTK_BOX (box), vbox, FALSE, FALSE, 4);
	gtk_box_pack_start (GTK_BOX (box), separator, FALSE, FALSE, 4);
	gtk_box_pack_start (GTK_BOX (box), vbox2, FALSE, FALSE, 4);
	
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
