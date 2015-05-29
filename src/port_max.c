#include "port_max.h"

static const gchar *
get_port_max ()
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
    config_lookup_string (&config, "gjackctl.server.port_max", &string);

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

    config_file_input ("gjackctl.server.port_max",
                       CONFIG_TYPE_STRING,
                       (gpointer) str);

    g_free (str);
}

void
port_max (GtkWidget *grid)
{
    GtkWidget *label;
    GtkAdjustment *adj;
    gdouble k;
    const gchar * str;

    str = get_port_max ();
    k = g_ascii_strtod (str, NULL);
    GtkWidget *spin_button;
    label = gtk_label_new ("Max Port Count");
    adj = gtk_adjustment_new (k, 128, 2048, 1, 0, 0);
    spin_button = gtk_spin_button_new (adj, 1, 0);

    gtk_grid_attach (GTK_GRID (grid),
                     label,
                     0, 2, 1, 1);

    gtk_grid_attach_next_to (GTK_GRID (grid),
                             spin_button,
                             label,
                             GTK_POS_RIGHT,
                             1, 1);

    //gtk_widget_set_margin_start (label, 10);
    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_margin_start (spin_button, 20);
    gtk_widget_set_halign (spin_button, GTK_ALIGN_FILL);

    gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON (spin_button),
                                       GTK_UPDATE_IF_VALID);

    g_signal_connect (spin_button,
                      "value-changed",
                      G_CALLBACK (value_changed_cb),
                      &k);
}
