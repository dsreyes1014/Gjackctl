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
server_name (GtkWidget *box, GtkWidget *button)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */

    GtkWidget *label;
	GtkWidget *entry;
    GtkWidget *child_box;
	const gchar *name;
	config_t config;

    label = gtk_label_new ("Name");
	entry = gtk_entry_new ();	
	name = get_name (config);
    child_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	
	if (!name)
	{
		config_destroy (&config);
	}
	
    gtk_entry_set_text (GTK_ENTRY (entry), name);	
    gtk_entry_set_placeholder_text (GTK_ENTRY (entry), "default");

	gtk_widget_set_tooltip_text (entry, "Enter the name of the JACK server");	
	gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold 11.5"));    

    gtk_box_pack_start (GTK_BOX (child_box), label, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box), entry, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), child_box, FALSE, FALSE, 2);

    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_halign (entry, GTK_ALIGN_START);
    gtk_widget_set_margin_start (label, 20);
    gtk_widget_set_margin_start (entry, 20);
    gtk_widget_set_margin_top (label, 6);

	g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), entry);
}
