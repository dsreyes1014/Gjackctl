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

    GtkPassedServerSwitchData *rdata;
    
	rdata = user_data;

	/* Check if `GPid pid` exists. */
	if (gtk_switch_get_active (sw) == TRUE)
	{						 
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw) , "Shutdown Server");
	    if (jack_server_init (GTK_WIDGET (sw), rdata -> text, rdata -> window) != 0)
        {
            //gtk_switch_set_active (sw, FALSE);
        }
	}
	else
	{	        
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw), "Start Server");
		kill (get_jack_gpid (), SIGKILL);	
        clear_log_view (rdata -> text);     
	}
}   

void 
server_switch (GtkWidget *window, 
               GtkWidget *text,
               GtkApplication *app, 
               GtkWidget *header_bar,
               GtkWidget *sw)
{	
	gint check_pid;
	gboolean check;	
    GtkPassedServerSwitchData *pdata;
    
	check = gtk_switch_get_active (GTK_SWITCH (sw));
    
    pdata = g_slice_new (GtkPassedServerSwitchData);    
    pdata -> text = text;
    pdata -> window = window;

    gtk_text_view_set_editable (GTK_TEXT_VIEW (text), FALSE);

	/* Check to see if server is already started. */
	check_pid = kill (get_jack_gpid (), 0);

	if (check_pid == 0)
	{
        gtk_switch_set_active (GTK_SWITCH (sw), TRUE);   
	}

	gtk_header_bar_pack_start (GTK_HEADER_BAR (header_bar), sw);

	gtk_widget_set_valign (sw, GTK_ALIGN_CENTER);
	
	gjackctl_settings (window, app, header_bar);
	//connections (vbox);
	
	/* Initiate tooltip for `jack_switch` here or else it won't show when 
	app first starts. */
	if (gtk_switch_get_active (GTK_SWITCH (sw)) == TRUE)
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw), "Shutdown Server");
	}
	else
	{
		gtk_widget_set_tooltip_text (GTK_WIDGET (sw), "Start Server");	
	}

    g_signal_connect (sw, "notify::active", G_CALLBACK (switch_pos_cb), pdata);
}
