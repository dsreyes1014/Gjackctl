#include "gjackctl_settings.h"

void
popup_destroy_cb (GtkWidget *widget, gpointer user_data)
{
	GtkWidget *window;
	
	window = user_data;

	gtk_widget_destroy (widget);
	gtk_widget_show_all (window);
}

void
popup_clicked_ok_cb (GtkButton *button, gpointer user_data)
{
	GtkWidget *widget1;
	GtkWidget *widget2;

	/* This is a `struct` that holds variables passed by 
	the `g_signal_connect ()` function through `gpointer user_data`. */
	pass_data_2 *data_received;
	
	data_received = user_data;
	widget1 = data_received -> data1;
	widget2 = data_received -> data2;

	gtk_widget_destroy (widget2);
	gtk_widget_show_all (widget1);
	g_free (data_received);
}

void
gjackctl_settings_cb (GtkButton *button, gpointer user_data)
{	
	GtkWidget *popup;
	GtkWidget *grid;
	GtkWidget *header_bar;
	GtkWidget *button1;
	GtkWidget *separator;
	GtkWidget *window;
	GtkApplication *app;

	/* This is a `struct` that holds variables passed by 
	the `g_signal_connect ()` function through `gpointer user_data`. */ 
	pass_data *data_received;
	pass_data_2 *data_to_pass;	

	data_to_pass = (pass_data_2 *) g_malloc (sizeof (pass_data_2));
	data_received = user_data;
	grid = gtk_grid_new ();
	app = data_received -> data2;
	button1 = gtk_button_new_with_label ("OK");
	separator = gtk_separator_new (GTK_ORIENTATION_VERTICAL);
	popup = gtk_application_window_new (GTK_APPLICATION (app));
	header_bar = gtk_header_bar_new ();
	window = data_received -> data1;
	data_to_pass -> data1 = window;
	data_to_pass -> data2 = popup;

	gtk_grid_set_row_spacing (GTK_GRID (grid), 4);
	gtk_grid_set_column_spacing (GTK_GRID (grid), 4);
	gtk_grid_set_column_homogeneous (GTK_GRID (grid), FALSE);
	gtk_grid_set_row_homogeneous (GTK_GRID (grid), FALSE);

	/* Show header_bar. */
	gtk_header_bar_set_title (GTK_HEADER_BAR (header_bar), "Settings");
	gtk_window_set_titlebar (GTK_WINDOW (popup), header_bar);
	gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), button1);

	/* Pack popup. */
	//server_name (grid);	
	rt_box (grid);
	//gtk_container_add (GTK_CONTAINER (hbox), separator);	
	rt_priority (grid);
	drivers (grid);
	sample_rate (grid);
	gtk_container_add (GTK_CONTAINER (popup), grid);
	
	gtk_widget_set_size_request (popup, 600, 400);
	
	g_signal_connect (popup, "destroy", 
					  G_CALLBACK (popup_destroy_cb), window);
	g_signal_connect (button1, "clicked", 
					  G_CALLBACK (popup_clicked_ok_cb), 
					  (gpointer) data_to_pass);

	/* Position `popup` to show wherever current mouse position is located. */
	gtk_window_set_position (GTK_WINDOW (popup), GTK_WIN_POS_MOUSE);
	
	gtk_widget_show_all (popup);
	gtk_widget_hide (window);
}

void
gjackctl_settings (GtkWidget *grid, GtkWidget *window, GtkApplication *app)
{
	GtkWidget *settings_button;
	GtkWidget *grid_space;
	pass_data *data_to_pass;

	data_to_pass = (pass_data *) g_malloc (sizeof (pass_data));

	data_to_pass -> data1 = window;
	data_to_pass -> data2 = app;
	settings_button = gtk_button_new_with_mnemonic ("_Settings");
	grid_space = gtk_label_new ("");

	gtk_widget_set_tooltip_text (settings_button, "JACK Server Settings");

	gtk_grid_attach (GTK_GRID (grid), settings_button, 1, 2, 1, 1);	
	//gtk_grid_attach (GTK_GRID (grid), grid_space, 91, 1, 1, 1);

	g_signal_connect (settings_button, 
					  "clicked", 
					  G_CALLBACK (gjackctl_settings_cb), 
					  (gpointer) data_to_pass);
}

