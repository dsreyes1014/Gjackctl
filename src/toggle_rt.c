#include "toggle_rt.h"

typedef struct _GtkPassedRtData GtkPassedRtData;

static gboolean
get_realtime ()
{
	gboolean realtime;
	gchar *file;
    config_t config;

    file = g_strconcat (g_getenv ("HOME"),
                               "/.config/gjackctl/gjackctl.conf",
                               NULL);

	config_init (&config);
	config_read_file (&config, file);
	
	if (config_lookup_bool (&config, "gjackctl.server.realtime", &realtime) == CONFIG_FALSE)
    {
        config_setting_t *group;
        config_setting_t *setting;
        
        g_print ("Realtime config option not available\n");
        g_print ("Creating config setting now...\n");

        group = config_lookup (&config, "gjackctl.server");

        setting = config_setting_add (group, "realtime", CONFIG_TYPE_BOOL);
        config_setting_set_bool (setting, TRUE);
        config_write_file (&config, file);       
    }

	if (realtime == FALSE)
	{
		config_destroy (&config);

        g_free (file);

		return FALSE;
	}
	else
    {
        config_destroy (&config);	

        g_free (file);

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
        gtk_widget_set_tooltip_text (widget, "Disable realtime audio");
    }
    else 
    {
        data -> passed_state = label_normal_off (GTK_LABEL (data -> passed_label));
        gtk_widget_set_tooltip_text (widget, "Enable realtime audio");        
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

        config_file_input ("gjackctl.server.realtime",
                           CONFIG_TYPE_BOOL,
                           (gpointer) &value);
    }
    else
    {	
        value = FALSE;

		config_file_input ("gjackctl.server.realtime",
                           CONFIG_TYPE_BOOL,
                           (gpointer) &value);
    }

    g_slice_free (GtkPassedData, data);
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
toggle_rt (GtkWidget *box, GtkWidget *button)
{   
    /* This gets called from `gjackctl_setings_cb` that's in the 
    `gjackctl_settings.c` module. */
	
    GtkWidget *label;
    GtkWidget *event_box;
    gboolean realtime;
    gint state;
    GtkPassedData *data;
 
    label = gtk_label_new ("Realtime");
    event_box = gtk_event_box_new ();
    state = label_normal_off (GTK_LABEL (label));

    data = g_slice_new (GtkPassedData);
    data -> passed_label = label;
  
    realtime = get_realtime ();
    if (realtime == FALSE)
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
        gtk_widget_set_tooltip_text (event_box, "Disable realtime audio");	
    }
    else
    {	
        gtk_widget_set_tooltip_text (event_box, "Enable realtime audio");
    }

    gtk_event_box_set_visible_window (GTK_EVENT_BOX (event_box), TRUE);

    /* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
    in the `gjackctl_settings_cb` function. */
    gtk_container_add (GTK_CONTAINER (event_box), label);
    gtk_box_pack_start (GTK_BOX (box), event_box, FALSE, FALSE, 2);

    gtk_widget_set_halign (event_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top (event_box, 20);
      
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
