#include "gjackctl_settings.h"

void
popup_destroy_cb (GtkWidget *widget, gpointer user_data)
{
	gtk_widget_destroy (widget);
	gtk_widget_show_all (window);
}

void
popup_clicked_ok_cb (GtkButton *button, gpointer user_data)
{
	GtkWidget *widget;

	widget = user_data;

	gtk_widget_destroy (widget);
	gtk_widget_show_all (window);
}

void
gjackctl_settings_cb (GtkWidget *button, gpointer user_data)
{	
	GtkWidget *popup;
	GtkWidget *grid;
	GtkWidget *header_bar;
	GtkWidget *button1;
	GtkWidget *separator;
	GtkApplication *app;
		
	grid = gtk_grid_new ();
	app = user_data;
	button1 = gtk_button_new_with_label ("OK");
	separator = gtk_separator_new (GTK_ORIENTATION_VERTICAL);
	popup = gtk_application_window_new (GTK_APPLICATION (app));
	header_bar = gtk_header_bar_new ();

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
	
	gtk_widget_set_size_request (popup, 600,400);
	
	g_signal_connect (popup, "destroy", G_CALLBACK (popup_destroy_cb), NULL);
	g_signal_connect (button1, "clicked", G_CALLBACK (popup_clicked_ok_cb), popup);

	gtk_widget_show_all (popup);
	gtk_widget_hide (window);
}

void
gjackctl_settings (GtkWidget *grid, GtkApplication *app)
{
	GtkWidget *settings_button;

	settings_button = gtk_button_new_with_mnemonic ("_Settings");
	
	gtk_widget_set_tooltip_text (settings_button, "JACK Server Settings");

	gtk_grid_attach (GTK_GRID (grid), settings_button, 1, 2, 1, 1);	
	
	g_signal_connect (settings_button, "clicked", G_CALLBACK (gjackctl_settings_cb), app);
}

