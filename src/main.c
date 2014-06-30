#include <gtk/gtk.h>

#include "server_switch.h"
#include "display.h"

static void
run_app_cb (GApplication *app, gpointer data)
{
	GtkWidget *window;	
	GtkWidget *header_bar;
	GtkWidget *stack;
	
	window = gtk_application_window_new (GTK_APPLICATION (app));
	stack = gtk_stack_new ();
	header_bar = gtk_header_bar_new ();

	server_switch (window, GTK_APPLICATION (app), header_bar);
	display (stack, header_bar);

	gtk_header_bar_set_title (GTK_HEADER_BAR (header_bar), "GJackCtl");
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header_bar), TRUE);
	gtk_header_bar_set_decoration_layout (GTK_HEADER_BAR (header_bar), ":close");
	
	/* Set `GtkHeaderBar *head_bar` as titlebar. */
	gtk_window_set_titlebar (GTK_WINDOW (window), header_bar);
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 220);

	gtk_container_add (GTK_CONTAINER (window), stack);	

	/* Position `window` to show wherever current mouse position is located. */	
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);

	gtk_widget_show_all (window);
}

int 
main (int argc, char *argv[])
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gnome.gjackctl", G_APPLICATION_FLAGS_NONE);

	g_signal_connect (app, "activate", G_CALLBACK (run_app_cb), NULL);

	status = g_application_run (G_APPLICATION (app), argc, argv);

	g_object_unref (app);

	return status;	
}
