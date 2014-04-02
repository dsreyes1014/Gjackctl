#include <gtk/gtk.h>

static void
activate (GApplication *app, gpointer data)
{
	GtkWidget *window;

	window = gtk_application_window_new (GTK_APPLICATION (app));

	gtk_widget_show_all (window);
}

int 
main (int argc, char *argv[])
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gnome.gjackctl", G_APPLICATION_FLAGS_NONE);
	status = g_application_run (G_APPLICATION (app), argc, argv);

	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

	g_object_unref (app);

	return status;	
}
