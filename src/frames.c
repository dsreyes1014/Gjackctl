#include "frames.h"

typedef struct _GtkPassedFramesData {
    GtkWidget *button;
    GtkWidget *popover;
} GtkPassedFramesData;

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *pbutton;

    pbutton = user_data;
    
    config_file_input ("gjackctl.driver.frames",
                       CONFIG_TYPE_STRING,
                       (gpointer) gtk_button_get_label (GTK_BUTTON (pbutton)));
}

static void
button_toggled_cb (GtkToggleButton *tb, gpointer user_data)
{
    GtkPassedFramesData *rdata;

    rdata = user_data;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (tb)) == TRUE)
    {
        gtk_button_set_label (GTK_BUTTON (rdata -> button), gtk_button_get_label (GTK_BUTTON (tb)));
    }

    gtk_widget_hide (rdata -> popover);
}

const gchar *
get_frames ()
{
    const gchar *string;
    gchar *file;
    config_t config;

    file = g_strconcat (g_getenv ("HOME"),
                               "/.config/gjackctl/gjackctl.conf",
                               NULL);
    config_init (&config);
    config_read_file (&config, file);
    config_lookup_string (&config, "gjackctl.driver.frames", &string);
    
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
    GtkWidget *radio7;
    GtkWidget *radio8;
    GtkWidget *radio9;
    GtkPassedFramesData *pdata;
    GSList *list; 
    config_t config;   

    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);    
    radio1 = gtk_radio_button_new_with_label (NULL, "16");
    radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "32");
    radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "64");
    radio4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "128");
    radio5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "256");
    radio6 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "512");
    radio7 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "1024");
    radio8 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "2048");
    radio9 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "4096");
      
    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

    pdata = g_slice_new (GtkPassedFramesData);
    pdata -> popover = gtk_popover_new (GTK_WIDGET (button));
    pdata -> button = (GTK_WIDGET (button));    
   
    while (list)
    {
        tb = list -> data;

        if (g_strcmp0 (gtk_button_get_label (GTK_BUTTON (tb)), get_frames ()) == 0)
        {
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tb), TRUE);
        }

        list = list -> next;
    }

    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

    gtk_box_pack_start (GTK_BOX (box), radio1, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), radio2, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), radio3, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), radio4, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), radio5, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), radio6, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), radio7, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), radio8, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), radio9, FALSE, FALSE, 2);
    gtk_container_add (GTK_CONTAINER (pdata -> popover), box);

    g_signal_connect (radio1, "toggled", G_CALLBACK (button_toggled_cb), pdata);
    g_signal_connect (radio2, "toggled", G_CALLBACK (button_toggled_cb), pdata);
    g_signal_connect (radio3, "toggled", G_CALLBACK (button_toggled_cb), pdata);
    g_signal_connect (radio4, "toggled", G_CALLBACK (button_toggled_cb), pdata);
    g_signal_connect (radio5, "toggled", G_CALLBACK (button_toggled_cb), pdata);
    g_signal_connect (radio6, "toggled", G_CALLBACK (button_toggled_cb), pdata);
    g_signal_connect (radio7, "toggled", G_CALLBACK (button_toggled_cb), pdata);
    g_signal_connect (radio8, "toggled", G_CALLBACK (button_toggled_cb), pdata);    
    g_signal_connect (radio9, "toggled", G_CALLBACK (button_toggled_cb), pdata);
   
    gtk_widget_show_all (pdata -> popover);   
}

void
frames (GtkWidget *box, GtkWidget *button)
{
    GtkWidget *label;
    GtkWidget *child_box1;
    GtkWidget *pbutton;
    gchar *string;
    config_t config;
    
    pbutton = gtk_button_new_with_label (get_frames ());
    label = gtk_label_new ("Frames"); 
    child_box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    
    //gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold 11.5"));
    gtk_widget_set_size_request (pbutton, 80, 10);
    gtk_widget_set_tooltip_text (pbutton, "Set number of frames between JACK process calls");

    gtk_button_set_relief (GTK_BUTTON (pbutton), GTK_RELIEF_NONE);

    /* Pack box. */
    gtk_box_pack_start (GTK_BOX (child_box1), label, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box1), pbutton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), child_box1, FALSE, FALSE, 2);

    g_signal_connect (pbutton, "clicked", G_CALLBACK (popover_button_clicked_cb), NULL);
    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), pbutton);
}
