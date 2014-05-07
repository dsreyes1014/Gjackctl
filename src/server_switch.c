#include "server_switch.h"

void
jack_settings_cb (GtkWidget *button, gpointer data)
{
	GtkWidget *popover;
	GtkWidget *box;

	popover = gtk_popover_new (button);
	box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);

	rt_box (box);

	gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_RIGHT);

	gtk_container_add (GTK_CONTAINER (popover), box);

	gtk_widget_show_all (popover);
}

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
server_switch (GtkWidget *box_main)
{	
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *jack_button;
	gchar result[10];
	gboolean check;	
	FILE *cmd;

	pid = -2;
	cmd = popen ("pgrep jackd", "r");
	jack_switch = gtk_switch_new ();
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	label = gtk_label_new ("JACK");
	check = gtk_switch_get_active (GTK_SWITCH (jack_switch));
	jack_button = gtk_button_new_with_mnemonic ("_Settings");

	while (fgets (result, sizeof (result), cmd) != NULL)
	{
		g_print ("%s\n", result);
		pid = g_ascii_strtoll (result, NULL, 10);
		g_print ("%d\n", pid);
	}
	
	dsp_init ();

	gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, TRUE, 6);
	gtk_box_pack_start (GTK_BOX (vbox), jack_switch, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), jack_button, FALSE, TRUE, 2);
	drivers (vbox);
	sample_rate (vbox);
	gtk_box_pack_start (GTK_BOX (box_main), vbox, FALSE, TRUE, 2);
	//rt_box (box_main);
	
	g_signal_connect (jack_switch, "notify::active", G_CALLBACK (switch_pos_cb), NULL);
	g_signal_connect (jack_button, "clicked", G_CALLBACK (jack_settings_cb), NULL);
	
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


