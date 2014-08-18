#include "jack_server_init.h"

static gboolean
jackdrc_init_input ()
{
    config_t config;
    gchar *file;
    gchar *jackdrc;
    FILE *fp;
    const gchar *value;
    gboolean value2;

    config_init (&config);
    file = g_strconcat (g_getenv ("HOME"), "/.config/gjackctl/gjackctl.conf", NULL);
    jackdrc = g_strconcat (g_getenv ("HOME"), "/.jackdrc", NULL);
    fp = fopen (jackdrc, "w+");

    fprintf (fp, "/usr/bin/jackd");
    config_read_file (&config, file);
    
    /*********************************************************************/
    config_lookup_string (&config, "gjackctl.server.name", &value);

    if ((strlen (value) == 0) | (strncmp (value, "default", 7) == 0)) 
    {
    }
    else   
    {
        g_print ("From `jack_server_init.c`: %s\n", value);

        g_fprintf (fp, " -n%s", value);
    }
    /*********************************************************************/
    config_lookup_string (&config, "gjackctl.server.priority", &value);

    g_fprintf (fp, " -P%s", value);
    /*********************************************************************/
    config_lookup_string (&config, "gjackctl.server.port_max", &value);

    if (g_strcmp0 (value, "256") != 0)
    {
        g_fprintf (fp, " -p%s", value);    
    }
    /*********************************************************************/
    config_lookup_string (&config, "gjackctl.server.timeout", &value);

    if (g_strcmp0 (value, "500") != 0)
    {
        g_fprintf (fp, " -t%s", value);    
    }
    /*********************************************************************/
    config_lookup_string (&config, "gjackctl.server.clocksource", &value);

    if (g_strcmp0 (value, "System") != 0)
    {
        if (g_strcmp0 (value, "Cycle") == 0)
        {
            g_fprintf (fp, " -cc");    
        }
        if (g_strcmp0 (value, "Hpet") == 0)
        {
            g_fprintf (fp, " -ch");
        }
    }
    /*********************************************************************/
    config_lookup_bool (&config, "gjackctl.server.realtime", &value2);

    if (value2 == FALSE)
    {
        g_fprintf (fp, " -r");
    }
    /*********************************************************************/
    config_lookup_bool (&config, "gjackctl.server.unlock_libs", &value2);

    if (value2 == TRUE)
    {
        g_fprintf (fp, " -u");
    }
    /*********************************************************************/
    config_lookup_bool (&config, "gjackctl.server.no_zombies", &value2);

    if (value2 == TRUE)
    {
        g_fprintf (fp, " -Z");
    }
    /*********************************************************************/
    config_lookup_bool (&config, "gjackctl.server.no_memlock", &value2);

    if (value2 == TRUE)
    {
        g_fprintf (fp, " -m");
    }
    /*********************************************************************/
    config_lookup_bool (&config, "gjackctl.server.midi", &value2);

    if (value2 == TRUE)
    {
        g_fprintf (fp, " -Xalsa_midi");
    }
    /*********************************************************************/
    config_lookup_string (&config, "gjackctl.driver.type", &value);

    if ((g_strcmp0 (value, NULL) != 0) && (g_strcmp0 (value, "ALSA") == 0))
    {
        value = g_strconcat ("alsa", NULL);
        g_fprintf (fp, " -d%s", value);
    }
    else if (g_strcmp0 (value, NULL) != 0)
    {
        g_fprintf (fp," -d%s", value);
    }
    /*********************************************************************/
    config_lookup_string (&config, "gjackctl.driver.device", &value);

    if (g_strcmp0 (value, NULL) != 0)
    {
        g_fprintf (fp, " -dhw:%s", value);
    }    
    /*********************************************************************/
    config_lookup_string (&config, "gjackctl.driver.sample_rate", &value);

    if (g_strcmp0 (value, NULL) != 0)
    {
        g_fprintf (fp, " -r%s", value);
    }
    /*********************************************************************/
    config_lookup_string (&config, "gjackctl.driver.period", &value);

    if (g_strcmp0 (value, NULL) != 0)
    {
        g_fprintf (fp, " -n%s", value);
    }
    /*********************************************************************/

    fclose (fp);

    return TRUE;
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

	/*jack_client_t *client;
	jack_status_t status;
	gint check_pid;
	gchar **argvp;
	gboolean ret;

	argvp = get_arg_vector ();*/

    jackdrc_init_input ();
	/*(ret = g_spawn_async (NULL, argvp, 
						 NULL, G_SPAWN_SEARCH_PATH, 
						 NULL, NULL, 
						 &pid, NULL);*/

	/* Check for errors on server startup. */	
	/*if (ret == FALSE)
	{
		gtk_switch_set_active (GTK_SWITCH (sw), FALSE);
		g_print ("Couldn't start JACK server.\n");

        return -1;
	}*/

	/* We wait 2 seconds for server to initiate so we can create `gjackctl` 
	jack client. */
	// (2);

	/* `if/else` statement does a final check to see if `GPid pid` is actually
	running/initiated.  We use the `check_pid` statement for this. */
	//check_pid = kill (pid, 0);

	/*if (check_pid == 0)
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
	}*/

	return 0;
}
