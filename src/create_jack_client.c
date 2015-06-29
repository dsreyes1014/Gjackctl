#include "create_jack_client.h"

GtkWidget *text_view;

static gint
process_cb (jack_nframes_t nframes, gpointer user_data)
{
    /*jack_port *playback;   

    jack_default_audio_sample_t *in;
    jack_default_audio_sample_t *out;

    //jack_port *ports;
    //ports = user_data;

    in = jack_port_get_buffer (ports -> in_port, nframes);
    out = jack_port_get_buffer (ports -> out_port, nframes);

    memcpy (out, in, sizeof (jack_default_audio_sample_t) * nframes);*/

    return 0;
}

static void
switch_pos_cb (GtkSwitch *sw, GParamSpec *pspec, gpointer user_data)
{
    GtkPassedJackPortsData *pdata;
    gboolean active;

    pdata = user_data;
    active = gtk_switch_get_active (sw);

    if (active == FALSE)
    {
        gtk_widget_destroy (pdata -> button);
        if (pdata -> pwindow != NULL)
        {
            gtk_widget_destroy (pdata -> pwindow);
        }

        g_slice_free (GtkPassedJackPortsData, pdata);
        g_signal_handler_disconnect (sw, pdata -> handler_id);
    }
}

static void
jack_shutdown_cb (gpointer user_data)
{

}

static gint
srate_cb (jack_nframes_t nframes, gpointer user_data)
{
    GtkWidget *label2;
    gchar srate[10];

    label2 = user_data;

    g_sprintf (srate, "%d", nframes);
    gtk_label_set_text (GTK_LABEL (label2), g_strdup (srate));

    return 0;
}

gint
jack_client_init (jack_client_t          *client,
                  GtkWidget              *sw,
                  GtkWidget              *button_box,
                  GtkWidget              *label,
                  GtkWidget              *label2,
                  GtkWidget              *level_bar)
{
    /*
     * This function creates a client for the JACK server
     * to provide some info from the jackd server.
     *
     *  Argument info:
     *      --argument 1 'client' is declared in 'main.h'
     *      --argument 2 'sw' is declared in 'main.h'
     *      --argument 3 'button_box' is declared in 'main.h'
     *      --argument 4 'label' is declared in 'display.c'
     *      --argument 5 'label2' is declared in 'display.c'
     *      --argument 6 'level_bar' is declared in 'display.c'
     */

    jack_status_t status;
    gchar srate[10];
    GtkPassedJackPortsData *pdata;

    pdata = g_slice_new (GtkPassedJackPortsData);
    /*client = jack_client_open ("gjackctl",
                               JackNoStartServer |
                               JackUseExactName,
                               &status);*/

    pdata -> client = client;
    pdata -> pwindow = NULL;

    if (pdata -> client == NULL)
    {
        g_print ("client creation failed in main\n");
        return -1;
    }

    jack_set_process_callback (pdata -> client, process_cb, NULL);
    jack_on_shutdown (pdata -> client, jack_shutdown_cb, pdata);
    jack_set_sample_rate_callback (pdata -> client, srate_cb, label2);

    /* We activate in 'jack_ports.c' to set up our 'check_connect_cb'. */
    //jack_activate (pdata -> client);

    jack_ports (button_box, pdata);

    //sleep (1);

    /* Function updates label to show cpu load. */
    dsp_load (sw, label, pdata -> client, level_bar);

    g_sprintf (srate, "%d", jack_get_sample_rate (pdata -> client));
    gtk_label_set_text (GTK_LABEL (label2), g_strdup (srate));

    pdata -> handler_id = g_signal_connect_after (sw,
                                                  "notify::active",
                                                  G_CALLBACK (switch_pos_cb),
                                                  pdata);

    return 0;
}
