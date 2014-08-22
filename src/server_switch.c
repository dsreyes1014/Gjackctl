#include "server_switch.h"

typedef struct _GtkPassedServerSwitchData {    
    GtkWidget *window; 
    GtkWidget *text; 
} GtkPassedServerSwitchData;

static void
clear_log_view (GtkWidget *text)
{
    GtkTextBuffer *buffer;
    GtkTextIter start;
    GtkTextIter end;

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text));

    gtk_text_buffer_get_start_iter (buffer, &start);
    gtk_text_buffer_get_end_iter (buffer, &end);

    gtk_text_buffer_delete (buffer, &start, &end);
}

static void
switch_pos_cb (GtkSwitch *sw, GParamSpec *pspec, gpointer user_data)
{	
	/* This callback function will start JACK when switched on and terminate 
	it when switched off. */

	GPid pid;
	gboolean check;
	gboolean check_pid;
	FILE *get_pid;
	gchar pid_string[16];
	GtkWidget *window;
    GtkPassedServerSwitchData *rdata;

	check = gtk_switch_get_active (sw);	
	pid = -2;
	rdata = user_data;

	/* Here we get the `GPid pid` from the `get_pid` statement, which
	uses the `pgrep` command to obtain the pid, and convert it to a gint 
	using the `g_ascii_strtoll ()` function. */
	get_pid = popen ("pgrep jackd", "r");

	while (fgets (pid_string, sizeof (pid_string), get_pid) != NULL)
	{
		pid = atoi (pid_string);
	}

	pclose (get_pid);
	
	/* Check if `GPid pid` exists. */
	check_pid = kill (pid, 0);

	if (check == TRUE)
	{						
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Shutdown Server");
		
		/* If server isn't started yet, start it here when switch is 
		turned on. */
		if (check_pid != 0)
		{
			jack_server_init (sw, pid, rdata -> text, rdata -> window);
		}
	}
	else
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Start Server");
		kill (pid, SIGTERM);	
        clear_log_view (rdata -> text);        
	}
}   

void 
server_switch (GtkWidget *window, 
               GtkWidget *text,
               GtkApplication *app, 
               GtkWidget *header_bar)
{	
	GtkWidget *jack_switch;
	gchar result[10];
	gint check_pid;
	gboolean check;	
	FILE *cmd;
	GPid pid;
    GtkPassedServerSwitchData *pdata;
    
	pid = -2;
	cmd = popen ("pgrep jackd", "r");
	jack_switch = gtk_switch_new ();
	check = gtk_switch_get_active (GTK_SWITCH (jack_switch));
    
    pdata = g_slice_new (GtkPassedServerSwitchData);    
    pdata -> window = window;
    pdata -> text = text;

    gtk_text_view_set_editable (GTK_TEXT_VIEW (text), FALSE);
    //gtk_text_view_set_overwrite (GTK_TEXT_VIEW (pdata -> text), TRUE);

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

	gtk_header_bar_pack_start (GTK_HEADER_BAR (header_bar), jack_switch);

	gtk_widget_set_valign (jack_switch, GTK_ALIGN_CENTER);
	
	gjackctl_settings (window, app, header_bar);
	//connections (vbox);
	
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

    g_signal_connect (jack_switch,
                      "notify::active", 
                   	  G_CALLBACK (switch_pos_cb),
                      pdata);
}
