#include "dsp_load.h"

/* Declared in `rt_box.c`. */
extern GtkWidget *label_dsp;

void
dsp_init ()
{
	/* Using the `kill ()` function to check if JACK server is running and `GPid pid` exists. 
	`GPid pid` is declared in `jack_server_init.h`. Using the `g_timeout_add ()` 
	function to refresh the cpu usage. `GtkWidget *jack_switch` declared in 
	`server_switch.h`. */

	jack_client_t *client;
	jack_status_t status;
	gint check_pid;

	check_pid = kill (pid, 0);
	
	if (check_pid == 0)
	{
		client = jack_client_open ("gjackctl", JackServerName, &status, "default");	
		gtk_switch_set_active (GTK_SWITCH (jack_switch), TRUE);
		jack_activate (client);
		g_timeout_add (1000, (GSourceFunc) dsp_load, (gpointer) client);
	}
}

gboolean
dsp_load (jack_client_t *client)
{
	/* Function that shows cpu load. Have to make `GJackCtl` a JACK
	client in order to see cpu usage. */

	gint check_pid;
	
	check_pid = kill (pid, 0);

	if (check_pid != 0)
	{
		gtk_label_set_text (GTK_LABEL (label_dsp), "DSP");
		gtk_widget_set_tooltip_text (GTK_WIDGET (label_dsp), "JACK not running");

		return FALSE;
	}
	else
	{		
		float dsp;
		gchar print_dsp[6];
		
		dsp = jack_cpu_load (client);	
		sprintf (print_dsp, "%.2f", dsp);
		gtk_label_set_text (GTK_LABEL (label_dsp), print_dsp);
		gtk_widget_set_tooltip_text (GTK_WIDGET (label_dsp), "CPU Load");
		//g_print ("PID from dsp_load.c: %d\n", pid);
		//g_print ("CPU Load from dsp_load.c: %.2f\n", dsp);

		return TRUE;
	}
}
