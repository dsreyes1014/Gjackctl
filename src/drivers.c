#include <gtk/gtk.h>

void
activate_popover (GtkButton *button, gpointer data)
{
	GtkWidget *popover;
	GtkWidget *label;
	GdkRGBA color;
	GMenuModel *model;
	GtkWidget *menu;

	popover = gtk_popover_new (GTK_WIDGET (button));
	label = gtk_label_new ("Hi");
	gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_RIGHT);
	{color.red = 0.0; color.green = 0.0; color.blue = 0.0; color.alpha = 0.6;}		
	menu = gtk_menu_new ();	

	gtk_menu_button_set_popover (GTK_MENU_BUTTON (button), popover);

	gtk_container_add (GTK_CONTAINER (popover), label);
	gtk_container_add (GTK_CONTAINER (popover), menu);

	gtk_widget_set_size_request (GTK_WIDGET (popover), 120,100);
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
	button = gtk_menu_button_new ();	

	gtk_widget_set_tooltip_text (button , "Choose Driver");

	gtk_container_add (GTK_CONTAINER (box), label);
	gtk_container_add (GTK_CONTAINER (button), label2);
	gtk_container_add (GTK_CONTAINER (box), button);

	g_signal_connect (button, "clicked", G_CALLBACK (activate_popover), NULL);
}


