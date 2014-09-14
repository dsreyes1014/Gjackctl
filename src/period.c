#include "period.h"

static const gchar *
get_period (config_t config)
{
    const gchar *period;
    gchar *file;

    config_init (&config);
    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

    config_read_file (&config, file);
    config_lookup_string (&config, "gjackctl.driver.period", &period);

    return period;
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

void
period (GtkWidget *box, GtkWidget *button)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */

	GtkWidget *label;
	GtkWidget *spin_button;
    GtkWidget *child_box;
	GtkAdjustment *adjustment;
    gint period;
    config_t config;

    label = gtk_label_new ("Period");
	adjustment = gtk_adjustment_new (2, 0, 6, 1, 0, 0);
	spin_button = gtk_spin_button_new (adjustment, 1, 0);
    period = g_ascii_strtoll (get_period (config), NULL, 0);
    child_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);

    gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold 11.5"));
    gtk_widget_set_tooltip_text (spin_button, "Number of periods of latency");

    gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_button), period);

	/* Pack `box` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
    gtk_box_pack_start (GTK_BOX (child_box), label, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box), spin_button, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), child_box, FALSE, FALSE, 2);
	
    //gtk_widget_set_halign (label, GTK_ALIGN_START);
    //gtk_widget_set_halign (spin_button, GTK_ALIGN_START);
    gtk_widget_set_margin_start (label, 10);
    gtk_widget_set_margin_start (spin_button, 10);
    //gtk_widget_set_margin_end (spin_button, 70);

    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), spin_button);
}
