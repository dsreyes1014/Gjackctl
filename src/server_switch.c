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
		//dsp_init ();
	}
	else
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Start Server");
		kill (pid, SIGTERM);		
	}
}

void
dsp_init ()
{
	/* Signal that's recommended here for the `GtkSwitch` is `notify::active` which is a
	different convention used for other widget's signals that I've used. Also
	using the `kill ()` function to check if JACK server is running and `GPid pid` exists. 
	`GPid pid` is declared in `jack_server_init.h`. */

	jack_client_t *client;
	jack_status_t status;
	gint check_pid;

	check_pid = kill (pid, 0);
	
	if (check_pid == 0)
	{
		client = jack_client_open ("gjackctl", JackServerName, &status, "default");	
		gtk_switch_set_active (GTK_SWITCH (jack_switch), TRUE);
		jack_activate (client);
		g_timeout_add (1000, (GSourceFunc) dsp_load, (gpointer) client);
	}
}

void 
server_switch (GtkWidget *box_main)
{	
	GtkWidget *vbox;
	GtkWidget *label;
	gchar result[10];
	gboolean check;	
	FILE *cmd;

	pid = -2;
	cmd = popen ("pgrep jackd", "r");
	jack_switch = gtk_switch_new ();
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	label = gtk_label_new ("JACK");
	check = gtk_switch_get_active (GTK_SWITCH (jack_switch));		

	while (fgets (result, sizeof (result), cmd) != NULL)
	{
		g_print ("%s\n", result);
		pid = g_ascii_strtoll (result, NULL, 10);
		g_print ("%d\n", pid);
	}
	
	dsp_init ();

	gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, TRUE, 6);
	gtk_box_pack_start (GTK_BOX (vbox), jack_switch, FALSE, TRUE, 0);
	drivers (vbox);
	gtk_box_pack_start (GTK_BOX (box_main), vbox, FALSE, TRUE, 2);
	rt_box (box_main);
	
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


