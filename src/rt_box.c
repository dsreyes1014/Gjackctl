#include "rt_box.h"

GtkWidget *checkbox;

void
real_time (GtkToggleButton *button, gpointer data)
{
	gboolean check;
	
	check = gtk_toggle_button_get_active (button);

	/* `gchar *jack_start[]` declared in `jack_server_init.c`. */
	if (check == TRUE)
	{
		gtk_widget_set_tooltip_text (checkbox, "Disable Realtime Audio");
		jack_start[1] = "-R";
	}
	else
	{	
		gtk_widget_set_tooltip_text (checkbox, "Enable Realtime Audio");
		jack_start[1] = "-r";
	}
}

void
rt_box (GtkWidget *grid)
{
	GtkWidget *label;
	gboolean check;

	/* `GtkWidget checkbox` is declared globally. */
	checkbox = gtk_check_button_new ();
	label = gtk_label_new ("Realtime");
	check = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbox));	

	/* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
	gtk_grid_attach (GTK_GRID (grid), checkbox, 0, 0, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), label, 
								checkbox, GTK_POS_RIGHT, 1, 1);
	
	g_signal_connect (checkbox, "toggled", G_CALLBACK (real_time), NULL);	

	/* Initiate tooltip for `checkbox` in the if/else statement. It won't 
	show when app first starts if we don't.  `gchar *jack_start[]` is 
	declared in `jack_server_init.c` which is a NULL terminated array used 
	to spawn the `jackd` server and obtain the process id for signal handling.*/
	if (check == TRUE)
	{
		/*  */
		gtk_widget_set_tooltip_text (checkbox, "Disable Realtime Audio");	
		jack_start[1] = "-R";
	}
	else
	{	
		gtk_widget_set_tooltip_text (checkbox, "Enable Realtime Audio");
		jack_start[1] = "-r";
	}

	/* Set `GtkWidget checkbox` to active to enable the `--realtime` 
	argument for `jackd` as default. */
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbox), TRUE);	
}
