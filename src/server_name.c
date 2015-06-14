#include "server_name.h"

static const gchar *
get_server_name ()
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
    if (config_lookup_string (&config, "gjackctl.server.name", &string) == CONFIG_FALSE)
    {
        gchar *value_copy;
        config_setting_t *group;
        config_setting_t *setting;

        g_print ("\'Timeout\' config option not available\n");
        g_print ("Creating config setting now...\n");

        value_copy = g_strdup ("500");
        group = config_lookup (&config, "gjackctl.server");
        setting = config_setting_add (group, "name", CONFIG_TYPE_STRING);
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

static gboolean
focus_out_event_cb (GtkWidget *widget,
                    GdkEvent  *event,
                    gpointer   user_data)
{
    config_file_input ("gjackctl.server.name",
                       CONFIG_TYPE_STRING,
                       (gpointer) gtk_entry_get_text (GTK_ENTRY (widget)));

    return FALSE;
}

void
server_name (GtkWidget *grid)
{
    GtkWidget *label;
	GtkWidget *entry;
	const gchar *name;

    label = gtk_label_new ("Name");
	entry = gtk_entry_new ();
	name = get_server_name ();

    gtk_entry_set_text (GTK_ENTRY (entry), name);
    gtk_entry_set_placeholder_text (GTK_ENTRY (entry), "default");

	gtk_widget_set_tooltip_text (entry, "Enter the name of the JACK server");

    gtk_grid_attach (GTK_GRID (grid),
                     label,
                     0, 0, 1, 1);

    gtk_grid_attach_next_to (GTK_GRID (grid),
                             entry,
                             label,
                             GTK_POS_RIGHT,
                             1, 1);

    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_margin_start (entry, 20);
    gtk_widget_set_halign (entry, GTK_ALIGN_FILL);

    g_signal_connect (entry, "focus-out-event", G_CALLBACK (focus_out_event_cb), NULL);
}
