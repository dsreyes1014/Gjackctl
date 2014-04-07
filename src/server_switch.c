#define _POSIX_SOURCE /* <-- Need to add this for the kill () function. */
#include <gtk/gtk.h>
#include <stdio.h>

#include "server_switch.h"
#include "rt_box.h"
#include "drivers.h"

extern GtkWidget *checkbox;

/* 'switch_pos' will start JACK when switched on and terminate it when switched off. */
gboolean
switch_pos (GtkSwitch *sw, gpointer data)
{	
	gboolean check;
	gboolean spawn;
	gchar *jack_args[10];
	static GPid pid;
		
	check = gtk_switch_get_active (sw);
	jack_args[0] = "jackd";
	jack_args[1] = rt_status();
	jack_args[2] = "-P75";
	jack_args[3] = "-dalsa";
	jack_args[4] = NULL;

	if (check == TRUE)
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Shutdown Server");

		spawn = g_spawn_async (NULL, jack_args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &pid, NULL);

		return spawn;
	}
	else
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Start Server");
	
		kill (pid, SIGTERM);	
		
		spawn = FALSE;

		return spawn;	
	}
}

/* 'rt_status' checks the status of checkbox and returns the realtime argument for jackd. */
gchar
*rt_status ()
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

	gtk_container_add (GTK_CONTAINER (vbox), label);
	gtk_container_add (GTK_CONTAINER (vbox), jack_switch);
	drivers (vbox);
	gtk_container_add (GTK_CONTAINER (box), vbox);

	g_signal_connect (jack_switch, "notify::active", G_CALLBACK (switch_pos), jack_switch);

	/* Starts a tooltip for the switch here because it doesn't show when starting app */
	if (check == TRUE)
	{
		gtk_widget_set_tooltip_text (GTK_WIDGET (jack_switch) , "Shutdown Server");
	}
	else
	{
		gtk_widget_set_tooltip_text (GTK_WIDGET (jack_switch) , "Start Server");	
	}
}


