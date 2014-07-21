#include "rt_priority.h"

static const gchar *
get_priority (config_t config)
{
    const gchar *priority;
    gchar *file;

    config_init (&config);
    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

    config_read_file (&config, file);
    config_lookup_string (&config, "gjackctl.server.priority", &priority);

    return priority;
}

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *spin_button;
    gint priority;
    gchar string[10];

    spin_button = user_data;
    priority = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spin_button));        
    g_sprintf (string, "%d", priority);

    config_file_input ("gjackctl.server.priority",
                       CONFIG_TYPE_STRING,
                       (gpointer) string);    
}

void
rt_priority (GtkWidget *grid, GtkWidget *button)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */

	//GtkWidget *label;
	GtkWidget *spin_button;
    GtkWidget *frame;
	GtkAdjustment *adjustment;
    gint priority;
    config_t config;
    const gchar *string;

    frame = gtk_frame_new ("Priority");
	adjustment = gtk_adjustment_new (75, 0, 99, 1, 0, 0);
	spin_button = gtk_spin_button_new (adjustment, 1, 0);
    string = get_priority (config);
    priority = g_ascii_strtoll (string, NULL, 0);

    config_destroy (&config);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_button), priority);

	/* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
    gtk_container_add (GTK_CONTAINER (frame), spin_button);
	gtk_grid_attach (GTK_GRID (grid), frame, 1, 0, 1, 1);
	
	gtk_widget_set_margin_top (frame, 6);
	gtk_widget_set_margin_top (spin_button, 4);
	gtk_widget_set_margin_end (spin_button, 6);
    gtk_widget_set_margin_start (spin_button, 6);
    gtk_widget_set_margin_bottom (spin_button, 4);

    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), spin_button);
}
