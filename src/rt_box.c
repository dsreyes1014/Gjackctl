#include "rt_box.h"

static gboolean
get_realtime (config_t config)
{
	gboolean realtime;
	gchar config_file[128];

	g_sprintf (config_file, "%s/.config/gjackctl/gjackctl.conf", g_getenv ("HOME"));

	config_init (&config);
	config_read_file (&config, config_file);
	
	config_lookup_bool (&config, "gjackctl.server.realtime", &realtime);

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

        config_file_input1 ("gjackctl.server.realtime",
                            CONFIG_TYPE_BOOL,
                            (gpointer) &value);
    }
    else
    {	
        value = FALSE;

		config_file_input1 ("gjackctl.server.realtime",
                            CONFIG_TYPE_BOOL,
                            (gpointer) &value);
    }
}

static void
real_time_cb (GtkToggleButton *button, gpointer data)
{
    gboolean check;
		
    check = gtk_toggle_button_get_active (button);

    if (check == TRUE)
    {
        gtk_widget_set_tooltip_text (GTK_WIDGET (button), 
                                     "Disable Realtime Audio");
    }
    else
    {	
        gtk_widget_set_tooltip_text (GTK_WIDGET (button), 
                                     "Enable Realtime Audio");
    }
}

void
rt_box (GtkWidget *grid, GtkWidget *button)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */
	
	GtkWidget *checkbox;
    GtkWidget *label;
	config_t config;
	gboolean realtime;

	checkbox = gtk_check_button_new ();
    label = gtk_label_new ("Realtime");
	config_init (&config);

	realtime = get_realtime (config);

	if (realtime == FALSE)
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbox), FALSE);
	}
	else
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbox), TRUE);
	}

	/* Initiate tooltip for `checkbox` in the if/else statement. It won't 
	show when app first starts if we don't. */
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbox)) == TRUE)
	{
		gtk_widget_set_tooltip_text (checkbox, "Disable realtime audio");	
	}
	else
	{	
		gtk_widget_set_tooltip_text (checkbox, "Enable realtime audio");
	}

	/* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
    gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), checkbox, 1, 1, 1, 1);

	gtk_widget_set_margin_start (label, 40);
	gtk_widget_set_margin_top (checkbox, 10);
	gtk_widget_set_margin_top (label, 10);

    g_signal_connect (checkbox, "toggled", G_CALLBACK (real_time_cb), NULL);
    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), checkbox);
}
