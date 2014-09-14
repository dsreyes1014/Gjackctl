#include "drivers.h"

const GActionEntry entries[] =
	{
		{"print_alsa_driver", print_alsa_driver_activate_cb, "s"}
	};

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkPassedDriverData *rdata;
   
    rdata = user_data;

    config_file_input ("gjackctl.driver.type",
                       CONFIG_TYPE_STRING,
                       (gpointer) gtk_button_get_label (GTK_BUTTON (rdata -> pbutton)));

    config_file_input ("gjackctl.driver.device",
                       CONFIG_TYPE_STRING,
                       (gpointer) gtk_label_get_text (GTK_LABEL (rdata -> label))); 
}

static void
popover_button_clicked_cb (GtkWidget *button, GdkEvent *event, gpointer user_data)
{
	/* Creates popover menu for drivers button. */

	GtkWidget *popover;	
	GMenu *menu;  
	GMenu *submenu;
	GMenu *section; 
	GMenuItem *item1;
	
	/* `menu` is the main menu which is a subclass of `GMenuModel`. 
	All `GMenus` are. */
	menu = g_menu_new ();	
	section = g_menu_new ();	
	submenu = g_menu_new ();
	item1 = g_menu_item_new ("Dummy", NULL);
	
	alsa_device_names (submenu);	
		
	/* Creates a submenu from an item in `section`. `GMenu submenu` 
	is declared in `alsa_device_names.h` */
	g_menu_insert_submenu (section, 0, "ALSA", G_MENU_MODEL (submenu));
	g_menu_insert_item (section, 1, item1);
	
	/* Creates the first section of `menu` which labels drivers */
	g_menu_insert_section (menu, 0, "Driver", G_MENU_MODEL (section));

	popover = gtk_popover_new_from_model (button, G_MENU_MODEL (menu));	

	gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_RIGHT);
	gtk_popover_set_modal (GTK_POPOVER (popover), TRUE);
	
	gtk_widget_show_all (popover);
} 

static const gchar *
get_driver_type (config_t config)
{
    const gchar *string;
    gchar *file;

    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

    config_init (&config);
    config_read_file (&config, file);
    config_lookup_string (&config, "gjackctl.driver.type", &string);
    
    return string;
}

static const gchar *
get_driver_device (config_t config)
{
    const gchar *string;
    gchar *file;

    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

    config_init (&config);
    config_read_file (&config, file);
    config_lookup_string (&config, "gjackctl.driver.device", &string);
    
    return string;
}

void
drivers (GtkWidget *box, GtkApplication *app, GtkWidget *button)
{
    GtkWidget *child_box;
	GtkWidget *label;
    GtkWidget *separator;
    GtkPassedDriverData *pdata;
    gchar *tt;
    config_t config;
	
    pdata = g_malloc (sizeof (GtkPassedDriverData));	

    label = gtk_label_new ("Driver/Interface");	
	pdata -> pbutton = gtk_button_new_with_label (get_driver_type (config));
    pdata -> label = gtk_label_new (get_driver_device (config));	
    child_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);

    tt = g_strconcat ("Current device is ", 
                      "\"",
                      get_driver_device (config),
                      "\"", 
                      "\n",
                      "Click to choose driver/device",
                      NULL);

	g_action_map_add_action_entries (G_ACTION_MAP (app),
									 entries, 
									 G_N_ELEMENTS (entries), 
									 pdata);
	
    gtk_button_set_relief (GTK_BUTTON (pdata -> pbutton), GTK_RELIEF_NONE);

    gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold 11.5"));
    gtk_widget_set_size_request (pdata -> pbutton, 80, 10);
    gtk_widget_set_tooltip_text (pdata -> pbutton, tt);

    gtk_box_pack_start (GTK_BOX (child_box), label, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box), pdata -> pbutton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box), separator, FALSE, FALSE, 2); 
    gtk_box_pack_start (GTK_BOX (box), child_box, FALSE, FALSE, 2);

    //gtk_widget_set_halign (pdata -> pbutton, GTK_ALIGN_START);
    //tk_widget_set_margin_start (pdata -> pbutton, 22);
    //gtk_widget_set_margin_start (label, 18);
    gtk_widget_set_margin_top (child_box, 6);
	
    g_signal_connect (pdata -> pbutton, "clicked", G_CALLBACK (popover_button_clicked_cb), NULL);
    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), pdata);
}
