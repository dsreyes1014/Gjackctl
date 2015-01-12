#include "dsp_load.h"

typedef struct _GtkPassedDspData GtkPassedDspData;

struct _GtkPassedDspData {
    jack_client_t *client;
    GtkWidget *label;
    GtkWidget *sw;
    GtkWidget *level_bar;
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
        g_slice_free (GtkPassedDspData, rdata);

		return FALSE;

        
	}
	else
	{		
		float dsp;
		gchar print_dsp[16];
        gchar *label_dsp;
		
		dsp = jack_cpu_load (rdata -> client);	
		g_sprintf (print_dsp, "%.2f%", dsp);
        label_dsp = g_strconcat (print_dsp, "%", NULL);
		gtk_label_set_text (GTK_LABEL (rdata -> label), label_dsp);
        gtk_level_bar_set_value (GTK_LEVEL_BAR (rdata -> level_bar), dsp);
    
        gtk_widget_set_tooltip_text (rdata -> level_bar, "CPU Load");

		return TRUE;
	}
}

void
dsp_init (GtkWidget *sw, GtkWidget *label, jack_client_t *client, GtkWidget *level_bar)
{
    GtkPassedDspData *pdata;

    pdata = g_slice_new (GtkPassedDspData);

    pdata -> client = client;
    pdata -> sw = sw;
    pdata -> label = label;
    pdata -> level_bar = level_bar;

    g_timeout_add (1000, (GSourceFunc) dsp_load, pdata);
}
