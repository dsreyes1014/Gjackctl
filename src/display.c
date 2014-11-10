#include "display.h"

typedef struct _GtkPassedDisplayData GtkPassedDisplayData;

struct _GtkPassedDisplayData {
    GtkWidget *layout;
    GtkWidget *scwindow;
};

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

static gboolean
layout_on (GtkWidget *sw, GtkWidget *layout)
{
    GtkWidget *label;
    GtkWidget *labela;
    GtkWidget *label2;
    GtkWidget *label2a;
    GtkWidget *label3;
    GtkWidget *label3a;
    GtkWidget *label4;
    GtkWidget *label4a;

    label = gtk_label_new ("CPU Load");
    labela = gtk_label_new (NULL);
    label2 = gtk_label_new ("Sample Rate");
    label2a = gtk_label_new (NULL);    
    label3 = gtk_label_new ("Driver");
    label3a = gtk_label_new (get_config_setting_string ("gjackctl.driver.type"));
    label4 = gtk_label_new ("Device");
    label4a = gtk_label_new (get_config_setting_string ("gjackctl.driver.device"));

    if (jack_client_init (sw, labela, label2a) != 0)
    {
        //gtk_switch_set_active (GTK_SWITCH (sw), FALSE);
        return FALSE;
    }
    
    gtk_widget_override_font (labela, pango_font_description_from_string ("Cantarell Bold 32"));
    gtk_widget_override_font (label2, pango_font_description_from_string ("Cantarell Bold 12"));
    gtk_widget_override_font (label3, pango_font_description_from_string ("Cantarell Bold 12"));
    gtk_widget_override_font (label4, pango_font_description_from_string ("Cantarell Bold 12"));
    gtk_widget_set_tooltip_text (labela, "CPU Load");

    //gtk_layout_put (GTK_LAYOUT (layout), label, 310, 16);
    gtk_layout_put (GTK_LAYOUT (layout), labela, 220, 42);
    gtk_layout_put (GTK_LAYOUT (layout), label2, 400, 16);
    gtk_layout_put (GTK_LAYOUT (layout), label2a, 496, 16);
    gtk_layout_put (GTK_LAYOUT (layout), label3, 10, 16);
    gtk_layout_put (GTK_LAYOUT (layout), label3a, 60, 16);
    gtk_layout_put (GTK_LAYOUT (layout), label4, 110, 16);
    gtk_layout_put (GTK_LAYOUT (layout), label4a, 164, 16);
    
    return TRUE;
}

static void
layout_off (GtkWidget *layout)
{
    GtkWidget *label;

    label = gtk_label_new ("");

    gtk_widget_override_font (label, pango_font_description_from_string ("Cantarell Bold Italic 14"));

    gtk_layout_put (GTK_LAYOUT (layout), label, 170, 40);
}

static void
switch_pos_cb (GtkSwitch *sw, GParamSpec *pspec, gpointer user_data)
{
    GtkWidget *layout;
    GtkPassedDisplayData *rdata;
    
    rdata = user_data;
    layout = gtk_layout_new (NULL, NULL);

    gtk_widget_destroy (rdata -> layout);

    if (gtk_switch_get_active (sw) == TRUE)
    {
        if (layout_on (GTK_WIDGET (sw), layout) == FALSE)
        {
            //gtk_switch_set_active (sw, FALSE);
            layout_off (layout);
        }

        rdata -> layout = layout;
    }
    else
    {
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
    GtkPassedDisplayData *pdata;

    pdata = g_slice_new (GtkPassedDisplayData);
    pdata -> scwindow = gtk_scrolled_window_new (NULL, NULL);
    layout = gtk_layout_new (NULL, NULL);

	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (pdata -> scwindow), 
									GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	/* Pack `header_bar` & `sc_window`. */
    if (gtk_switch_get_active (GTK_SWITCH (sw)) == TRUE)
    {
        layout_on (sw, layout);
        pdata -> layout = layout;
    }
	else
    {
        layout_off (layout);
        pdata -> layout = layout;
    }

    gtk_container_add (GTK_CONTAINER (pdata -> scwindow), pdata -> layout);

	gtk_stack_add_titled (GTK_STACK (stack), pdata -> scwindow,
                          "display", 
                          "Display");

    g_signal_connect_after (sw, "notify::active", G_CALLBACK (switch_pos_cb), pdata);
}
