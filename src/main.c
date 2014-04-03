#include <gtk/gtk.h>

#include "server_switch.h"


static void
run_app (GApplication *app, gpointer data)
{
	GtkWidget *window;
	GtkWidget *box;

	window = gtk_application_window_new (GTK_APPLICATION (app));
	box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);

	server_switch (box);

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
