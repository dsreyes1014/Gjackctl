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

	config_file_input ("gjackctl.server.name", 
                       CONFIG_TYPE_STRING, 
                       (gpointer) gtk_entry_get_text (GTK_ENTRY (entry)));
}

void
server_name (GtkWidget *grid, GtkWidget *button)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */

    GtkWidget *frame;
	GtkWidget *entry;
	const gchar *name;
	config_t config;

    frame = gtk_frame_new ("Name");
	entry = gtk_entry_new ();	
	name = get_name (config);
	
	if (!name)
	{
		config_destroy (&config);
	}
	
	gtk_entry_set_placeholder_text (GTK_ENTRY (entry), "default");
	gtk_widget_set_tooltip_text (entry, "Enter the name of the JACK server");	
	gtk_entry_set_text (GTK_ENTRY (entry), name);    
    gtk_entry_set_max_width_chars (GTK_ENTRY (entry), 10);
    gtk_widget_set_size_request (frame, 10, 20);

    gtk_container_add (GTK_CONTAINER (frame), entry);
	gtk_grid_attach (GTK_GRID (grid), frame, 1, 0, 1, 1);

    gtk_entry_set_width_chars (GTK_ENTRY (entry), 10);

	//gtk_widget_set_margin_start (frame, 10);
	//gtk_widget_set_margin_top (frame, 6);
    //gtk_widget_set_margin_bottom (frame, 10);
	//gtk_widget_set_margin_top (entry, 4);
    gtk_widget_set_margin_bottom (entry, 4);
    gtk_widget_set_margin_start (entry, 6);
    gtk_widget_set_margin_end (entry, 6);
    gtk_widget_set_valign (frame, GTK_ALIGN_CENTER);

	g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), entry);
}
