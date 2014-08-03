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
timeout (GtkWidget *box, GtkWidget *button)
{
    GtkWidget *label;
    GtkWidget *radio1;
    GtkWidget *radio2;
    GtkWidget *radio3;
    GtkWidget *radio4;
    GtkWidget *radio5;
    GtkWidget *radio6;
    GtkWidget *child_box1;
    GtkWidget *child_box2;
    GtkWidget *child_box3;
    GtkWidget *tb;
    const gchar *string;
    GSList *list;
    config_t config;
    
    label = gtk_label_new ("Timeout (ms)"); 
    child_box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    child_box2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    child_box3 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    radio1 = gtk_radio_button_new_with_label (NULL, "200");
    radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "500");
    radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "1000");
    radio4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "2000");
    radio5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "5000");
    radio6 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "10000");
    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));
    string = get_timeout (config);

    gtk_widget_set_tooltip_text (child_box1, "Choose JACK client timeout in milliseconds.");

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

    gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold 11.5"));

    /* Pack box. */
    gtk_box_pack_start (GTK_BOX (child_box2), radio1, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box2), radio2, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box2), radio3, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box2), radio4, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box3), radio5, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box3), radio6, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box1), label, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box1), child_box2, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box1), child_box3, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), child_box1, FALSE, FALSE, 2);

    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_margin_start (label, 60);
    gtk_widget_set_margin_start (radio1, 80);
    gtk_widget_set_margin_start (radio5, 146);
    gtk_widget_set_margin_top (label, 10);

    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), list);
}
