#include "server_switch.h"

void
switch_pos_cb (GtkSwitch *sw, gpointer user_data)
{	
	/* This callback function will start JACK when switched on and terminate 
	it when switched off. */

	GPid pid;
	gboolean check;
	gboolean check_pid;
	FILE *get_pid;
	gchar pid_string[16];
	GtkWidget *window;

	check = gtk_switch_get_active (sw);	
	pid = -2;
	window = user_data;

	/* Here we get the `GPid pid` from the `get_pid` statement, which
	uses the `pgrep` command to obtain the pid, and convert it to a gint 
	using the `g_ascii_strtoll ()` function. */
	get_pid = popen ("pgrep jackd", "r");

	while (fgets (pid_string, sizeof (pid_string), get_pid) != NULL)
	{
		pid = atoi (pid_string);
	}
	
	g_print ("From `server_switch.c` line 28: %s\n", pid_string);

	pclose (get_pid);
	
	/* Check if `GPid pid` exists. */
	check_pid = kill (pid, 0);

	g_print ("From `server_switch.c` line 35: %d\n", pid);

	if (check == TRUE)
	{						
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Shutdown Server");
		
		/* If server isn't started yet, start it here when switch is 
		turned on. */
		if (check_pid != 0)
		{
			jack_server_init (sw, pid, window);
		}
	}
	else
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Start Server");
		kill (pid, SIGTERM);	
	}
}   

void 
server_switch (GtkWidget *grid, GtkWidget *window, GtkApplication *app)
{	
	GtkWidget *jack_switch;
	GtkWidget *grid_space;
	gchar result[10];
	gint check_pid;
	gboolean check;	
	FILE *cmd;
	GPid pid;

	pid = -2;
	cmd = popen ("pgrep jackd", "r");
	jack_switch = gtk_switch_new ();
	grid_space = gtk_label_new ("");
	check = gtk_switch_get_active (GTK_SWITCH (jack_switch));

	/* Here we get the `GPid pid` from the `cmd` statement and convert 
	it to a gint using the `g_ascii_strtoll ()` function. */
	while (fgets (result, sizeof (result), cmd) != NULL)
	{
		pid = g_ascii_strtoll (result, NULL, 10);
	}
	
	pclose (cmd);

	/* Check to see if server is already started. If it is we call 
	`dsp_init ()` to create this program as a JACK client and obtain 
	cpu load. */
	check_pid = kill (pid, 0);
	if (check_pid == 0)
	{
		dsp_init (jack_switch, pid);
	}
	
	/* Position widgets inside of `grid`. */
	gtk_widget_set_valign (jack_switch, GTK_ALIGN_START);
	gtk_widget_set_valign (grid_space, GTK_ALIGN_END);

	/* Pack `grid` which is declared in `main.c`. */
	gtk_grid_attach (GTK_GRID (grid), grid_space, 0, 0, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), jack_switch, 1, 1, 1, 1);	
	gjackctl_settings (grid, window, app);
	//connections (vbox);
	
	g_signal_connect (jack_switch, "notify::active", 
					  G_CALLBACK (switch_pos_cb), window);
	
	/* Initiate tooltip for `jack_switch` here or else it won't show when 
	app first starts. */
	if (check == TRUE)
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (jack_switch), 
									 "Shutdown Server");
	}
	else
	{
		gtk_widget_set_tooltip_text (GTK_WIDGET (jack_switch),
									 "Start Server");	
	}
}
