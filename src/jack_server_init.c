#include "jack_server_init.h"

void
file_input (gchar **string, gint argc)
{
	gchar file[128];	
	gint i;
	FILE *jackdrc;
	
	g_sprintf (file, "%s/.jackdrc", g_getenv ("HOME"));

	jackdrc = g_fopen (file, "w+");

	for (i = 0; i < argc; i++)
	{
		g_fprintf (jackdrc, "%s ", string[i]);
	}
		
	fclose (jackdrc);
}

gchar **
get_arg_vector ()
{
	gchar cmd[128];
	gchar *contents;
	gchar **argvp;
	gint argcp;
	gsize size;

	/* Create path to file `.jackdrc` using `g_sprintf ()`. */
	g_sprintf (cmd, "%s/.jackdrc", g_getenv ("HOME"));

	/* Check if file path exists. */
	if (g_file_test (cmd, G_FILE_TEST_EXISTS) == FALSE)
	{
		g_print ("File doesn't exist.  Create file.");

		return NULL;
	}

	g_file_get_contents (cmd, &contents, &size, NULL);
	g_shell_parse_argv (contents, &argcp, &argvp, NULL);

	g_free (contents);

	return argvp;
}

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

	/* Pack `dialog_grid`. */
	gtk_grid_attach (GTK_GRID (dialog_grid), icon, 0, 0, 1, 1);
	gtk_grid_attach (GTK_GRID (dialog_grid), warning, 1, 0, 1, 1);
	gtk_container_add (GTK_CONTAINER (content_area), dialog_grid);
	
	g_signal_connect (dialog, "response", 
					  G_CALLBACK (gtk_widget_destroy), NULL);

	gtk_widget_show_all (dialog);
}

gint 
jack_server_init (GtkSwitch *sw, GPid pid)
{
	/* Starts the JACK server using `g_spawn_async ()` with the
	 `GPid pid` as an out. */

	jack_client_t *client;
	jack_status_t status;
	gint check_pid;
	gint argc;
	gsize size;
	const gchar *home;
	gchar cmd[128];
	gchar *contents;
	gchar **argvp;
	gboolean ret;
	gboolean check_contents;

	home = g_getenv ("HOME");

	/* Here we create the path using `g_sprintf ()` and place it into
	`gchar cmd[]`.  We use this for `g_file_get_contents ()` and place the 
	contents into `gchar **argvp` for `g_spawn_async ()`. */
	g_sprintf (cmd, "%s/.jackdrc", home);
	check_contents = g_file_get_contents (cmd, &contents, &size, NULL);

	if (check_contents == FALSE)
	{
		/* Error if file doesn't exist. */
		g_print ("File does not exist");
	}
	else
	{
		g_shell_parse_argv (contents, &argc, &argvp, NULL);
	}

	ret = g_spawn_async (NULL, argvp, 
						 NULL, G_SPAWN_SEARCH_PATH, 
						 NULL, NULL, 
						 &pid, NULL);

	g_free (argvp);
	g_free (contents);

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
	running/initiated.  We use the `check_pid` statement for this. */
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

	return 0;
}
