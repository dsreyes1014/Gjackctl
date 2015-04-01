#include "rt_priority.h"

typedef struct _GtkPassedRtData GtkPassedRtData;

struct _GtkPassedRtData {
    GtkWidget *spin_button;
    config_t config;
};

const gchar *
get_priority ()
{
    const gchar *priority;
    gchar *file;
    config_t config;

    config_init (&config);
    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

    config_read_file (&config, file);
    config_lookup_string (&config, "gjackctl.server.priority", &priority);

    g_free (file);

    return priority;
}

static void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkPassedRtData *rdata;
    gint priority;
    gchar string[10];

    rdata = user_data;
    priority = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (rdata -> spin_button));        
    g_sprintf (string, "%d", priority);

    config_file_input ("gjackctl.server.priority",
                       CONFIG_TYPE_STRING,
                       (gpointer) string); 

    g_slice_free (GtkPassedRtData, rdata);
}

void
rt_priority (GtkWidget *box, GtkWidget *button)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */

	GtkWidget *label;
	GtkWidget *spin_button;
    GtkWidget *child_box;
	GtkAdjustment *adjustment;
    gint priority;
    GtkPassedRtData *pdata;

    pdata = g_slice_new (GtkPassedRtData);        

    label = gtk_label_new ("Priority");
	adjustment = gtk_adjustment_new (75, 0, 99, 1, 0, 0);
	pdata -> spin_button = gtk_spin_button_new (adjustment, 1, 0);
    priority = g_ascii_strtoll (get_priority (), NULL, 0);
    child_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);

    gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold 11.5"));
    gtk_spin_button_set_value (GTK_SPIN_BUTTON (pdata -> spin_button), priority);

	/* Pack `box` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
    gtk_box_pack_start (GTK_BOX (child_box), label, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (child_box), pdata -> spin_button, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), child_box, FALSE, FALSE, 2);
	
    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_halign (pdata -> spin_button, GTK_ALIGN_START);
    gtk_widget_set_margin_start (label, 40);
    gtk_widget_set_margin_start (pdata -> spin_button, 40);
    gtk_widget_set_margin_top (label, 30);

    g_signal_connect (button, "clicked", G_CALLBACK (button_clicked_cb), pdata);
}
