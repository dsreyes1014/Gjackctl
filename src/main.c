#include <gtk/gtk.h>


/**** close_window ****/
void 
close_window ()
{
	gtk_main_quit ();
}

/**** main ****/
int
main (int argc, char *argv[])
{
	/* Place declarations here */

	GtkWidget *window;
	
	gtk_init (&argc, &argv); 

	/* Initialize here */

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

	gtk_widget_show (window);

	gtk_main (); 

	return 0;
}
