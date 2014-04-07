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
	GtkWidget *menu;
	GtkWidget *item1;
	GtkWidget *item2;
	GdkRGBA color;
	
	popover = gtk_popover_new (button);
	menu = gtk_menu_bar_new ();
	item1 = gtk_menu_item_new_with_label ("alsa");
	item2 = gtk_menu_item_new_with_label ("firewire");
	color.red = 0.0; 
	color.green = 0.0; 
	color.blue = 0.0; 
	color.alpha = 0.6;		
	
	gtk_menu_bar_set_pack_direction (GTK_MENU_BAR (menu), GTK_PACK_DIRECTION_TTB);
	gtk_menu_shell_prepend (GTK_MENU_SHELL (menu), item1);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), item2);

	g_signal_connect (item1, "activate", G_CALLBACK (alsa_sel), NULL);

	gtk_container_add (GTK_CONTAINER (popover), menu);

	gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_RIGHT);
	gtk_widget_set_size_request (GTK_WIDGET (popover), 90,60);
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

	gtk_container_add (GTK_CONTAINER (box), label);
	gtk_container_add (GTK_CONTAINER (button), label2);
	gtk_container_add (GTK_CONTAINER (box), button);

	g_signal_connect (button, "clicked", G_CALLBACK (activate_popover), NULL);
}


