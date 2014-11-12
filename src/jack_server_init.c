#include "jack_server_init.h"

typedef struct _GtkPassedServerData {
    GtkWidget *ptext_view;
    gchar pbuffer[8192];    
} GtkPassedServerData;

/*
    This struct is used to pass data 
    through to the 'void suprocess2_pipe_cb ()'
    function.
*/
typedef struct _GtkPassedServerData2 {
    gchar pbuffer2[2048];
    GtkWidget *psw;
    GtkWidget *pwindow;
} GtkPassedServerData2;

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
    config_lookup_bool (&config, "gjackctl.server.verbose", &value2);

    if (value2 == TRUE)
    {
        g_fprintf (fp, " -v");
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
err_msg_box (GtkWidget *window)
{
	GtkWidget *msg_dialog;

	msg_dialog = gtk_message_dialog_new (GTK_WINDOW (window), 
				                    	 GTK_DIALOG_MODAL, 
										 GTK_MESSAGE_WARNING,
										 GTK_BUTTONS_CLOSE,
										 "JACK Server Error");                                                      
	
	gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (msg_dialog),
						                      "Server could not be started!"); 

	gtk_dialog_run (GTK_DIALOG (msg_dialog));
	gtk_widget_destroy (msg_dialog);
}

static void
subprocess_pipe_cb (GObject *source, GAsyncResult *res, gpointer data)
{
    GtkTextBuffer *buffer;
    GtkPassedServerData *rdata;
    const gchar *string;
    gssize bytes;

    rdata = data;
    bytes = g_input_stream_read_finish (G_INPUT_STREAM (source),
                                        res,
                                        NULL);

    g_print ("'jack_server_init.c': number of bytes in log pipe %d\n", bytes);

    string = g_strndup (rdata -> pbuffer, bytes - 1);
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (rdata -> ptext_view));

    gtk_text_buffer_insert_at_cursor (buffer, string, -1);
}

static void
subprocess2_pipe_cb (GObject *source, GAsyncResult *res, gpointer data)
{
    gssize bytes;
    GInputStream *out2;
    const gchar *string;
    GtkPassedServerData2 *rdata2;

    rdata2 = data;

    bytes = g_input_stream_read_finish (G_INPUT_STREAM (source),
                                        res,
                                        NULL);

    string = g_strndup (rdata2 -> pbuffer2, bytes - 1);

    g_print ("'jack_server_init.c': number of bytes %d\n", bytes);

    g_print ("%s\n", string);

    if (g_strcmp0 (string, "running") == 0)
    {
        g_print ("success\n");
    }
    else if (g_strcmp0 (string, "not running") == 0)
    {
        g_print ("failure\n");

        err_msg_box (rdata2 -> pwindow);

        gtk_switch_set_active (GTK_SWITCH (rdata2 -> psw), FALSE);
    }
}

static void
subprocess_wait_cb (GObject *source, GAsyncResult *res, gpointer data)
{
    gboolean status;
    gboolean check;
    GError *error;

    check = g_subprocess_get_if_exited (G_SUBPROCESS (source));

    status = g_subprocess_wait_finish (G_SUBPROCESS (source), res, &error);

    if (check == TRUE)
    {
        g_print ("'jack_server_init.c': %d\n", g_subprocess_get_exit_status (G_SUBPROCESS (source)));
        g_print ("'jack_server_init.c': jackd terminated\n");
    }
}

gint 
jack_server_init (GtkWidget *sw, 
                  GtkWidget *text,
                  GtkWidget *window)
{
	/*
        This functions starts the JACK server using a
        `GSubprocess *subprocess`.  

        Argument info:
            --argument 1 'GtkWidget *sw' is declared in 'main.c'
            --argument 2 'GtkWidget *text' is declared in 'main.c'
            --argument 3 'GtkWidget *window' is declared in 'main.c'

            1. Is a 'GtkSwitch' widget that is set to be switched off 
            if an error was set starting the JACK server.

            2. Is a 'GtkTextView' widget that is filled with log information.

            3. Is a 'GtkWindow' widget that is used to help create 
            a popup window upon error.

        Returns '0' if successfully created a jack server or '-1' upon error.
        
    */

    GPid pid;
	gint check_pid;
	gchar **argv;
    GtkPassedServerData *pdata;
    GtkPassedServerData2 *pdata2; 
    GSubprocess *subprocess;
    GSubprocess *subprocess2;
    const gchar *pid_string;
    GInputStream *out;
    GInputStream *out2;

    pdata = g_slice_new (GtkPassedServerData);
    pdata -> ptext_view = text;
    pdata2 = g_slice_new (GtkPassedServerData2);
    pdata2 -> psw = sw;
    pdata2 -> pwindow = window;

    jackdrc_init_input ();
	argv = get_arg_vector ();

    subprocess = g_subprocess_newv ((const gchar *const *) argv,
                                    G_SUBPROCESS_FLAGS_STDOUT_PIPE |
                                    G_SUBPROCESS_FLAGS_STDERR_PIPE,
                                    NULL);

    g_subprocess_wait_async (subprocess,
                             NULL,
                             subprocess_wait_cb,
                             NULL);

    out = g_subprocess_get_stdout_pipe (subprocess);

    g_input_stream_read_async (out,
                               pdata -> pbuffer,
                               4096,
                               G_PRIORITY_DEFAULT,
                               NULL,
                               subprocess_pipe_cb,
                               pdata);    

    sleep (5);

    /* 
        Check here to see if 'jackd' didn't stop 
        after starting it with 'GSubprocess *subprocess' 
        using 'GSubprocess *subprocess2'.
    */
    subprocess2 = g_subprocess_new (G_SUBPROCESS_FLAGS_STDOUT_PIPE |
                                    G_SUBPROCESS_FLAGS_STDERR_PIPE,
                                    NULL,
                                    "/usr/bin/jack_wait",
                                    "-c",
                                    NULL);

    out2 = g_subprocess_get_stdout_pipe (subprocess2);

    g_input_stream_read_async (out2,
                               pdata2 -> pbuffer2,
                               2048,
                               G_PRIORITY_DEFAULT,
                               NULL,
                               subprocess2_pipe_cb,
                               pdata2);

    pid_string = g_subprocess_get_identifier (subprocess);

    pid = get_jack_gpid (pid_string);

    g_print ("'jack_server_init.c': GPid %d\n", pid);

    if (pid == -2)
    {
        return -1;
    }

	return 0;
}
