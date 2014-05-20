#include <gtk/gtk.h>

#include "server_switch.h"
#include "alsa_device_names.h"
#include "display.h"

GtkWidget *window;	

/* Callback `print_alsa_driver_activate` is defined in `alsa_device_names.c` */
const GActionEntry entries[] =
	{
		{"print_alsa_driver", print_alsa_driver_activate, "s"}
	};

void
run_app (GApplication *app, gpointer data)
{
	GtkWidget *header_bar;
	GtkWidget *grid;
	
	window = gtk_application_window_new (GTK_APPLICATION (app));
	grid = gtk_grid_new ();
	header_bar = gtk_header_bar_new ();

	g_action_map_add_action_entries (G_ACTION_MAP (app), entries, G_N_ELEMENTS (entries), app);
	gtk_grid_set_row_spacing (GTK_GRID (grid), 4);
	gtk_grid_set_column_spacing (GTK_GRID (grid), 4);	

	server_switch (grid, GTK_APPLICATION (app));
	display (grid);

	gtk_header_bar_set_title (GTK_HEADER_BAR (header_bar), "GJackCtl");
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header_bar), TRUE);
	gtk_header_bar_set_decoration_layout (GTK_HEADER_BAR (header_bar), "close:");
	/* Use `GtkHeaderBar *head_bar` as titlebar. */
	gtk_window_set_titlebar (GTK_WINDOW (window), header_bar);
	gtk_widget_set_size_request (window, 400, 200);

	gtk_container_add (GTK_CONTAINER (window), grid);	

	gtk_widget_show_all (window);
}

int 
main (int argc, char *argv[])
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gnome.gjackctl", G_APPLICATION_FLAGS_NONE);

	g_signal_connect (app, "activate", G_CALLBACK (run_app), NULL);

	status = g_application_run (G_APPLICATION (app), argc, argv);

	g_object_unref (app);

	return status;	
}
