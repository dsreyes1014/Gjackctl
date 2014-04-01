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
	/* Declarations */

	GtkWidget *window;
	GtkWidget *box;
	GtkWidget *buttonbox;
	GtkWidget *frame;

	GdkRGBA color;
	
	/* Initialize */

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	buttonbox = gtk_button_box_new (GTK_ORIENTATION_VERTICAL);
	frame = gtk_frame_new (NULL);	

	/* Widget attributes */

	gtk_widget_set_size_request (frame, 350, 120);
	gtk_widget_set_halign (frame, GTK_ALIGN_BASELINE);
	gdk_rgba_parse (&color, "black");
	gtk_widget_override_background_color (frame,
											GTK_STATE_FLAG_NORMAL,
											&color);

	/* Signals */

	g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

	/* Widget packing */

	gtk_box_pack_start (GTK_BOX (box), 
						buttonbox,
						FALSE,
						FALSE,
						2);
	gtk_box_pack_end (GTK_BOX (box),
						frame,
						TRUE,
						FALSE,
						2);

	gtk_container_add (GTK_CONTAINER (window), box);

	/* Show all in toplevel window */

	gtk_widget_show_all (window);

	/* Start main loop */

	gtk_main (); 

	return 0;
}
