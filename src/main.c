#include "main.h"

static void
visible_child_cb (GtkWidget *stack, GParamSpec *pspec, gpointer user_data)
{
    if (g_strcmp0 (gtk_stack_get_visible_child_name (GTK_STACK (stack)), "display") == 0)
    {
        gtk_window_resize (GTK_WINDOW (user_data), 650, 200);
    }
   
    if (g_strcmp0 (gtk_stack_get_visible_child_name (GTK_STACK (stack)), "log") == 0)
    {
        gtk_window_resize (GTK_WINDOW (user_data), 750, 500);
    }
    
    if (g_strcmp0 (gtk_stack_get_visible_child_name (GTK_STACK (stack)), "ports") == 0)
    {
        gtk_window_resize (GTK_WINDOW (user_data), 650, 400);
    }
}

/*
static void
apply_theme ()
{
    GtkCssProvider *provider;
    GFile *file;
    GdkScreen *screen;
    const gchar *path;
    const gchar *theme_sheet;      

    theme_sheet = g_strdup ("./src/gjackctl-adwaita-dark.css");
    screen = gdk_screen_get_default ();    
    provider = gtk_css_provider_get_named ("adwaita", "dark");
    

    if (provider)
    {
        

        path = g_strconcat (g_getenv ("HOME"),
                            "/.config/gjackctl/",
                            theme_sheet, NULL);

        file = g_file_new_for_path (theme_sheet);
        
        gtk_style_context_add_provider_for_screen (screen,
                                                   GTK_STYLE_PROVIDER (provider),
                                                   GTK_STYLE_PROVIDER_PRIORITY_USER);

        gtk_css_provider_load_from_file (provider, file, NULL);

        g_object_unref (file);
    }
}*/

static void
run_app_cb (GApplication *app, gpointer data)
{
    GtkPassedMainData *pdata;
    jack_status_t status;

    pdata = g_slice_new (GtkPassedMainData);
	pdata -> window = gtk_application_window_new (GTK_APPLICATION (app));

    /*
     * Initialize our widgets here so we can manipulate from
     * different modules of our app.
     */
    pdata -> vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    pdata -> button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
	pdata -> header_bar = gtk_header_bar_new ();
    pdata -> sw = gtk_switch_new ();
    pdata -> text = gtk_text_view_new ();

    if (get_jack_gpid (NULL) != -2)
    {
        pdata -> client = jack_client_open ("gjackctl",
                                            JackNoStartServer,
                                            &status,
                                            "default");
    }
    else
    {
        pdata -> client = NULL;
    }

    gtk_button_box_set_layout (GTK_BUTTON_BOX (pdata -> button_box),
                               GTK_BUTTONBOX_END);

    /*
     * This function will setup our config file preset with some defaults.
     * Call this first as some of our modules need this config file to
     * reference certain jackd settings.
     */
	config_file_init ();

	//server_switch (pdata);
    gjackctl_settings (pdata);
    display (pdata);
    jack_log (pdata);

    g_object_set (gtk_settings_get_default (),
                  "gtk-application-prefer-dark-theme",
                  TRUE,
                  NULL);

    gtk_header_bar_pack_start (GTK_HEADER_BAR (pdata -> header_bar),
                               pdata -> sw);
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (pdata -> header_bar),
                                          TRUE);

	gtk_header_bar_set_decoration_layout (GTK_HEADER_BAR (pdata -> header_bar),
                                          ":close");

	/* Set 'GtkHeaderBar *head_bar' as titlebar. */
	gtk_window_set_titlebar (GTK_WINDOW (pdata -> window), pdata -> header_bar);
	gtk_window_set_default_size (GTK_WINDOW (pdata -> window), 650, 200);
    gtk_window_set_resizable (GTK_WINDOW (pdata -> window), FALSE);

    /*
     * Position 'window' to show wherever current mouse position is located.
     * Does not work for Wayland backend.
     */
    gtk_window_set_position (GTK_WINDOW (pdata -> window), GTK_WIN_POS_MOUSE);

    gtk_box_pack_end (GTK_BOX (pdata -> vbox), pdata -> button_box, FALSE, TRUE, 2);
	gtk_container_add (GTK_CONTAINER (pdata -> window), pdata -> vbox);

    g_signal_connect (pdata -> window,
                      "destroy",
                      G_CALLBACK (gtk_widget_destroy),
                      pdata -> client);

    gtk_widget_set_size_request (pdata -> button_box, 500, 20);
	gtk_widget_show_all (pdata -> window);
}

gint
main (int argc, char *argv[])
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.dsr.gjackctl", G_APPLICATION_FLAGS_NONE);

	g_signal_connect (app, "activate", G_CALLBACK (run_app_cb), NULL);

	status = g_application_run (G_APPLICATION (app), argc, argv);

	g_object_unref (app);

	return status;	
}
