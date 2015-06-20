#include "display.h"

typedef struct _GtkPassedDisplayData GtkPassedDisplayData;

struct _GtkPassedDisplayData {
    GtkWidget *grid;
    GtkWidget *pbar;
    GtkPassedMainData *pdata;
    gint handler_id;
};

static const gchar *
get_config_setting_string (const gchar *path)
{
    const gchar *string;
    config_t config;
    gchar *file;
    const gchar *string_copy;

    file = g_strconcat (g_getenv ("HOME"), "/.config/gjackctl/gjackctl.conf", NULL);
    config_init (&config);

    config_read_file (&config, file);
    config_lookup_string (&config, path, &string);
    string_copy = g_strdup (string);
    
    /* Clean up after ourselves. */
    g_free (file);
    config_destroy (&config);

    return string_copy;
}

static gint
clear_container (GtkWidget *container)
{
    GList *children, *iter;

    children = gtk_container_get_children (GTK_CONTAINER (container));

    for (iter = children; iter != NULL; iter = g_list_next (iter))
    {
        gtk_widget_destroy (iter -> data);
    }

    g_list_free (children);
    g_list_free (iter);

    return 0;
}

static gboolean
grid_on (GtkPassedDisplayData *rdata)
{
    GtkWidget *label;
    GtkWidget *labela;
    GtkWidget *label2;
    GtkWidget *label2a;
    GtkWidget *label3;
    GtkWidget *label3a;
    GtkWidget *label4;
    GtkWidget *label4a;
    GtkWidget *level_bar;

    label = gtk_label_new ("CPU Load");
    labela = gtk_label_new (NULL);
    label2 = gtk_label_new ("Sample Rate");
    label2a = gtk_label_new (NULL);    
    label3 = gtk_label_new ("Driver");
    label3a = gtk_label_new (get_config_setting_string ("gjackctl.driver.type"));
    label4 = gtk_label_new ("Device");
    label4a = gtk_label_new (get_config_setting_string ("gjackctl.driver.device"));
    level_bar = gtk_level_bar_new ();    

    //clear_container (rdata -> grid);

    gtk_level_bar_set_min_value (GTK_LEVEL_BAR (level_bar), 0);
    gtk_level_bar_set_max_value (GTK_LEVEL_BAR (level_bar), 100);
    gtk_level_bar_set_mode (GTK_LEVEL_BAR (level_bar),
                            GTK_LEVEL_BAR_MODE_CONTINUOUS);

    if (jack_client_init (rdata -> pdata -> client,
                          rdata -> pdata -> sw,
                          rdata -> pdata -> button_box,
                          labela,
                          label2a,
                          level_bar,
                          rdata -> pdata -> text) != 0)
    {
        return FALSE;
    }

    gtk_widget_set_tooltip_text (level_bar, "CPU Load");

    gtk_grid_attach (GTK_GRID (rdata -> grid), level_bar, 0, 0, 7, 1);
    gtk_grid_attach_next_to (GTK_GRID (rdata -> grid),
                             labela,
                             level_bar,
                             GTK_POS_RIGHT,
                             1,
                             1); 

    gtk_grid_attach (GTK_GRID (rdata -> grid), label3, 0, 1, 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (rdata -> grid),
                             label3a,
                             label3,
                             GTK_POS_RIGHT,
                             1,
                             1);

    gtk_grid_attach (GTK_GRID (rdata -> grid), label4, 3, 1, 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (rdata -> grid),
                             label4a,
                             label4,
                             GTK_POS_RIGHT,
                             1,
                             1);

    gtk_grid_attach (GTK_GRID (rdata -> grid), label2, 5, 1, 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (rdata -> grid),
                             label2a,
                             label2,
                             GTK_POS_RIGHT,
                             1,
                             1);

    gtk_widget_set_margin_top (level_bar, 6);
    gtk_widget_set_margin_start (level_bar, 34); 
    gtk_widget_set_margin_bottom (level_bar, 4); 
    gtk_widget_set_margin_start (label3, 34);
    gtk_widget_set_margin_start (label3a, 4);
    gtk_widget_set_margin_start (label4, 26);
    gtk_widget_set_margin_start (label4a, 4);
    gtk_widget_set_margin_start (label2, 26);
    gtk_widget_set_margin_start (label2a, 4);
    gtk_widget_set_margin_start (labela, 4);

    //gtk_widget_set_vexpand (level_bar, TRUE);
    gtk_widget_set_halign (level_bar, GTK_ALIGN_FILL);

    gtk_widget_show_all (rdata -> grid);

    return TRUE;
}

static gboolean
pulse_timeout_cb (gpointer user_data)
{
    gboolean value;
    GtkPassedDisplayData *rdata;

    rdata = user_data;
    value = gtk_switch_get_active (GTK_SWITCH (rdata -> pdata -> sw));

    if (value == TRUE)
    {
        return FALSE;
    }
    else
    {
        gtk_progress_bar_set_pulse_step (GTK_PROGRESS_BAR (rdata -> pbar),
                                         0.3);

        gtk_progress_bar_pulse (GTK_PROGRESS_BAR (rdata -> pbar));
        return TRUE;
    }
}

static void
grid_off (GtkPassedDisplayData *rdata)
{
    clear_container (rdata -> grid);
}

static void
switch_pos_cb (GtkSwitch *sw, GParamSpec *pspec, gpointer user_data)
{
    /*
     * Here we destroy the 'GtkWidget *grid' passed through by the struct
     * and create a new one for on/off functionality.
     */

    GtkWidget *stack_child;
    GtkPassedDisplayData *rdata;

    rdata = user_data;

    if (gtk_switch_get_active (sw) == TRUE)
    {
        if (grid_on (rdata) == FALSE)
        {
            grid_off (rdata);
        }
    }
    else
    {
        grid_off (rdata);
    }

    gtk_widget_show_all (rdata -> grid);
}

void
display (GtkPassedMainData *pdata)
{
    GtkPassedDisplayData *rdata;

    rdata = g_slice_new (GtkPassedDisplayData);
    rdata -> grid = gtk_grid_new ();
    rdata -> pdata = pdata;

    if (gtk_switch_get_active (GTK_SWITCH (rdata -> pdata -> sw)) == TRUE)
    {
        grid_on (rdata);
    }
	else
    {
        grid_off (rdata);
    }

    gtk_box_pack_start (GTK_BOX (pdata -> vbox),
                        rdata -> grid,
                        FALSE,
                        FALSE,
                        2);

    g_signal_connect (pdata -> sw,
                      "notify::active",
                      G_CALLBACK (switch_pos_cb),
                      rdata);
}
