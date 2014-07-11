#include "server_name.h"

static const gchar *
get_name (config_t config)
{
	const gchar *name;
	gchar config_file[128];

	g_sprintf (config_file, "%s/.config/gjackctl/gjackctl.conf", g_getenv ("HOME"));

	config_init (&config);
	config_read_file (&config, config_file);
	
	config_lookup_string (&config, "gjackctl.server.name", &name);

	if (name == NULL)
	{
		config_destroy (&config);

		return NULL;
	}
	else if (g_strcmp0 (name, "default") == 0)
	{
		config_destroy (&config);

		return "";
	}

	return name;
}

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
	GtkWidget *entry;

	entry = user_data;

	config_file_input1 ("gjackctl.server.name", 
                        CONFIG_TYPE_STRING, 
                        (gpointer) gtk_entry_get_text (GTK_ENTRY (entry)));
}

void
server_name (GtkWidget *grid, GtkWidget *button)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */

	GtkWidget *label_name;
	GtkWidget *entry;
	const gchar *name;
	config_t config;

	label_name = gtk_label_new ("Name");
	entry = gtk_entry_new ();	
	name = get_name (config);
	
	if (!name)
	{
		config_destroy (&config);
	}
	
	gtk_entry_set_placeholder_text (GTK_ENTRY (entry), "default");
	gtk_widget_set_tooltip_text (entry, "Enter the name of the JACK server");	
	gtk_entry_set_text (GTK_ENTRY (entry), name);

	gtk_grid_attach (GTK_GRID (grid), label_name, 0, 0, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), entry, label_name, GTK_POS_RIGHT, 1, 1);

	gtk_widget_set_margin_start (label_name, 50);
	gtk_widget_set_margin_top (entry, 10);
	gtk_widget_set_margin_top (label_name, 5);

	g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), entry);
}
