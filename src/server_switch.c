#include "server_switch.h"

gint handler_id;

static void
clear_log_view (GtkWidget *text)
{
    GtkTextBuffer *buffer;
    GtkTextIter start;
    GtkTextIter end;

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text));

    gtk_text_buffer_get_start_iter (buffer, &start);
    gtk_text_buffer_get_end_iter (buffer, &end);

    gtk_text_buffer_delete (buffer, &start, &end);
}

static void
switch_pos_cb (GtkSwitch *sw, GParamSpec *pspec, gpointer user_data)
{	
    GtkPassedMainData *rdata;
    jack_status_t status;
    gint res;

	rdata = user_data;

	if (gtk_switch_get_active (sw) == TRUE)
	{
        //clear_log_view (rdata -> text);
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Shutdown Server");

        /*
         * We have to start the server first so our app can make itself a
         * client.
         */
	    jack_server_init (rdata);

        /*
         * We make our app a jack client if server was not running else we
         * just activate ourselves after making ourselves a client in 'main.c'.
         */
        if (rdata -> client == NULL)
        {
            rdata -> client = jack_client_open ("gjackctl", JackNoStartServer, &status);
        }
        else
        {
            res = jack_activate (rdata -> client);
            g_print ("client activate result: %d\n", res);
        }
	}
}

void
server_switch (GtkPassedMainData *pdata)
{
	gint check_pid;
	gboolean check;

	check = gtk_switch_get_active (GTK_SWITCH (pdata -> sw));



	/* Check to see if jackd server is already started. */
	check_pid = kill (get_jack_gpid (NULL), 0);

	if (check_pid == 0)
	{
        gtk_switch_set_active (GTK_SWITCH (pdata -> sw), TRUE);
	}

	gtk_header_bar_pack_start (GTK_HEADER_BAR (pdata -> header_bar),
                               pdata -> sw);

	gtk_widget_set_valign (pdata -> sw, GTK_ALIGN_CENTER);

	if (gtk_switch_get_active (GTK_SWITCH (pdata -> sw)) == TRUE)
	{
		gtk_widget_set_tooltip_text (GTK_WIDGET (pdata -> sw),
                                     "Shutdown Server");
	}
	else
	{
		gtk_widget_set_tooltip_text (GTK_WIDGET (pdata -> sw),
                                     "Start Server");
	}

    g_signal_connect (pdata -> sw,
                      "notify::active",
                      G_CALLBACK (switch_pos_cb),
                      pdata);
}
