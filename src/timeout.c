#include "timeout.h"

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
            config_file_input ("gjackctl.server.timeout",
                               CONFIG_TYPE_STRING,
                               (gpointer) gtk_button_get_label (GTK_BUTTON (tb)));
        }

        list = list -> next;
    }    
}

static const gchar *
get_timeout (config_t config)
{
    const gchar *string;
    gchar *file;

    file = g_strconcat (g_getenv ("HOME"),
                               "/.config/gjackctl/gjackctl.conf",
                               NULL);
    config_init (&config);
    config_read_file (&config, file);
    config_lookup_string (&config, "gjackctl.server.timeout", &string);
    
    return string;
}

void
timeout (GtkWidget *grid, GtkWidget *button)
{
    GtkWidget *frame;
    GtkWidget *radio1;
    GtkWidget *radio2;
    GtkWidget *radio3;
    GtkWidget *radio4;
    GtkWidget *radio5;
    GtkWidget *radio6;
    GtkWidget *grid1;
    GtkWidget *tb;
    const gchar *string;
    GSList *list;
    config_t config;
    
    frame = gtk_frame_new ("Timeout (ms)"); 
    grid1 = gtk_grid_new ();
    radio1 = gtk_radio_button_new_with_label (NULL, "200");
    radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "500");
    radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "1000");
    radio4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "2000");
    radio5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "5000");
    radio6 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "10000");
    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));
    string = get_timeout (config);

    gtk_widget_set_tooltip_text (frame, "Choose JACK client timeout in milliseconds.");

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
    gtk_grid_attach (GTK_GRID (grid1), radio2, 0, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid1), radio3, 0, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (grid1), radio4, 0, 3, 1, 1);
    gtk_grid_attach (GTK_GRID (grid1), radio5, 0, 4, 1, 1);
    gtk_grid_attach (GTK_GRID (grid1), radio6, 0, 5, 1, 1);
    gtk_container_add (GTK_CONTAINER (frame), grid1);                
    gtk_grid_attach (GTK_GRID (grid), frame, 2, 1, 1, 1);
    
    //gtk_widget_set_margin_top (frame, 6);  
    gtk_widget_set_margin_end (frame, 30);  
    //gtk_widget_set_margin_bottom (frame, 10);
    //gtk_widget_set_margin_start (frame, 14);
    gtk_widget_set_valign (frame, GTK_ALIGN_CENTER);

    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), list);
}
