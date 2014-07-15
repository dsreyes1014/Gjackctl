#include "midi.h"

static gboolean
get_midi (config_t config)
{
	gboolean realtime;
	gchar config_file[128];

	g_sprintf (config_file, "%s/.config/gjackctl/gjackctl.conf", g_getenv ("HOME"));

	config_init (&config);
	config_read_file (&config, config_file);
	
	config_lookup_bool (&config, "gjackctl.server.midi", &realtime);

	if (realtime == FALSE)
	{
		config_destroy (&config);

		return FALSE;
	}
	else
    {
        config_destroy (&config);	

        return TRUE;
   	}
}

static gboolean
event_box_released_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GtkPassedData *data;
   
    data = user_data;

    if (data -> passed_state == GTK_LABEL_NORMAL_OFF)
    {
        data -> passed_state = label_normal_on (GTK_LABEL (data -> passed_label));
        gtk_widget_set_tooltip_text (widget, "Disable MIDI");
    }
    else 
    {
        data -> passed_state = label_normal_off (GTK_LABEL (data -> passed_label));
        gtk_widget_set_tooltip_text (widget, "Enable MIDI");        
    }

    return FALSE;
}

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkPassedData *data;
    gboolean value;

    data = user_data;
    

    if (data -> passed_state == GTK_LABEL_NORMAL_ON)
    {
        value = TRUE;

        config_file_input ("gjackctl.server.midi",
                           CONFIG_TYPE_BOOL,
                           (gpointer) &value);
    }
    else
    {	
        value = FALSE;

		config_file_input ("gjackctl.server.midi",
                           CONFIG_TYPE_BOOL,
                           (gpointer) &value);
    }
}

static gboolean
enter_event_box_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GtkWidget *label;

    label = user_data;
    gtk_label_set_attributes (GTK_LABEL (label), label_prelight ());   
    
    return FALSE;                             
}

static gboolean
leave_event_box_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GtkPassedData *data;
        
    data = user_data;

    if (data -> passed_state == GTK_LABEL_NORMAL_ON)
    {
        label_normal_on (GTK_LABEL (data -> passed_label));
    }
    else
    {
        label_normal_off (GTK_LABEL (data -> passed_label));
    }

    return FALSE;
}

void
midi (GtkWidget *grid, GtkWidget *button)
{
    /* This gets called from `gjackctl_setings_cb` that's in the 
    `gjackctl_settings.c` module. */
	
    GtkWidget *label;
    GtkWidget *event_box;
    config_t config;
    gboolean midi;
    gint state;
    GtkPassedData *data;
 
    label = gtk_label_new ("MIDI");
    event_box = gtk_event_box_new ();
    config_init (&config);
    state = label_normal_off (GTK_LABEL (label));
    data = (GtkPassedData *) g_malloc (sizeof (GtkPassedData));
    data -> passed_label = label;
  
    midi = get_midi (config);
    if (midi == FALSE)
    {
        state = label_normal_off (GTK_LABEL (label));
        data -> passed_state = state;
    }
    else
    {
        state = label_normal_on (GTK_LABEL (label));
        data -> passed_state = state;
    }
    
    /* Initiate tooltip for `checkbox` in the if/else statement. It won't 
    show when app first starts if we don't. */
    if (state == GTK_LABEL_NORMAL_ON)
    {
        gtk_widget_set_tooltip_text (event_box, "Disable MIDI");	
    }
    else
    {	
        gtk_widget_set_tooltip_text (event_box, "Enable MIDI");
    }

    gtk_event_box_set_visible_window (GTK_EVENT_BOX (event_box), TRUE);

    /* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
    in the `gjackctl_settings_cb` function. */
    gtk_container_add (GTK_CONTAINER (event_box), label);
    gtk_grid_attach (GTK_GRID (grid), event_box, 1, 3, 1, 1);

    gtk_widget_set_margin_end (event_box, 126);
         
    gtk_widget_add_events (event_box, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events (event_box, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events (event_box, GDK_ENTER_NOTIFY_MASK);
    gtk_widget_add_events (event_box, GDK_LEAVE_NOTIFY_MASK);
    gtk_widget_add_events (event_box, GDK_PROPERTY_CHANGE_MASK);
    
    g_signal_connect (event_box, "button-release-event", G_CALLBACK (event_box_released_cb), data);
    g_signal_connect (event_box, "enter-notify-event", G_CALLBACK (enter_event_box_cb), label);
    g_signal_connect (event_box, "leave-notify-event", G_CALLBACK (leave_event_box_cb), data);
    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), data);
}
