#include "period.h"

const gchar *
get_period ()
{
    const gchar *string;
    const gchar *copy;
    gchar *file;
    config_t config;

    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

    config_init (&config);
    config_read_file (&config, file);
    if (config_lookup_string (&config, "gjackctl.driver.period", &string) == CONFIG_FALSE)
    {
        gchar *value_copy;
        config_setting_t *group;
        config_setting_t *setting;

        g_print ("\'Period Size\' config option not available\n");
        g_print ("Creating config setting now...\n");

        value_copy = g_strdup ("2");
        group = config_lookup (&config, "gjackctl.driver");
        setting = config_setting_add (group, "period", CONFIG_TYPE_STRING);
        config_setting_set_string (setting, value_copy);
        config_write_file (&config, file);
        string = g_strdup (value_copy);
        g_free (value_copy);
    }

    copy = g_strdup (string);
    g_free (file);
    config_destroy (&config);

    return copy;
}

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *spin_button;
    gint period;
    gchar string[10];

    spin_button = user_data;
    period = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spin_button));        
    g_sprintf (string, "%d", period);

    config_file_input ("gjackctl.driver.period",
                       CONFIG_TYPE_STRING,
                       (gpointer) string);    
}

static void
value_changed_cb (GtkSpinButton *button,
                  gpointer       user_data)
{
    config_file_input ("gjackctl.driver.period",
                       CONFIG_TYPE_STRING,
                       (gpointer) gtk_entry_get_text (GTK_ENTRY (button)));
}

void
period (GtkWidget *grid)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */

	GtkWidget *label;
	GtkWidget *spin_button;
    GtkWidget *child_box;
	GtkAdjustment *adjustment;
    gdouble value;
    config_t config;

    value = g_ascii_strtod (get_period (), NULL);
    label = gtk_label_new ("Period");
	adjustment = gtk_adjustment_new (value, 0, 6, 1, 0, 0);
	spin_button = gtk_spin_button_new (adjustment, 0, 0);

    gtk_widget_set_tooltip_text (spin_button,
                                 "Number of periods of playback latency");

    gtk_grid_attach (GTK_GRID (grid),
                     label,
                     0, 3, 1, 1);

    gtk_grid_attach_next_to (GTK_GRID (grid),
                             spin_button,
                             label,
                             GTK_POS_RIGHT,
                             2, 1);

    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_margin_start (spin_button, 20);
    gtk_widget_set_halign (spin_button, GTK_ALIGN_FILL);

    g_signal_connect (spin_button,
                      "value-changed",
                      G_CALLBACK (value_changed_cb),
                      NULL);
}
