#include "timeout.h"

typedef struct _GtkPassedTimeoutData {
    GtkWidget *pbutton;
    GtkWidget *popover;
} GtkPassedTimeoutData;

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkPassedTimeoutData *rdata;

    rdata = user_data;
    
    config_file_input ("gjackctl.server.timeout",
                       CONFIG_TYPE_STRING,
                       (gpointer) gtk_button_get_label (GTK_BUTTON (rdata -> pbutton)));

    g_slice_free (GtkPassedTimeoutData, rdata);
}

static void
notify_button_toggled_cb (GtkToggleButton *tb, GParamSpec *pspec, gpointer user_data)
{
    GtkPassedTimeoutData *rdata;

    rdata = user_data;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (tb)) == TRUE)
    {
        gtk_button_set_label (GTK_BUTTON (rdata -> pbutton),
                              gtk_button_get_label (GTK_BUTTON (tb)));
    }

    gtk_widget_hide (rdata -> popover);
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

static void
popover_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *tb;
    GtkWidget *box;
    GtkWidget *radio1;
    GtkWidget *radio2;
    GtkWidget *radio3;
    GtkWidget *radio4;
    GtkWidget *radio5;
    GtkWidget *radio6;
    GtkPassedTimeoutData *pdata;
    GSList *list; 
    config_t config;   

    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);    
    radio1 = gtk_radio_button_new_with_label (NULL, "200");
    radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "500");
    radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "1000");
    radio4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "2000");
    radio5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "5000");
    radio6 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "10000");    
    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

    pdata = user_data;  

    if (pdata -> popover == NULL)
    {
        pdata -> popover = gtk_popover_new (GTK_WIDGET (button));

        while (list)
        {
            tb = list -> data;

            if (g_strcmp0 (gtk_button_get_label (GTK_BUTTON (tb)),
                get_timeout (config)) == 0)
            {
                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tb), TRUE);
            }

            list = list -> next;
        }
    
        gtk_box_pack_start (GTK_BOX (box), radio1, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (box), radio2, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (box), radio3, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (box), radio4, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (box), radio5, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (box), radio6, FALSE, FALSE, 2);
        gtk_container_add (GTK_CONTAINER (pdata -> popover), box);
    }
    else
    {
        g_print ("Not NULL\n");
    }

    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

    g_signal_connect (radio1,
                      "notify::active",
                      G_CALLBACK (notify_button_toggled_cb),
                      pdata);

    g_signal_connect (radio2,
                      "notify::active",
                      G_CALLBACK (notify_button_toggled_cb),
                      pdata);

    g_signal_connect (radio3,
                      "notify::active",
                      G_CALLBACK (notify_button_toggled_cb),
                      pdata);

    g_signal_connect (radio4,
                      "notify::active",
                      G_CALLBACK (notify_button_toggled_cb),
                      pdata);

    g_signal_connect (radio5,
                      "notify::active",
                      G_CALLBACK (notify_button_toggled_cb),
                      pdata);

    g_signal_connect (radio6,
                      "notify::active",
                      G_CALLBACK (notify_button_toggled_cb),
                      pdata);
   
    gtk_widget_show_all (pdata -> popover);   
}

void
timeout (GtkWidget *box, GtkWidget *button)
{
    GtkWidget *label;
    GtkWidget *child_box1;
    config_t config;
    GtkPassedTimeoutData *pdata;
    
    label = gtk_label_new ("Timeout"); 
    child_box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);

    pdata = g_slice_new (GtkPassedTimeoutData);
    pdata -> pbutton = gtk_button_new_with_label (get_timeout (config));
    pdata -> popover = NULL;
    
    gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold 11.5"));
    gtk_widget_set_size_request (pdata -> pbutton, 80, 10);
    gtk_widget_set_tooltip_text (pdata -> pbutton, "Set JACK client timeout in milliseconds");

    gtk_button_set_relief (GTK_BUTTON (pdata -> pbutton), GTK_RELIEF_NONE);

    /* Pack box. */
    gtk_box_pack_start (GTK_BOX (child_box1), label, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box1), pdata -> pbutton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), child_box1, FALSE, FALSE, 2);

    gtk_widget_set_margin_start (label, 10);
    gtk_widget_set_margin_start (pdata -> pbutton, 10);

    g_signal_connect (pdata -> pbutton, "clicked", G_CALLBACK (popover_button_clicked_cb), pdata);
    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), pdata);
}
