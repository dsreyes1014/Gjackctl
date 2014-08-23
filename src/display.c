#include "display.h"

GtkWidget *label_dsp;

typedef struct _GtkPassedDisplayData GtkPassedDisplayData;
struct _GtkPassedDisplayData {
    jack_client_t *client;
    GtkWidget *layout;
    GtkWidget *scwindow;
};

static GtkWidget *
gtk_layout_new_with_bg_color_override (GdkRGBA bg_color)
{
    GtkWidget *layout;

    layout = gtk_layout_new (NULL, NULL);

    gtk_widget_override_background_color (layout, GTK_STATE_FLAG_NORMAL, &bg_color);

    return layout;
}

static const gchar *
get_config_setting_string (const gchar *path)
{
    const gchar *string;
    config_t config;
    gchar *file;

    file = g_strconcat (g_getenv ("HOME"), "/.config/gjackctl/gjackctl.conf", NULL);
    config_init (&config);

    config_read_file (&config, file);
    config_lookup_string (&config, path, &string);

    return string;
}

static void
layout_on (GtkWidget *sw, GtkWidget *layout)
{
    GtkWidget *label;
    GtkWidget *labela;
    GtkWidget *label2;
    GtkWidget *label2a;
    GtkWidget *label3;
    GtkWidget *label3a;

    label = gtk_label_new ("CPU Load");
    labela = gtk_label_new (NULL);
    label2 = gtk_label_new ("Sample Rate");
    label2a = gtk_label_new (get_config_setting_string ("gjackctl.driver.sample_rate"));    
    label3 = gtk_label_new ("Driver");
    label3a = gtk_label_new (get_config_setting_string ("gjackctl.driver.type"));

    //dsp_init (sw, labela, client);

    gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold 12"));
    gtk_widget_override_font (label2, pango_font_description_from_string ("Cantarell Bold 12"));
    gtk_widget_override_font (label3, pango_font_description_from_string ("Cantarell Bold 12"));

    gtk_layout_put (GTK_LAYOUT (layout), label, 310, 16);
    gtk_layout_put (GTK_LAYOUT (layout), labela, 360, 16);
    gtk_layout_put (GTK_LAYOUT (layout), label2, 120, 14);
    gtk_layout_put (GTK_LAYOUT (layout), label2a, 216, 16);
    gtk_layout_put (GTK_LAYOUT (layout), label3, 10, 15);
    gtk_layout_put (GTK_LAYOUT (layout), label3a, 60, 16);
}

static void
layout_off (GtkWidget *layout)
{
    GtkWidget *label;

    label = gtk_label_new ("Server not running\n    Nothing to display");

    gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold Italic 14"));

    gtk_layout_put (GTK_LAYOUT (layout), label, 170, 36);
}

static void
switch_pos_cb (GtkSwitch *sw, GParamSpec *pspec, gpointer user_data)
{
    GtkWidget *layout;
    GdkRGBA color = {0.0, 0.0, 0.0, 0.5};
    gboolean check;
    GtkPassedDisplayData *rdata;

    rdata = user_data;
    check = gtk_switch_get_active (GTK_SWITCH (sw));

    gtk_widget_destroy (rdata -> layout);

    if (check == TRUE)
    {
        layout = gtk_layout_new_with_bg_color_override (color);
        layout_on (GTK_WIDGET (sw), layout);
        rdata -> layout = layout;
    }
    else
    {
        layout = gtk_layout_new_with_bg_color_override (color);
        layout_off (layout);
        rdata -> layout = layout;
    }
    
    gtk_container_add (GTK_CONTAINER (rdata -> scwindow), layout);
    gtk_widget_show_all (layout);
}

void
display (GtkWidget *stack, GtkWidget *sw)
{
	GtkWidget *layout;
	GtkWidget *scwindow;
	GdkRGBA bg_color = {0.0, 0.0, 0.0, 0.5};
    GtkPassedDisplayData *pdata;

    pdata = g_malloc (sizeof (GtkPassedDisplayData));
    pdata -> scwindow = gtk_scrolled_window_new (NULL, NULL);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (pdata -> scwindow), 
									GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	/* Pack `header_bar` & `sc_window`. */
    if (gtk_switch_get_active (GTK_SWITCH (sw)) == TRUE)
    {
        layout = gtk_layout_new_with_bg_color_override (bg_color);
        layout_on (sw, layout);
        pdata -> layout = layout;
    }
	else
    {
        layout = gtk_layout_new_with_bg_color_override (bg_color);
        layout_off (layout);
        pdata -> layout = layout;
    }

    gtk_container_add (GTK_CONTAINER (pdata -> scwindow), layout);
	gtk_stack_add_titled (GTK_STACK (stack), pdata -> scwindow, "display", "Display");

    g_signal_connect (sw,
                      "notify::active", 
                   	  G_CALLBACK (switch_pos_cb),
                      pdata);
}
