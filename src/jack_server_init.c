#include "jack_server_init.h"

gchar *jack_init[20];
gchar *driver_arg;
gchar device_arg[20];

gint 
jack_server_init ()
{
	jack_client_t *client;
	jack_status_t status;
	gint check_pid;

	jack_init[0] = COMMAND;
	jack_init[20] = NULL;

	g_spawn_async_with_pipes (NULL, jack_init, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, 
								&pid, NULL, NULL, NULL, NULL);

	//g_child_watch_add (pid, child_watch, NULL);
	check_pid = kill (pid, 0);
	if (check_pid == 0)
	{
		client = jack_client_open ("gjackctl", JackNoStartServer || JackServerName, &status, "default");
		//jack_activate (client);
		g_timeout_add (1000, (GSourceFunc) dsp_load, (gpointer) client);
	}

	g_print ("%s\n", jack_init[1]);

	return 0;
}
