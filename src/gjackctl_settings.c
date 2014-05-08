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
	GtkWidget *hbox;
	GtkWidget *vbox;
	GtkWidget *header_bar;
	GtkWidget *button1;
	GtkWidget *separator;
	GtkApplication *app;
	
		
	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	app = user_data;
	button1 = gtk_button_new_with_label ("Ok");
	separator = gtk_separator_new (GTK_ORIENTATION_VERTICAL);

	popup = gtk_application_window_new (GTK_APPLICATION (app));
	header_bar = gtk_header_bar_new ();

	/* Show header_bar. */
	gtk_header_bar_set_title (GTK_HEADER_BAR (header_bar), "Settings");
	gtk_window_set_titlebar (GTK_WINDOW (popup), header_bar);
	gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), button1);

	/* Pack popup. */
	rt_box (hbox);
	gtk_container_add (GTK_CONTAINER (hbox), separator);
	drivers (vbox);
	sample_rate (vbox);
	gtk_box_pack_start (GTK_BOX (hbox), vbox, FALSE, TRUE, 2);
	gtk_container_add (GTK_CONTAINER (popup), hbox);

	gtk_widget_set_size_request (popup, 600,400);
	
	g_signal_connect (popup, "destroy", G_CALLBACK (popup_destroy_cb), NULL);
	g_signal_connect (button1, "clicked", G_CALLBACK (popup_clicked_ok_cb), popup);

	gtk_widget_show_all (popup);
	gtk_widget_hide (window);
}

void
gjackctl_settings (GtkWidget *box, GtkApplication *app)
{
	GtkWidget *settings_button;

	settings_button = gtk_button_new_with_mnemonic ("_Settings");
	
	gtk_widget_set_tooltip_text (settings_button, "JACK Server Settings");

	gtk_box_pack_start (GTK_BOX (box), settings_button, FALSE, TRUE, 2);

	g_signal_connect (settings_button, "clicked", G_CALLBACK (gjackctl_settings_cb), app);
}

