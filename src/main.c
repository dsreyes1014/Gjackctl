#include <gtk/gtk.h>
#include <jack/jack.h>

#include "server_switch.h"
#include "display.h"
#include "jack_log.h"
#include "config_file_init.h"

static void
visible_child_cb (GtkWidget *stack, GParamSpec *pspec, gpointer user_data)
{
    if (g_strcmp0 (gtk_stack_get_visible_child_name (GTK_STACK (stack)), "display") == 0)
    {
        gtk_window_resize (GTK_WINDOW (user_data), 600, 220);
    }
   
    if (g_strcmp0 (gtk_stack_get_visible_child_name (GTK_STACK (stack)), "log") == 0)
    {
        gtk_window_resize (GTK_WINDOW (user_data), 870, 500);
    }
}

static void
run_app_cb (GApplication *app, gpointer data)
{
	GtkWidget *window;	
	GtkWidget *header_bar;
	GtkWidget *stack;
    GtkWidget *text;
    GtkWidget *sswitcher;	
    GtkWidget *sw;

	window = gtk_application_window_new (GTK_APPLICATION (app));
	stack = gtk_stack_new ();
	header_bar = gtk_header_bar_new ();
    sw = gtk_switch_new ();    
    text = gtk_text_view_new ();
    sswitcher = gtk_stack_switcher_new ();
    
    gtk_stack_set_homogeneous (GTK_STACK (stack), FALSE);

	config_file_init ();
    
	server_switch (window, 
                   text,
                   GTK_APPLICATION (app),
                   header_bar,
                   sw);

    display (stack, sw);
    jack_log (stack, text);

	//gtk_header_bar_set_title (GTK_HEADER_BAR (header_bar), "GJackCtl");
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header_bar), TRUE);
	gtk_header_bar_set_decoration_layout (GTK_HEADER_BAR (header_bar), ":close");

    gtk_stack_switcher_set_stack (GTK_STACK_SWITCHER (sswitcher),
                                  GTK_STACK (stack));

	gtk_header_bar_set_custom_title (GTK_HEADER_BAR (header_bar), sswitcher);
	
	/* Set `GtkHeaderBar *head_bar` as titlebar. */
	gtk_window_set_titlebar (GTK_WINDOW (window), header_bar);
	gtk_window_set_default_size (GTK_WINDOW (window), 600, 220);

	gtk_container_add (GTK_CONTAINER (window), stack);	

	/* Position `window` to show wherever current mouse position is located. */	
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);

    g_signal_connect (stack, "notify::visible-child-name", G_CALLBACK (visible_child_cb), window);

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
