#include "toggle_no_memlock.h"

static gboolean
get_no_memlock (config_t config)
{
	gboolean realtime;
	gchar config_file[128];

	g_sprintf (config_file, "%s/.config/gjackctl/gjackctl.conf", g_getenv ("HOME"));

	config_init (&config);
	config_read_file (&config, config_file);
	
	config_lookup_bool (&config, "gjackctl.server.no_memlock", &realtime);

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

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkPassedData *data;
    gboolean value;

    data = user_data;
    
    if (data -> passed_state == GTK_LABEL_NORMAL_ON)
    {
        value = TRUE;

        config_file_input ("gjackctl.server.no_memlock",
                           CONFIG_TYPE_BOOL,
                           (gpointer) &value);
    }
    else
    {	
        value = FALSE;

		config_file_input ("gjackctl.server.no_memlock",
                           CONFIG_TYPE_BOOL,
                           (gpointer) &value);
    }

    g_slice_free (GtkPassedData, data);
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

static gboolean
enter_event_box_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GtkWidget *label;

    label = user_data;
    gtk_label_set_attributes (GTK_LABEL (label), label_prelight ());   
    
    return FALSE;                             
}

static gboolean
event_box_released_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GtkPassedData *data;
   
    data = user_data;

    if (data -> passed_state == GTK_LABEL_NORMAL_OFF)
    {
        data -> passed_state = label_normal_on (GTK_LABEL (data -> passed_label));
        gtk_widget_set_tooltip_text (widget, "Lock memory");	
        
    }
    else 
    {
        data -> passed_state = label_normal_off (GTK_LABEL (data -> passed_label));
        gtk_widget_set_tooltip_text (widget, 
                                     "Do not lock memory, even in realtime.");
    }

    return FALSE;
}

void
toggle_no_memlock (GtkWidget *box, GtkWidget *button)
{
	GtkWidget *event_box;
    GtkWidget *label;
    gboolean memlock;
    config_t config;
    GtkPassedData *data;
    gint state;
    
    data = g_slice_new (GtkPassedData);
    label = gtk_label_new ("No Memlock");
    event_box = gtk_event_box_new ();
    config_init (&config);
    memlock = get_no_memlock (config);
    data -> passed_label = label;
    
    if (memlock == FALSE)
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
        gtk_widget_set_tooltip_text (event_box, "Lock memory");	
    }
    else
    {	
        gtk_widget_set_tooltip_text (event_box, 
                                     "Do not lock memory, even in realtime.");
    }

    gtk_widget_add_events (event_box, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events (event_box, GDK_BUTTON_RELEASE_MASK);
    gtk_widget_add_events (event_box, GDK_ENTER_NOTIFY_MASK);
    gtk_widget_add_events (event_box, GDK_LEAVE_NOTIFY_MASK);
    gtk_widget_add_events (event_box, GDK_PROPERTY_CHANGE_MASK);

    /* Pack grid. */
    gtk_container_add (GTK_CONTAINER (event_box), label);
    gtk_box_pack_start (GTK_BOX (box), event_box, FALSE, FALSE, 2);

	//gtk_widget_set_margin_end (event_box, 20);
    gtk_widget_set_halign (event_box, GTK_ALIGN_CENTER);
    //gtk_widget_set_margin_start (event_box, 80);

    g_signal_connect (event_box,
                      "button-release-event",
                      G_CALLBACK (event_box_released_cb),
                      data);
    g_signal_connect (event_box,
                      "enter-notify-event",
                      G_CALLBACK (enter_event_box_cb),
                      label);
    g_signal_connect (event_box,
                      "leave-notify-event",
                      G_CALLBACK (leave_event_box_cb),
                      data);
    g_signal_connect (button,
                      "clicked",
                      G_CALLBACK (button_clicked_cb),
                      data);
}
