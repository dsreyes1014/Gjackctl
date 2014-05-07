#include "drivers.h"

void
activate_popover_cb (GtkWidget *button, gpointer data)
{
	/* Creates popover menu for drivers button. */

	GtkWidget *popover;
	GMenu *menu;  
	GMenu *section; 
	GMenuItem *item1;

	/* `menu` is the main menu which is a subclass of `GMenuModel`. All `GMenus` are. */
	menu = g_menu_new ();	
	section = g_menu_new ();	
	item1 = g_menu_item_new ("firewire", NULL);

	alsa_device_names (data);	
		
	/* Creates a submenu from an item in `section`. `GMenu submenu` 
	is declared in `alsa_device_names.h` */
	g_menu_insert_submenu (section, 0, "ALSA", G_MENU_MODEL (submenu));

	g_menu_insert_item (section, 1, item1);
	
	/* Creates the first section of `menu` which labels drivers */
	g_menu_insert_section (menu, 0, "Driver", G_MENU_MODEL (section));

	popover = gtk_popover_new_from_model (button, G_MENU_MODEL (menu));	

	gtk_popover_set_modal (GTK_POPOVER (popover), TRUE);
	gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_RIGHT);

	gtk_widget_show_all (popover);
}

void
drivers (GtkWidget *box)
{
	GtkWidget *button;
	
	label_driver = gtk_label_new_with_mnemonic ("_Driver");	
	button = gtk_button_new ();	

	gtk_widget_set_tooltip_text (button, "Choose Driver");

	//gtk_box_pack_start (GTK_BOX (box), separator, FALSE, TRUE, 2);
	gtk_container_add (GTK_CONTAINER (button), label_driver);
	gtk_box_pack_start (GTK_BOX (box), button, FALSE, TRUE, 4);

	g_signal_connect (button, "clicked", G_CALLBACK (activate_popover_cb), NULL);
}
