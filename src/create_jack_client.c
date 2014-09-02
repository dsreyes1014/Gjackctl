#include "create_jack_client.h"

typedef struct _jack_port jack_port;

static gint
process_cb (jack_nframes_t nframes, gpointer user_data)
{
    /*jack_default_audio_sample_t *in;
    jack_default_audio_sample_t *out;

    jack_port *ports;
    ports = user_data;

    in = jack_port_get_buffer (ports -> in_port, nframes);
    out = jack_port_get_buffer (ports -> out_port, nframes);

    memcpy (out, in, sizeof (jack_default_audio_sample_t) * nframes);

    return 0;*/
}

static void
jack_shutdown_cb (gpointer user_data)
{
    //exit (1);
}

static gint
alloc_port (const gchar *string)
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
}

gint
jack_client_init (GtkWidget *sw, GtkWidget *label, GtkWidget *label2)
{
    jack_client_t *client;
    jack_status_t status;
    //_port *ports;
    gchar srate[10];
    const gchar **playback;
    gint i;

    sleep (1);
    
    client = jack_client_open ("gjackctl", 
                               JackNoStartServer | JackUseExactName,
                               &status);
    i = 0;

    //ports = g_slice_new (jack_port);
    //ports -> in_port = jack_port_register (client, "capture", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    //ports -> out_port = jack_port_register (client, "playback", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);    

    jack_set_process_callback (client, process_cb, NULL);
    jack_on_shutdown (client, jack_shutdown_cb, NULL);
    jack_set_sample_rate_callback (client, srate_cb, label2);  

    playback = jack_get_ports (client, NULL, NULL, JackPortIsPhysical | JackPortIsInput);

    while (playback[i])
    {
        g_print ("create_jack_client.c: %s\n", playback[i]);

        alloc_port (playback[i]);
        i++;
    }

    jack_activate (client); 

    /* function updates label declared in display.c that shows jack cpu load. */
    dsp_init (sw, label, client); 

    g_sprintf (srate, "%d", jack_get_sample_rate (client));
    gtk_label_set_text (GTK_LABEL (label2), g_strdup (srate));        

    return 0;
}
