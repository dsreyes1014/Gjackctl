#include "toggle_midi.h"

static gboolean
get_midi ()
{
	gboolean value;
	gchar *file;
    config_t config;

	file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

	config_init (&config);
	config_read_file (&config, file);

    if (config_lookup_bool (&config, "gjackctl.server.midi", &value) == CONFIG_FALSE)
    {
        config_setting_t *group;
        config_setting_t *setting;

        g_print ("\'MIDI\' config option not available\n");
        g_print ("Creating config setting now...\n");

        group = config_lookup (&config, "gjackctl.server");

        setting = config_setting_add (group, "midi", CONFIG_TYPE_BOOL);
        config_setting_set_bool (setting, TRUE);
        config_write_file (&config, file);
    }

	if (value == FALSE)
	{
		config_destroy (&config);
        g_free (file);

		return FALSE;
	}
	else
    {
        config_destroy (&config);
        g_free (file);

        return TRUE;
   	}
}

static void
toggle_cb (GtkToggleButton *tb,
           gpointer         user_data)
{
    gboolean value;

    value = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (tb));

    config_file_input ("gjackctl.server.midi",
                       CONFIG_TYPE_BOOL,
                       (gpointer) &value);
}

void
toggle_midi (GtkWidget *grid)
{
    GtkWidget *cbutton;

    cbutton = gtk_check_button_new_with_label ("MIDI");

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cbutton),
                                  get_midi ());

    gtk_grid_attach (GTK_GRID (grid),
                     cbutton,
                     0, 1, 1, 1);

    gtk_widget_set_margin_top (cbutton, 4);
    gtk_widget_set_margin_start (cbutton, 14);

    g_signal_connect (cbutton, "toggled", G_CALLBACK (toggle_cb), NULL);
}
