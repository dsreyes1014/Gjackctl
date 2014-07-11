#include "drivers.h"

const GActionEntry entries[] =
	{
		{"print_alsa_driver", print_alsa_driver_activate_cb, "s"}
	};

static PangoAttrList *
label_settings_attr ()
{
	PangoAttrList *list;
	PangoAttribute *attr;

	list = pango_attr_list_new ();

	attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
	pango_attr_list_insert (list, attr);
	attr = pango_attr_weight_new (PANGO_WEIGHT_HEAVY);
	pango_attr_list_insert (list, attr);	
	attr = pango_attr_size_new (14000);
	pango_attr_list_insert (list, attr);	

	return list;
}

static void
driver_popover_cb (GtkWidget *button, GdkEvent *event, gpointer user_data)
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
	item1 = g_menu_item_new ("firewire", NULL);
	
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

void
drivers (GtkWidget *grid, GtkApplication *app)
{
	GtkWidget *driver_button;
	GtkWidget *label;
	GtkWidget *label_driver;
	GtkWidget *event_box;
	gchar **argvp;
	gint i;

	label = gtk_label_new ("Driver/Interface");
	event_box = gtk_event_box_new ();
	argvp = get_arg_vector ();
	label_driver = gtk_label_new_with_mnemonic ("_Driver");	
	driver_button = gtk_button_new ();	
	i = 0;

	gtk_label_set_attributes (GTK_LABEL (label), label_settings_attr ());

	g_action_map_add_action_entries (G_ACTION_MAP (app),
									 entries, 
									 G_N_ELEMENTS (entries), 
									 label_driver);
	
	while (argvp[i])
	{
		if ((strncmp (argvp[i], "-dalsa", 2) == 0) &&
			(strncmp (argvp[i + 1], "-d", 2) == 0))	
		{
			gchar *tt_label;
			gchar *tooltip;
	
			tt_label = g_strdup (argvp[i + 1]);
			tooltip = g_strconcat ("Soundcard: '", &tt_label[5], "'", NULL);

			gtk_label_set_text (GTK_LABEL (label_driver), "ALSA");
			gtk_widget_set_tooltip_text (label_driver, tooltip); 
			break;	
		}	
		else
		{
			i++;
			gtk_widget_set_tooltip_text (label_driver, "Choose Soundcard");
		}
	} 

	g_strfreev (argvp);

	//gtk_widget_set_tooltip_text (label_driver, "Choose Driver");
	gtk_widget_set_halign (driver_button, GTK_ALIGN_CENTER);
	gtk_widget_set_size_request (driver_button, 96, 34);

	/* Pack `grid`. */
	gtk_container_add (GTK_CONTAINER (event_box), label_driver);
	gtk_grid_attach (GTK_GRID (grid), label, 1, 5, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), event_box, 1, 6, 1, 1);

	g_signal_connect (event_box, "button-press-event", 
					  G_CALLBACK (driver_popover_cb), NULL);
}
