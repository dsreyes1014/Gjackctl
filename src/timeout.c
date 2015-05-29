#include "timeout.h"

static const gchar *
get_timeout ()
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
    if (config_lookup_string (&config, "gjackctl.server.timeout", &string) == CONFIG_FALSE)
    {
        gchar *value_copy;
        config_setting_t *group;
        config_setting_t *setting;

        g_print ("\'Timeout\' config option not available\n");
        g_print ("Creating config setting now...\n");

        value_copy = g_strdup ("500");
        group = config_lookup (&config, "gjackctl.server");
        setting = config_setting_add (group, "timeout", CONFIG_TYPE_STRING);
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
value_changed_cb (GtkSpinButton *button,
                  gpointer       user_data)
{
    GtkAdjustment *adj;
    gchar *str;
    gint value;

    adj = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (button));
    value = gtk_spin_button_get_value_as_int (button);
    str = g_strdup_printf ("%d", value);

    config_file_input ("gjackctl.server.timeout",
                       CONFIG_TYPE_STRING,
                       (gpointer) str);

    g_free (str);
}

void
timeout (GtkWidget *grid)
{
    GtkWidget *label;
    GtkAdjustment *adj;
    gdouble k;
    const gchar * str;

    str = get_timeout ();
    k = g_ascii_strtod (str, NULL);
    GtkWidget *spin_button;
    label = gtk_label_new ("Timeout (ms)");
    adj = gtk_adjustment_new (k, 200, 10000, 100, 0, 0);
    spin_button = gtk_spin_button_new (adj, 100, 0);

    gtk_grid_attach (GTK_GRID (grid),
                     label,
                     0, 1, 1, 1);

    gtk_grid_attach_next_to (GTK_GRID (grid),
                             spin_button,
                             label,
                             GTK_POS_RIGHT,
                             1, 1);

    //gtk_widget_set_margin_start (label, 10);
    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_halign (spin_button, GTK_ALIGN_FILL);
    gtk_widget_set_margin_start (spin_button, 20);

    g_signal_connect (spin_button,
                      "value-changed",
                      G_CALLBACK (value_changed_cb),
                      &k);
}
