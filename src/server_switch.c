#include "server_switch.h"

GtkWidget *jack_switch;

void
switch_pos_cb (GtkSwitch *sw, jack_client_t *client)
{	
	/* `switch_pos_cb` will start JACK when switched on and terminate it when switched off. */

	gboolean check;

	check = gtk_switch_get_active (sw);	

	if (check == TRUE)
	{						
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Shutdown Server");
		jack_server_init ();
		g_print ("PID from switch cb: %d\n", pid);
	}
	else
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Start Server");
		kill (pid, SIGTERM);	
		g_spawn_close_pid (pid);	
	}
}

void 
server_switch (GtkWidget *grid, GtkApplication *app)
{	
	GtkWidget *grid_space1;
	gchar result[10];
	gboolean check;	
	FILE *cmd;

	pid = -2;
	cmd = popen ("pgrep jackd", "r");
	jack_switch = gtk_switch_new ();
	grid_space1 = gtk_label_new ("");
	check = gtk_switch_get_active (GTK_SWITCH (jack_switch));

	while (fgets (result, sizeof (result), cmd) != NULL)
	{
		g_print ("%s\n", result);
		pid = g_ascii_strtoll (result, NULL, 10);
		g_print ("%d\n", pid);
	}
	
	dsp_init ();
	
	/* Widget positions inside of `grid`. */
	gtk_widget_set_valign (jack_switch, GTK_ALIGN_START);
	gtk_widget_set_valign (grid_space1, GTK_ALIGN_END);

	/* Pack `grid` which is declared in `main.c`. */
	gtk_grid_attach (GTK_GRID (grid), grid_space1, 0, 0, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), jack_switch, 1, 1, 1, 1);	
	gjackctl_settings (grid, app);
	//connections (vbox);
	
	g_signal_connect (jack_switch, "notify::active", G_CALLBACK (switch_pos_cb), NULL);
	
	/* Initiate tooltip for `jack_switch` here or else it won't show when app first starts. */
	if (check == TRUE)
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (jack_switch) , "Shutdown Server");
	}
	else
	{
		gtk_widget_set_tooltip_text (GTK_WIDGET (jack_switch) , "Start Server");	
	}
}


