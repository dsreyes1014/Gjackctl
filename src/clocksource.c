#include "clocksource.h"

typedef struct _GtkPassedClockSourceData {
    GtkWidget *button;
    GtkWidget *popover;
} GtkPassedClockSourceData;

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *pbutton;
   
    pbutton = user_data;

    config_file_input ("gjackctl.server.clocksource",
                       CONFIG_TYPE_STRING,
                       (gpointer) gtk_button_get_label (GTK_BUTTON (pbutton))); 
}

static const gchar *
get_clocksource (config_t config)
{
    const gchar *string;
    gchar *file;

    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

    config_init (&config);
    config_read_file (&config, file);
    config_lookup_string (&config, "gjackctl.server.clocksource", &string);
    
    return string;
}

static void
button_toggled_cb (GtkToggleButton *tb, gpointer user_data)
{
    GtkPassedClockSourceData *rdata;

    rdata = user_data;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (tb)) == TRUE)
    {
        gtk_button_set_label (GTK_BUTTON (rdata -> button), gtk_button_get_label (GTK_BUTTON (tb)));
    }

    gtk_widget_hide (rdata -> popover);
}

static void
popover_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *tb;
    GtkWidget *box;
    GtkWidget *radio1;
    GtkWidget *radio2;
    GtkWidget *radio3;
    GtkWidget *popover;
    GtkPassedClockSourceData *pdata;
    GSList *list; 
    config_t config;   

    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);    
    radio1 = gtk_radio_button_new_with_label (NULL, "Cycle");
    radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "Hpet");
    radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "System");
    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

    pdata = (GtkPassedClockSourceData *) g_malloc (sizeof (GtkPassedClockSourceData));
    pdata -> popover = gtk_popover_new (GTK_WIDGET (button));
    pdata -> button = (GTK_WIDGET (button));    
   
    while (list)
    {
        tb = list -> data;

        if (g_strcmp0 (gtk_button_get_label (GTK_BUTTON (tb)), get_clocksource (config)) == 0)
        {
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tb), TRUE);
        }

        list = list -> next;
    }

    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

    gtk_box_pack_start (GTK_BOX (box), radio1, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), radio2, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), radio3, FALSE, FALSE, 2);
    gtk_container_add (GTK_CONTAINER (pdata -> popover), box);

    g_signal_connect (radio1, "toggled", G_CALLBACK (button_toggled_cb), pdata);
    g_signal_connect (radio2, "toggled", G_CALLBACK (button_toggled_cb), pdata);
    g_signal_connect (radio3, "toggled", G_CALLBACK (button_toggled_cb), pdata);
   
    gtk_widget_show_all (pdata -> popover);   
}

void
clocksource (GtkWidget *box, GtkWidget *button)
{
    GtkWidget *label;
    GtkWidget *pbutton;
    GtkWidget *child_box1;
    GtkWidget *child_box2;
    config_t config;
    
    child_box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    child_box2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    label = gtk_label_new ("Clock Source"); 
    pbutton = gtk_button_new_with_label (get_clocksource (config));

    gtk_widget_set_tooltip_text (pbutton, "Choose a specific wall clock.");
    gtk_button_set_relief (GTK_BUTTON (pbutton), GTK_RELIEF_NONE);
    gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold 11.5"));

    /* Pack box. */
    gtk_box_pack_start (GTK_BOX (child_box1), label, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box1), pbutton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), child_box1, FALSE, FALSE, 2);
  
    gtk_widget_set_size_request (pbutton, 80, 10);
    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_margin_start (label, 40);
    gtk_widget_set_margin_start (pbutton, 40);

    g_signal_connect (pbutton, "clicked", G_CALLBACK (popover_button_clicked_cb), NULL);
    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), pbutton);
}
