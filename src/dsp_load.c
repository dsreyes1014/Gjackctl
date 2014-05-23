#include "dsp_load.h"

void
dsp_init (GtkWidget *sw, GPid pid)
{
	/* Using the `kill ()` function to check if JACK server is running and 
	`GPid pid` exists.  `GPid pid` is declared in `jack_server_init.h`.
	 Using the `g_timeout_add ()` function to refresh the cpu usage. 
	`GtkWidget *jack_switch` declared in `server_switch.c`. */

	jack_client_t *client;
	jack_status_t status;
	gint check_pid;

	check_pid = kill (pid, 0);
	
	if (check_pid == 0)
	{
		client = jack_client_open ("gjackctl", JackServerName,
									 &status, "default");	

		gtk_switch_set_active (GTK_SWITCH (sw), TRUE);

		jack_activate (client);

		g_timeout_add (1000, (GSourceFunc) dsp_load, client);
	}
}

gboolean
dsp_load (gpointer user_data)
{
	/* Function that shows cpu load. Have to make `GJackCtl` a JACK
	client in order to see cpu usage. */

	FILE *get_pid;
	gchar pid_string[6];
	gint check_pid;	
	GPid pid;
	jack_client_t *client;	
	
	/* Here we get the `GPid pid` from the `get_pid` variable and convert 
	it to an gint. */
	get_pid = popen ("pgrep jackd", "r");

	while (fgets (pid_string, sizeof (pid_string), get_pid) != NULL)
	{
		pid = g_ascii_strtoll (pid_string, NULL, 10);
	}
	
	pclose (get_pid);
	
	check_pid = kill (pid, 0);
	client = user_data;

	if (check_pid != 0)
	{
		gtk_label_set_text (GTK_LABEL (label_dsp), "--");
		gtk_widget_set_tooltip_text (GTK_WIDGET (label_dsp), 
									 "JACK not running");

		return FALSE;
	}
	else
	{		
		float dsp;
		gchar print_dsp[6];
		
		dsp = jack_cpu_load (client);	
		g_sprintf (print_dsp, "%.2f", dsp);
		gtk_label_set_text (GTK_LABEL (label_dsp), print_dsp);
		gtk_widget_set_tooltip_text (GTK_WIDGET (label_dsp), "CPU Load"); 

		return TRUE;
	}
}
