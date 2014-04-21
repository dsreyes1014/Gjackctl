#define _POSIX_SOURCE /* <-- Need to add this for the kill () function. */
#include <gtk/gtk.h>
#include <stdio.h>

#include "server_switch.h"
#include "alsa_device_names.h"
#include "rt_box.h"
#include "drivers.h"


gchar driver_arg1[20];
gchar driver_arg2[20];

/* `switch_pos` will start JACK when switched on and terminate it when switched off. */
void
switch_pos_cb (GtkSwitch *sw, gpointer data)
{	
	gboolean check;
	gchar *jack_args[10];

	/* Declared with static or else `pid` will reset to 0 in the else command. */
	static GPid pid;
		
	check = gtk_switch_get_active (sw);
	jack_args[0] = "jackd";
	jack_args[1] = rt_arg ();
	jack_args[2] = "-P75";
	jack_args[3] = driver_arg1;
	jack_args[4] = driver_arg2;
	jack_args[5] = NULL;

	if (check == TRUE)
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Shutdown Server");

		g_spawn_async (NULL, jack_args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &pid, NULL);
	}
	else
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Start Server");
	
		kill (pid, SIGTERM);		
	}
}

void 
server_switch (GtkWidget *box)
{
	GtkWidget *jack_switch;	
	GtkWidget *vbox;
	GtkWidget *label;
	gboolean check;	
	
	jack_switch = gtk_switch_new ();
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	label = gtk_label_new ("JACK");
	check = gtk_switch_get_active (GTK_SWITCH (jack_switch));

	gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, TRUE, 6);
	gtk_box_pack_start (GTK_BOX (vbox), jack_switch, FALSE, TRUE, 0);
	drivers (vbox);
	gtk_box_pack_start (GTK_BOX (box), vbox, FALSE, FALSE, 2);

	/* Signal that works here for the `GtkSwitch` is `notify::active` which is a
		different convention used for other widget's signals that I've used. */
	g_signal_connect (jack_switch, "notify::active", G_CALLBACK (switch_pos_cb), jack_switch);

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


