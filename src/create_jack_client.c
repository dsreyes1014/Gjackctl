#include "create_jack_client.h"

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
jack_shutdown_cb (gpointer user_data)
{
    GtkPassedJackPortsData *rdata;

    rdata = user_data;

    gtk_widget_destroy (rdata -> button);
    gtk_widget_destroy (rdata -> pwindow);
    /*gtk_widget_destroy (rdata -> view);
    gtk_widget_destroy (rdata -> view2);
    gtk_widget_destroy (rdata -> from_audio);
    gtk_widget_destroy (rdata -> to_audio);
    gtk_widget_destroy (rdata -> from_midi);
    gtk_widget_destroy (rdata -> to_midi);
    gtk_widget_destroy (rdata -> sc_window);
    gtk_widget_destroy (rdata -> sc_window2);*/

    g_slice_free (GtkPassedJackPortsData, rdata);
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
jack_client_init (GtkWidget *sw,
                  GtkWidget *button_box,
                  GtkWidget *label,
                  GtkWidget *label2,
                  GtkWidget *level_bar)
{
    /*
     * This function creates a client for the JACK server
     * to provide some info of the jackd server.
     *
     *  Argument info:
     *      --argument 1 'GtkWidget *sw' is declared in 'main.c'
     *      --argument 2 'GtkWidget *label' is declared in 'display.c'
     *      --argument 3 'GtkWidget *label2' is declared in 'display.c'
     *      --argument 4 'GtkWidget *level_bar' is declared in 'display.c'
     */

    jack_status_t status;
    gchar srate[10];
    GtkPassedJackPortsData *pdata;

    pdata = g_slice_new (GtkPassedJackPortsData);
    sleep (1);
    pdata -> client = NULL;
    pdata -> client = jack_client_open ("gjackctl",
                                        JackNoStartServer |
                                        JackUseExactName,
                                        &status);

    if (pdata -> client == NULL)
    {
        return -1;
    }

    jack_set_process_callback (pdata -> client, process_cb, NULL);
    jack_on_shutdown (pdata -> client, jack_shutdown_cb, pdata);
    jack_set_sample_rate_callback (pdata -> client, srate_cb, label2);


    //jack_activate (pdata -> client);

    jack_ports (button_box, pdata);

    /* Function updates label to show cpu load. */
    dsp_load (sw, label, pdata -> client, level_bar);

    g_sprintf (srate, "%d", jack_get_sample_rate (pdata -> client));
    gtk_label_set_text (GTK_LABEL (label2), g_strdup (srate));

    return 0;
}
