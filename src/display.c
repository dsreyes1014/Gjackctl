#include "display.h"

typedef struct _GtkPassedDisplayData GtkPassedDisplayData;

struct _GtkPassedDisplayData {
    GtkWidget *grid;
    GtkWidget *stack;
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
    
    g_free (file);

    return string;
}

static gboolean
grid_on (GtkWidget *sw, GtkWidget *grid, gpointer user_data)
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
    GtkPassedDisplayData *rdata;

    label = gtk_label_new ("CPU Load");
    labela = gtk_label_new (NULL);
    label2 = gtk_label_new ("Sample Rate");
    label2a = gtk_label_new (NULL);    
    label3 = gtk_label_new ("Driver");
    label3a = gtk_label_new (get_config_setting_string ("gjackctl.driver.type"));
    label4 = gtk_label_new ("Device");
    label4a = gtk_label_new (get_config_setting_string ("gjackctl.driver.device"));
    level_bar = gtk_level_bar_new ();    
    rdata = user_data;
    
    gtk_level_bar_set_min_value (GTK_LEVEL_BAR (level_bar), 0);
    gtk_level_bar_set_max_value (GTK_LEVEL_BAR (level_bar), 100);
    gtk_level_bar_set_mode (GTK_LEVEL_BAR (level_bar),
                            GTK_LEVEL_BAR_MODE_CONTINUOUS);

    if (jack_client_init (sw,
                          rdata -> stack,
                          labela,
                          label2a,
                          level_bar) != 0)
    {
        return FALSE;
    }

    
    
    g_print ("display.c: line 66\n");

    gtk_widget_set_tooltip_text (level_bar, "CPU Load");

    gtk_grid_attach (GTK_GRID (grid), level_bar, 0, 0, 7, 1);
    gtk_grid_attach_next_to (GTK_GRID (grid),
                             labela,
                             level_bar,
                             GTK_POS_RIGHT,
                             1,
                             1); 

    gtk_grid_attach (GTK_GRID (grid), label3, 0, 1, 1, 1); 
    gtk_grid_attach_next_to (GTK_GRID (grid),
                             label3a,
                             label3,
                             GTK_POS_RIGHT,
                             1,
                             1);

    gtk_grid_attach (GTK_GRID (grid), label4, 3, 1, 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (grid),
                             label4a,
                             label4,
                             GTK_POS_RIGHT,
                             1,
                             1);

    gtk_grid_attach (GTK_GRID (grid), label2, 5, 1, 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (grid),
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

    gtk_widget_show_all (grid);

    return TRUE;
}

static void
grid_off (GtkWidget *stack, GtkWidget *grid, gpointer user_data)
{
    GList *children, *iter;
    GtkPassedDisplayData *rdata;
    GtkWidget *stack_child;

    children = gtk_container_get_children (GTK_CONTAINER (grid));
    rdata = user_data;

    stack_child = gtk_stack_get_child_by_name (GTK_STACK (rdata -> stack), "ports");    
    
    for (iter = children; iter != NULL; iter = g_list_next (iter))
    {
        gtk_widget_destroy (iter -> data);
    }

    g_list_free (children);
    
    if (stack_child != NULL)
    {
        gtk_widget_destroy (stack_child);
    }
}

static void
switch_pos_cb (GtkSwitch *sw, GParamSpec *pspec, gpointer user_data)
{
    /* 
        Here we destroy the 'GtkWidget *grid' passed through by the struct
        and create a new one for on/off functionality.
    */
     
    GtkWidget *stack_child;
    GtkPassedDisplayData *rdata;
    
    rdata = user_data;
    stack_child = gtk_stack_get_child_by_name (GTK_STACK (rdata -> stack), "log");

    if (gtk_switch_get_active (sw) == TRUE)
    {
        g_object_ref (stack_child);
        gtk_container_remove (GTK_CONTAINER (rdata -> stack), stack_child);

        if (grid_on (GTK_WIDGET (sw), rdata -> grid, rdata) == FALSE)
        {
            grid_off (rdata -> stack, rdata -> grid, rdata);
        }

        gtk_stack_add_titled (GTK_STACK (rdata -> stack),
                              stack_child,
                              "log",
                              "Log");

        g_object_unref (stack_child);
    }
    else
    {
        grid_off (rdata -> stack, rdata -> grid, rdata);
    }

    gtk_widget_show_all (rdata -> grid);
}

void
display (GtkWidget *stack, GtkWidget *sw)
{
	GtkWidget *scwindow;
    GtkWidget *grid;
    GtkPassedDisplayData *pdata;
    GtkWidget *stack_child;
    GtkWidget *stack_child2;

    pdata = g_slice_new (GtkPassedDisplayData);
    grid = gtk_grid_new ();
    pdata -> stack = stack;
    stack_child = NULL;

    if (gtk_switch_get_active (GTK_SWITCH (sw)) == TRUE)
    {
        grid_on (sw, grid, pdata);
        pdata -> grid = grid;

        /* Here we remove the 'Ports' tab of our stack so that the 'Display'
        tab our stack shows first. */ 
        stack_child = gtk_stack_get_child_by_name (GTK_STACK (stack), "ports");
        g_object_ref (stack_child);
        gtk_container_remove (GTK_CONTAINER (stack), stack_child); 
    }
	else
    {
        grid_off (pdata -> stack, grid, pdata);
        pdata -> grid = grid;
    }

	gtk_stack_add_titled (GTK_STACK (stack),
                          grid,
                          "display", 
                          "Display");

    g_signal_connect (sw,
                      "notify::active",
                      G_CALLBACK (switch_pos_cb),
                      pdata);

    if (stack_child != NULL)
    {
        /* Here we add the 'Ports' tab of our stack back into the stack 
        and 'unref' it so it will be released when our app is destroyed. */
        gtk_stack_add_titled (GTK_STACK (stack), stack_child, "ports", "Ports");
        g_object_unref (stack_child);
    }
}
