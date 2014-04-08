#include <gtk/gtk.h>

#include "drivers.h"

void 
alsa_sel (GtkMenuItem *item, gpointer data)
{

}

void
activate_popover (GtkWidget *button, gpointer data)
{
	GtkWidget *popover;
	GMenu *menu;  
	GMenu *section; 
	GMenu *submenu;
	GMenuItem *item1;
	GMenuItem *item2;
	GdkRGBA color;
	
	color.red = 0.0; 
	color.green = 0.0; 
	color.blue = 0.0; 
	color.alpha = 0.5;

	/* 'menu' is the main menu which is a subclass of 'GMenuModel'. All 'GMenus' are. */
	menu = g_menu_new ();	
	section = g_menu_new ();	
	submenu = g_menu_new ();	
	item1 = g_menu_item_new ("firewire", NULL);
	item2 = g_menu_item_new ("test", NULL);
	popover = gtk_popover_new_from_model (button, G_MENU_MODEL (menu));

	g_menu_insert_item (submenu, 0, item2);
		
	/* Creates a submenu from an item in 'section'. */
	g_menu_insert_submenu (section, 0, "ALSA", G_MENU_MODEL (submenu));	
	g_menu_insert_item (section, 1, item1);

	/* Creates the first section of 'menu' which labels drivers */
	g_menu_insert_section (menu, 0, "Driver", G_MENU_MODEL (section));	

	gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_RIGHT);
	gtk_widget_set_size_request (GTK_WIDGET (popover), 90, 60);
	gtk_widget_override_background_color (popover, GTK_STATE_FLAG_NORMAL, &color);

	gtk_widget_show_all (popover);
}

void
drivers (GtkWidget *box)
{
	GtkWidget *label;
	GtkWidget *label2;
	GtkWidget *button;
	
	label = gtk_label_new ("Driver");
	label2 = gtk_label_new ("Test");	
	button = gtk_button_new ();	

	gtk_widget_set_tooltip_text (button , "Choose Driver");

	gtk_box_pack_start (GTK_BOX (box), label, FALSE, TRUE, 4);
	gtk_container_add (GTK_CONTAINER (button), label2);
	gtk_box_pack_start (GTK_BOX (box), button, FALSE, TRUE, 0);

	g_signal_connect (button, "clicked", G_CALLBACK (activate_popover), NULL);
}


