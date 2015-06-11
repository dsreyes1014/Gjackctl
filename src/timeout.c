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

static gdouble
get_initial_value (const gchar *str)
{
    gint i;
    static gchar *timeout_str[6] = {"200", "500", "1000", "2000", "5000", "10000"};

    for (i = 1; i <= 6; i++)
    {
        if (g_strcmp0 (timeout_str[i - 1], str) == 0)
        {
            break;
        }
    }

    return (gdouble) i;
}

static void
value_changed_cb (GtkSpinButton *button,
                  gpointer       user_data)
{
    config_file_input ("gjackctl.server.timeout",
                       CONFIG_TYPE_STRING,
                       (gpointer) gtk_entry_get_text (GTK_ENTRY (button)));
}

static gint
input_cb (GtkSpinButton *button,
          gpointer       new_value,
          gpointer       user_data)
{
    gint i;
    static gchar *timeout_str[6] = {"200", "500", "1000", "2000", "5000", "10000"};
    gint value;
    gboolean check;

    check = FALSE;
    value = gtk_spin_button_get_value_as_int (button);

    for (i = 1; i <= 6; i++)
    {
        if (g_strcmp0 (timeout_str[i - 1], gtk_entry_get_text (GTK_ENTRY (button))) == 0)
        {
            check = TRUE;
            break;
        }
    }

    if (check == FALSE)
    {
        new_value = &value;
        return GTK_INPUT_ERROR;
    }

    new_value = &i;

    return TRUE;
}

static gboolean
output_cb (GtkSpinButton *spin_button,
           gpointer       user_data)
{
    gint i;
    gdouble value;
    static gchar *timeout_str[6] = {"200", "500", "1000", "2000", "5000", "10000"};

    value = gtk_spin_button_get_value (spin_button);

    for (i = 1; i <= 6; i++)
    {
        if ((gdouble) i == value)
        {
            gtk_entry_set_text (GTK_ENTRY (spin_button), timeout_str[i - 1]);
        }
    }

    g_print ("Test if this reaches\n");

    return TRUE;
}

void
timeout (GtkWidget *grid)
{
    GtkWidget *label;
    GtkWidget *entry;
    GtkAdjustment *adj;
    gdouble value;
    const gchar *str;

    str = get_timeout ();
    label = gtk_label_new ("Timeout (ms)");
    adj = gtk_adjustment_new (get_initial_value (str), 1.0, 6.0, 1.0, 0, 0);
    entry = gtk_spin_button_new (adj, 0, 0);

    gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (entry),
                                       GTK_UPDATE_IF_VALID);

    gtk_grid_attach (GTK_GRID (grid),
                     label,
                     0, 1, 1, 1);

    gtk_grid_attach_next_to (GTK_GRID (grid),
                             entry,
                             label,
                             GTK_POS_RIGHT,
                             2, 1);

    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_margin_start (entry, 20);
    gtk_widget_set_halign (entry, GTK_ALIGN_FILL);

    g_signal_connect (entry,
                      "value-changed",
                      G_CALLBACK (value_changed_cb),
                      NULL);

    g_signal_connect (entry,
                      "output",
                      G_CALLBACK (output_cb),
                      NULL);

    g_signal_connect (entry,
                      "input",
                      G_CALLBACK (input_cb),
                      NULL);
}
