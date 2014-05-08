#include "jack_server_init.h"

GPid pid;
gchar *jack_start[20];

gint 
jack_server_init ()
{
	jack_client_t *client;
	jack_status_t status;
	gint check_pid;

	jack_start[0] = "jackd";
	jack_start[5] = NULL;

	g_spawn_async (NULL, jack_start, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &pid, NULL);

	check_pid = kill (pid, 0);
	/*if (check_pid == 0)
	{
		client = jack_client_open ("gjackctl", JackServerName, &status, "default");
		jack_activate (client);
		g_timeout_add (1000, (GSourceFunc) dsp_load, (gpointer) client);
	}*/

	//g_print ("%s\n", jack_start[2]);

	return 0;
}
