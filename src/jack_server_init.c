#include "jack_server_init.h"

//GPid pid;
gchar *jack_start[20];

void
err_msg_box ()
{
	GtkWidget *dialog;
	GtkWidget *content_area;
	GtkWidget *icon;
	GtkWidget *warning;
	GtkWidget *dialog_grid;

	dialog = gtk_dialog_new_with_buttons ("JACK Warning", NULL,
										  GTK_DIALOG_MODAL, 
										  "Close",
										  GTK_RESPONSE_CLOSE,
										  NULL);

	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	icon = gtk_image_new_from_file ("../img/dialog-warning.png");
	warning = gtk_label_new ("JACK server could not be started");
	dialog_grid = gtk_grid_new ();

	gtk_grid_attach (GTK_GRID (dialog_grid), icon, 0, 0, 1, 1);
	gtk_grid_attach (GTK_GRID (dialog_grid), warning, 1, 0, 1, 1);
	gtk_container_add (GTK_CONTAINER (content_area), dialog_grid);
	
	g_signal_connect (dialog, "response", 
					  G_CALLBACK (gtk_widget_destroy), NULL);

	gtk_widget_show_all (dialog);
}

gint 
jack_server_init (GtkSwitch *sw)
{
	jack_client_t *client;
	jack_status_t status;
	gint check_pid;
	GError *err;
	gboolean ret;
	GPid pid;

	jack_start[0] = "jackd";
	jack_start[20] = NULL;

	ret = g_spawn_async (NULL, jack_start, 
						 NULL, G_SPAWN_SEARCH_PATH, 
						 NULL, NULL, 
						 &pid, &err);

	/* Check for errors on server startup. */	
	if (ret == FALSE)
	{
		gtk_switch_set_active (GTK_SWITCH (sw), FALSE);
		g_print ("Couldn't start JACK server.\n");
	}

	/* We wait 2 seconds for server to initiate so we can create `gjackctl` 
	jack client. */
	sleep (2);

	/* `if/else` statement does a final check to see if `GPid pid` is actually
	running.  We use the `check_pid` statement for this. */
	check_pid = kill (pid, 0);

	if (check_pid == 0)
	{
		client = jack_client_open ("gjackctl", JackServerName,
									 &status, "default");
		jack_activate (client);
		g_timeout_add_seconds (2, (GSourceFunc) dsp_load, (gpointer) client);
	}
	else
	{
		gtk_switch_set_active (GTK_SWITCH (sw), FALSE);
		err_msg_box ();
	}

	//g_print ("%s\n", jack_start[2]);

	return 0;
}
