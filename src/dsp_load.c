#include "dsp_load.h"

/* Declared in `rt_box.c`. */
extern GtkWidget *label_dsp;

/* Declared in `server_switch.c`. */
extern GPid pid;
//extern jack_client_t *client;

gboolean
dsp_load (jack_client_t *client)
{
	gint check_pid;
	
	check_pid = kill (pid, 0);

	if (check_pid != 0)
	{
		gtk_label_set_text (GTK_LABEL (label_dsp), "DSP");

		return FALSE;
	}
	else
	{		
		float dsp;
		gchar print_dsp[6];
		
		dsp = jack_cpu_load (client);	
		sprintf (print_dsp, "%.2f", dsp);
		gtk_label_set_text (GTK_LABEL (label_dsp), print_dsp);
		//g_print ("PID from dsp_load.c: %d\n", pid);
		//g_print ("CPU Load from dsp_load.c: %.2f\n", dsp);

		return TRUE;
	}
}
