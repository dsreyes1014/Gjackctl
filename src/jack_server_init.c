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

	g_strfreev (string);
}

gchar **
get_arg_vector ()
{
	gchar cmd[128];
	gchar **argvp;
	gchar *contents;
	gint argcp;
	gsize size;

	argcp = 0;

	/* Create path to file `.jackdrc` using `g_sprintf ()`. */
	g_sprintf (cmd, "%s/.jackdrc", g_getenv ("HOME"));

	/* Check if file path exists. */
	if (g_file_test (cmd, G_FILE_TEST_EXISTS) == FALSE)
	{
		g_print ("File doesn't exist.  Create file.");

		return NULL;
	}

	/* Get contents of `.jackdrc` and add it to to `gchar **argvp`. */
	g_file_get_contents (cmd, &contents, &size, NULL);
	g_shell_parse_argv (contents, &argcp, &argvp, NULL);

	g_free (contents);

	/* reallocate argvp to provide enough room for adding arguments to the 
	vector. */
	argvp = g_realloc (argvp, (argcp + 8) * sizeof *argvp);

	return argvp;
}

static void
err_msg_box (GtkWidget *window)
{
	GtkWidget *msg_dialog;

	msg_dialog = gtk_message_dialog_new (NULL, 
										 GTK_DIALOG_MODAL, 
										 GTK_MESSAGE_WARNING,
										 GTK_BUTTONS_CLOSE,
										 "JACK Server Error");
	
	gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (msg_dialog),
											  "Server could not be started!"); 

	gtk_dialog_run (GTK_DIALOG (msg_dialog));
	gtk_widget_destroy (msg_dialog);
}

gint 
jack_server_init (GtkSwitch *sw, GPid pid, GtkWidget *window)
{
	/* Starts the JACK server using `g_spawn_async ()` with the
	 `GPid pid` as an out. */

	jack_client_t *client;
	jack_status_t status;
	gint check_pid;
	gchar **argvp;
	gboolean ret;

	argvp = get_arg_vector ();
	ret = g_spawn_async (NULL, argvp, 
						 NULL, G_SPAWN_SEARCH_PATH, 
						 NULL, NULL, 
						 &pid, NULL);

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
		err_msg_box (window);
	}

	return 0;
}
