#include "dsp_load.h"

typedef struct _GtkPassedDspData GtkPassedDspData;

struct _GtkPassedDspData {
    jack_client_t *client;
    GtkWidget *label;
    GtkWidget *sw;
};

static gboolean
dsp_load (gpointer user_data)
{
	/* Function that shows cpu load. Have to make `GJackCtl` a JACK
	client in order to see cpu usage. */
	gchar pid_string[6];
    GtkPassedDspData *rdata;

    rdata = user_data;

	if (gtk_switch_get_active (GTK_SWITCH (rdata -> sw)) == FALSE)
	{
		return FALSE;
	}
	else
	{		
		float dsp;
		gchar print_dsp[6];
		
		dsp = jack_cpu_load (rdata -> client);	
		g_sprintf (print_dsp, "%.2f", dsp);
		gtk_label_set_text (GTK_LABEL (rdata -> label), print_dsp);

		return TRUE;
	}
}

void
dsp_init (GtkWidget *sw, GtkWidget *label, jack_client_t *client)
{
    GtkPassedDspData *pdata;

    pdata = g_slice_new (GtkPassedDspData);

    pdata -> client = client;
    pdata -> sw = sw;
    pdata -> label = label;

    sleep (2);

    jack_activate (pdata -> client);

    g_timeout_add (1000, (GSourceFunc) dsp_load, pdata);
}
