#include "jack_server_init.h"

typedef struct _GtkPassedServerData {
    GtkWidget *text;
    //GtkWidget *progress;
    //gint timeout_id;
} GtkPassedServerData;

static gchar **
get_arg_vector ()
{
	gchar *jackdrc;
	gchar **argvp;
	gchar *contents;
	gint argcp;
	gsize size;

	argcp = 0;
    jackdrc = g_strconcat (g_getenv ("HOME"), "/.jackdrc", NULL);

	/* Check if file path exists. */
	if (g_file_test (jackdrc, G_FILE_TEST_EXISTS) == FALSE)
	{
		g_print ("File doesn't exist.  Create file.");

		return NULL;
	}

	/* Get contents of `.jackdrc` and add it to to `gchar **argvp`. */
	g_file_get_contents (jackdrc, &contents, &size, NULL);
	g_shell_parse_argv (contents, &argcp, &argvp, NULL);

	g_free (contents);

	return argvp;
}

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
    config_lookup_string (&config, "gjackctl.driver.frames", &value);

    if (g_strcmp0 (value, NULL) != 0)
    {
        g_fprintf (fp, " -p%s", value);
    }
    /*********************************************************************/
    config_lookup_string (&config, "gjackctl.driver.period", &value);

    if (g_strcmp0 (value, NULL) != 0)
    {
        g_fprintf (fp, " -n%s", value);
    }
    /*********************************************************************/

    fclose (fp);
    config_destroy (&config);
    g_free (jackdrc);
    g_free (file);

    return TRUE;
}

static void
err_msg_box ()
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

static gboolean
out_watch_cb (GIOChannel *channel,
              GIOCondition cond,
              gpointer user_data)
{
    gchar *log;
    gchar *string;
    gsize  size;
    GtkPassedServerData *rdata;
    GtkTextBuffer *buffer;

    rdata = user_data;
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (rdata -> text));

    if (cond == G_IO_HUP)
    {
        g_io_channel_unref (channel);
        return FALSE;
    }

    g_io_channel_read_line (channel, &log, &size, NULL, NULL);
    string = g_strdup (log);

    gtk_text_buffer_insert_at_cursor (buffer, string, -1);
    g_free (string);
    g_free (log);

    return TRUE; 
}

static gboolean
err_watch_cb (GIOChannel *channel,
              GIOCondition cond,
              gpointer user_data)
{
    gchar *log;
    gchar *string;
    gsize  size;
    GtkPassedServerData *rdata;
    GtkTextBuffer *buffer;
    GtkTextTag *tag;
    GtkTextIter start;
    GtkTextIter end;

    rdata = user_data;
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (rdata -> text));
    tag = gtk_text_buffer_create_tag (buffer, NULL, "foreground", "red", NULL);
    
    if (cond == G_IO_HUP)
    {
        g_io_channel_unref (channel);
        return FALSE;
    }

    g_io_channel_read_line (channel, &log, &size, NULL, NULL);
    string = g_strdup (log);    

    gtk_text_buffer_insert_at_cursor (buffer, string, -1);

    gtk_text_buffer_get_start_iter (buffer, &start);
    gtk_text_buffer_get_end_iter (buffer, &end);
    gtk_text_buffer_apply_tag (buffer, tag, &start, &end);
    
    g_free (string);
    g_free (log);

    return TRUE; 
}

/*static gboolean
timeout_cb (gpointer user_data)
{
    GtkPassedServerData *rdata;

    rdata = user_data;
    
    gtk_progress_bar_pulse (GTK_PROGRESS_BAR (rdata -> progress));

    return( TRUE );
}*/

static void
child_watch_cb (GPid pid, gint status, gpointer user_data)
{
    g_spawn_close_pid (pid);
}

gint 
jack_server_init (GtkWidget *sw, 
                  GtkWidget *text)
{
	/* Starts the JACK server using `g_spawn_async ()` with the
	 `GPid pid` as an out. */
    //GtkWidget *progress;
    GPid pid;
	jack_client_t *client;
	jack_status_t status;
	gint check_pid;
    gint in;
    gint out;
    gint err;
	gchar **argv;
	gboolean ret;
    GIOChannel *ch_out;
    GIOChannel *ch_err;
    GtkPassedServerData *pdata;

    pdata = g_slice_new (GtkPassedServerData);
    pdata -> text = text;

    jackdrc_init_input ();
	argv = get_arg_vector ();
  
	ret = g_spawn_async_with_pipes (NULL, 
                                    argv, 
						            NULL,
                                    G_SPAWN_DO_NOT_REAP_CHILD, 
						            NULL,
                                    NULL, 
						            &pid,
                                    NULL,
                                    &out,
                                    &err,
                                    NULL);

	/* Check for errors on server startup. */	
	if (ret == FALSE)
	{
		gtk_switch_set_active (GTK_SWITCH (sw), FALSE);
		g_print ("Couldn't start JACK server.\n");
        err_msg_box ();

        return -1;
	}

    //pdata -> timeout_id = g_timeout_add (1500, (GSourceFunc) timeout_cb, pdata);

    g_child_watch_add (pid, (GChildWatchFunc) child_watch_cb, pdata);

#ifdef G_OS_WIN32
    ch_out = g_io_channel_win32_new_fd (out);
    ch_err = g_io_channel_win32_new_fd (err);
#else
    ch_out = g_io_channel_unix_new (out);
    ch_err = g_io_channel_unix_new (err);
#endif

    if (gtk_switch_get_active (GTK_SWITCH (sw)) == TRUE)
    {
        g_io_add_watch (ch_out,
                        G_IO_IN | G_IO_ERR | 
                        G_IO_OUT | G_IO_PRI | 
                        G_IO_HUP | G_IO_NVAL, 
                        (GIOFunc) out_watch_cb,
                        pdata);

        g_io_add_watch (ch_err,
                        G_IO_ERR | G_IO_HUP,
                        (GIOFunc) err_watch_cb,
                        pdata);
    }
    else
    {
        g_io_add_watch (ch_out, 
                        G_IO_IN | G_IO_HUP,
                        (GIOFunc) out_watch_cb,
                        pdata);

        g_io_add_watch (ch_err,
                        G_IO_ERR | G_IO_HUP,
                        (GIOFunc) err_watch_cb,
                        pdata);
    }

	return 0;
}
