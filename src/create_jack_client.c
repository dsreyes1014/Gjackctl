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
    //exit (1);
    //jack_client_t *client;

    //client = user_data;

    //jack_deactivate (client);
    //jack_client_close (client);   
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
jack_client_init (GtkWidget *sw, GtkWidget *label, GtkWidget *label2)
{
    /*  
        This function creates a client for the JACK server
        to provide some info of server. 

        Argument info:
            --argument 1 ('GtkWidget *sw') is declared in 'main.c'
            --argument 2 ('GtkWidget *label') is declared in 'display.c'
            --argument 3 ('GtkWidget *label2') is declared in 'display.c'
    */

    jack_client_t *client;
    jack_status_t status;
    gchar srate[10];
    //const gchar **playback;
    gint i;

    sleep (2);
    
    client = jack_client_open ("gjackctl", 
                               JackNoStartServer,
                               &status);
    i = 0;

    

    //ports = g_slice_new (jack_port);
    //ports -> in_port = jack_port_register (client, "capture", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    //ports -> out_port = jack_port_register (client, "playback", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);      

    //playback = jack_get_ports (client, NULL, NULL, JackPortIsPhysical | JackPortIsInput);

    //ports = set_playback_ports (playback);

    /*while (playback[i])
    {
        g_print ("create_jack_client.c: %s\n", playback[i]);

        //alloc_port (playback[i]);
        i++;
    }*/

    jack_set_process_callback (client, process_cb, NULL);
    jack_on_shutdown (client, jack_shutdown_cb, client);
    jack_set_sample_rate_callback (client, srate_cb, label2);

    jack_activate (client); 

    dsp_init (sw, label, client); /* function updates label to show cpu load. */

    g_sprintf (srate, "%d", jack_get_sample_rate (client));
    gtk_label_set_text (GTK_LABEL (label2), g_strdup (srate));        

    return 0;
}
