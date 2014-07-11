#include "no_memlock.h"

static gboolean
get_no_memlock (config_t config)
{
	gboolean realtime;
	gchar config_file[128];

	g_sprintf (config_file, "%s/.config/gjackctl/gjackctl.conf", g_getenv ("HOME"));

	config_init (&config);
	config_read_file (&config, config_file);
	
	config_lookup_bool (&config, "gjackctl.server.no_memlock", &realtime);

	if (realtime == FALSE)
	{
		config_destroy (&config);

		return FALSE;
	}
	else
    {
        config_destroy (&config);	

        return TRUE;
   	}
}

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkToggleButton *checkbox;
    gboolean check;
    gboolean value;

    checkbox = user_data;
    check = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbox));

    if (check == TRUE)
    {
        value = TRUE;

        config_file_input1 ("gjackctl.server.no_memlock",
                            CONFIG_TYPE_BOOL,
                            (gpointer) &value);
    }
    else
    {	
        value = FALSE;

		config_file_input1 ("gjackctl.server.no_memlock",
                            CONFIG_TYPE_BOOL,
                            (gpointer) &value);
    }
}

static void
no_memlock_cb (GtkToggleButton *tb, gpointer user_data)
{
	gboolean check;

	check = gtk_toggle_button_get_active (tb);

	if (check == TRUE)
	{
		gtk_widget_set_tooltip_text (GTK_WIDGET (tb), 
                                     "Don't lock memory, even in realtime");
	}
	else
	{
		
	}
}

void
no_memlock (GtkWidget *grid, GtkWidget *button)
{
	GtkWidget *checkbutton;
    GtkWidget *label;
    gboolean memlock;
    config_t config;

	checkbutton = gtk_check_button_new ();
    label = gtk_label_new ("No MemLock");
    config_init (&config);
    memlock = get_no_memlock (config);

    if (memlock == FALSE)
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), FALSE);
	}
	else
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbutton), TRUE);
	}

    gtk_grid_attach (GTK_GRID (grid), label, 0, 2, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), checkbutton, 1, 2, 1, 1);

	gtk_widget_set_margin_start (label, 10);

	gtk_widget_set_tooltip_text (checkbutton, 
                                 "Don't lock memory, even in realtime.");

	g_signal_connect (checkbutton, "toggled", G_CALLBACK (no_memlock_cb), NULL);
    g_signal_connect (button,
                      "clicked",
                      G_CALLBACK (button_clicked_cb), 
                      checkbutton);
}
