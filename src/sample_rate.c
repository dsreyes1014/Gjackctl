#include "sample_rate.h"

typedef struct _GtkPassedSampleRateData {
    GtkWidget *pbutton;
    GtkWidget *popover;
} GtkPassedSampleRateData;

static void
warning_msg_box ()
{
	GtkWidget *msg_dialog;

	msg_dialog = gtk_message_dialog_new (NULL, 
										 GTK_DIALOG_MODAL, 
										 GTK_MESSAGE_WARNING,
										 GTK_BUTTONS_CLOSE,
										 "Please choose driver first!");
	
	//gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (msg_dialog),
											  //"Server could not be started!"); 

	gtk_dialog_run (GTK_DIALOG (msg_dialog));
	gtk_widget_destroy (msg_dialog);
}

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkPassedSampleRateData *rdata;

    rdata = user_data;
    
    config_file_input ("gjackctl.driver.sample_rate",
                       CONFIG_TYPE_STRING,
                       (gpointer) gtk_button_get_label (GTK_BUTTON (rdata -> pbutton)));

    g_slice_free (GtkPassedSampleRateData, rdata);
}

static void
button_toggled_cb (GtkToggleButton *tb, gpointer user_data)
{
    GtkPassedSampleRateData *rdata;

    rdata = user_data;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (tb)) == TRUE)
    {
        gtk_button_set_label (GTK_BUTTON (rdata -> pbutton), gtk_button_get_label (GTK_BUTTON (tb)));
    }

    gtk_widget_hide (rdata -> popover);
}

static const gchar *
get_sample_rate ()
{
    const gchar *string;
    gchar *file;
    config_t config;

    file = g_strconcat (g_getenv ("HOME"),
                               "/.config/gjackctl/gjackctl.conf",
                               NULL);
    config_init (&config);
    config_read_file (&config, file);
    config_lookup_string (&config, "gjackctl.driver.sample_rate", &string);

    g_free (file);
    
    return string;
}

static void
popover_button_clicked_cb (GtkWidget *button, gpointer user_data)
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
	GtkPassedSampleRateData *pdata;
    GSList *list; 
    config_t config;  
	gchar *string;

    pdata = user_data;
    pdata -> popover = gtk_popover_new (GTK_WIDGET (button));
    //pdata -> button = (GTK_WIDGET (button));

	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	
	/* `radio1` initiates group of radio buttons. */
	radio1 = gtk_radio_button_new_with_label (NULL, "22050");
	radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "32000");
	radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "44100");
	radio4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "48000");
	radio5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "88200");	
	radio6 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "96000");
	radio7 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "192000");
	list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

	while (list)
    {
        tb = list -> data;

        if (g_strcmp0 (gtk_button_get_label (GTK_BUTTON (tb)), get_sample_rate ()) == 0)
        {
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tb), TRUE);
        }

        list = list -> next;
    }

    list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

	gtk_box_pack_start (GTK_BOX (box), radio1, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio2, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio3, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio4, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio5, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio6, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio7, FALSE, TRUE, 2);
	gtk_container_add (GTK_CONTAINER (pdata -> popover), box);

	g_signal_connect (radio1, "toggled", G_CALLBACK (button_toggled_cb), pdata);
	g_signal_connect (radio2, "toggled", G_CALLBACK (button_toggled_cb), pdata);
	g_signal_connect (radio3, "toggled", G_CALLBACK (button_toggled_cb), pdata);
	g_signal_connect (radio4, "toggled", G_CALLBACK (button_toggled_cb), pdata);
	g_signal_connect (radio5, "toggled", G_CALLBACK (button_toggled_cb), pdata);
	g_signal_connect (radio6, "toggled", G_CALLBACK (button_toggled_cb), pdata);
	g_signal_connect (radio7, "toggled", G_CALLBACK (button_toggled_cb), pdata);

	gtk_widget_show_all (pdata -> popover);
}

void
sample_rate (GtkWidget *grid)
{
    GtkWidget *label;
    GtkWidget *mbutton;
    GtkWidget *mbutton_grid;
    GtkWidget *img;
    GtkWidget *entry;

    label = gtk_label_new ("Sample Rate");
    mbutton = gtk_menu_button_new ();
    mbutton_grid = gtk_grid_new ();
    img = gtk_image_new_from_icon_name ("pan-down-symbolic",
                                        GTK_ICON_SIZE_BUTTON);
    entry = gtk_entry_new ();

    gtk_grid_attach (GTK_GRID (grid),
                     label,
                     0, 1, 1, 1);

    gtk_grid_attach_next_to (GTK_GRID (grid),
                             entry,
                             label,
                             GTK_POS_RIGHT,
                             1, 1);
    gtk_grid_attach_next_to (GTK_GRID (grid),
                             mbutton,
                             entry,
                             GTK_POS_RIGHT,
                             1, 1);

    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_margin_start (entry, 20);
    gtk_widget_set_halign (entry, GTK_ALIGN_FILL);
}
