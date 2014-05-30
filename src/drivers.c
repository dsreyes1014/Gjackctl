#include "drivers.h"

//GtkWidget *label_driver;

void
driver_popover_cb (GtkWidget *button, gpointer user_data)
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
drivers (GtkWidget *grid)
{
	GtkWidget *driver_button;
	GtkWidget *label_driver;
	gchar **argvp;
	gint i;
	
	argvp = get_arg_vector ();
	label_driver = gtk_label_new_with_mnemonic ("_Driver");	
	driver_button = gtk_button_new ();	
	i = 0;
	
	while (argvp[i])
	{
		if ((strncmp (argvp[i], "-dalsa", 2) == 0) &&
			(strncmp (argvp[i + 1], "-d", 2) == 0))	
		{
			gchar *tt_label;
	
			tt_label = g_strdup (argvp[i + 1]);
			
			gtk_label_set_text (GTK_LABEL (label_driver), "ALSA");
			gtk_widget_set_tooltip_text (label_driver, &tt_label[2]); 
			break;	
		}	
		else
		{
			i++;
		}
	}

	//gtk_widget_set_tooltip_text (label_driver, "Choose Driver");
	gtk_widget_set_halign (driver_button, GTK_ALIGN_CENTER);
	gtk_widget_set_size_request (driver_button, 96, 34);

	/* Pack `grid`. */
	gtk_container_add (GTK_CONTAINER (driver_button), label_driver);
	gtk_grid_attach (GTK_GRID (grid), driver_button, 2, 3, 1, 1);

	g_signal_connect (driver_button, "clicked", 
					  G_CALLBACK (driver_popover_cb), NULL);
}
