#include "port_max.h"

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *tb;
    GSList *list;
    
    list = user_data;

    while (list)
    {
        tb = list -> data;

        if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (tb)) == TRUE)
        {
            config_file_input ("gjackctl.server.port_max",
                               CONFIG_TYPE_STRING,
                               (gpointer) gtk_button_get_label (GTK_BUTTON (tb)));
            g_print ("Selected: %s\n", gtk_button_get_label (GTK_BUTTON (tb)));
        }
        
        g_print ("%s not selected\n", gtk_button_get_label (GTK_BUTTON (tb)));

        list = list -> next;
    }    
}

static const gchar *
get_port_max (config_t config)
{
    const gchar *string;
    gchar *file;

    file = g_strconcat (g_getenv ("HOME"),
                               "/.config/gjackctl/gjackctl.conf",
                               NULL);
    config_init (&config);
    config_read_file (&config, file);
    config_lookup_string (&config, "gjackctl.server.port_max", &string);
    
    return string;
}

void
port_max (GtkWidget *grid, GtkWidget *button)
{
    GtkWidget *frame;
    GtkWidget *radio1;
    GtkWidget *radio2;
    GtkWidget *radio3;
    GtkWidget *radio4;
    GtkWidget *grid1;
    GtkWidget *tb;
    const gchar *string;
    GSList *list;
    config_t config;
    
    frame = gtk_frame_new ("Port Max"); 
    grid1 = gtk_grid_new ();
    radio1 = gtk_radio_button_new_with_label (NULL, "128");
    radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "256");
    radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "512");
    radio4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "1024");
    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));
    string = get_port_max (config);

    gtk_widget_set_tooltip_text (frame, "Choose maximum number of ports for the JACK server to manage.");

    while (list)
    {
        tb = list -> data;

        if (g_strcmp0 (gtk_button_get_label (GTK_BUTTON (tb)), string) == 0)
        {
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tb), TRUE);
        }

        list = list -> next;
    }

    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

    /* Pack grid. */
    gtk_grid_attach (GTK_GRID (grid1), radio1, 0, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (grid1), radio2, 1, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (grid1), radio3, 2, 0, 1, 1);
    gtk_grid_attach (GTK_GRID (grid1), radio4, 3, 0, 1, 1);
    gtk_container_add (GTK_CONTAINER (frame), grid1);                
    gtk_grid_attach (GTK_GRID (grid), frame, 2, 0, 1, 1);
    
    gtk_widget_set_margin_top (frame, 6);  
    gtk_widget_set_margin_end (frame, 10);  

    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), list);
}
