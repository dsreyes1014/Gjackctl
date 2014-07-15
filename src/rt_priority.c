#include "rt_priority.h"

static gint
get_priority (config_t config)
{
    gint priority;
    gchar *file;

    config_init (&config);
    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

    config_read_file (&config, file);
    config_lookup_int (&config, "gjackctl.server.priority", &priority);

    return priority;
}

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *spin_button;
    gint priority;

    spin_button = user_data;
    priority = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spin_button));        

    config_file_input ("gjackctl.server.priority",
                       CONFIG_TYPE_INT,
                       (gpointer) &priority);    
}

void
rt_priority (GtkWidget *grid, GtkWidget *button)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */

	GtkWidget *label;
	GtkWidget *spin_button;
	GtkAdjustment *adjustment;
    gint priority;
    config_t config;

	label = gtk_label_new ("Priority");
	adjustment = gtk_adjustment_new (75, 0, 99, 1, 0, 0);
	spin_button = gtk_spin_button_new (adjustment, 1, 0);
    priority = get_priority (config);

    config_destroy (&config);
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_button), priority);

	/* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
	gtk_grid_attach (GTK_GRID (grid), label, 2, 0, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), 
							 spin_button, 
							 label, 
							 GTK_POS_RIGHT, 1, 1);
	
	gtk_widget_set_margin_start (label, 15);
	gtk_widget_set_margin_top (label, 5);
	gtk_widget_set_margin_top (spin_button, 10);
	gtk_widget_set_margin_end (spin_button, 10);

    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), spin_button);
}
