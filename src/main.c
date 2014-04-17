#include <gtk/gtk.h>

#include "server_switch.h"
#include "rt_box.h"
#include "driver_alsa_names.h"

const GActionEntry entries[] =
	{
		{"alsa_driver", alsa_driver_activate, "s"}
	};	

void
run_app (GApplication *app, gpointer data)
{
	GtkWidget *window;
	GtkWidget *box;

	window = gtk_application_window_new (GTK_APPLICATION (app));
	box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 18); 	

	g_action_map_add_action_entries (G_ACTION_MAP (app), entries, G_N_ELEMENTS (entries), app);

	server_switch (box);
	rt_box (box);

	gtk_widget_set_size_request (window, 500, 250);
	gtk_window_set_title (GTK_WINDOW (window), "GJackCtl");

	gtk_container_add (GTK_CONTAINER (window), box);	

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
