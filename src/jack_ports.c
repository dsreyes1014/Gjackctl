#include "jack_ports.h"

static gchar **
modified_ports_array (const gchar **ports_array)
{
    gchar **array;
    gchar *dup;
    gchar *modded_dup;
    gchar *modded_dup_copy;
    gint i;
    gint j;
    gint k;

    i = 0;
    j = 0;
    k = 0;
    dup = NULL;
    modded_dup = NULL;
    modded_dup_copy = NULL;
    array = (gchar **) g_malloc (16 * sizeof (gchar *));

    while (ports_array[i])
    {
        dup = g_strdup (ports_array[i]);
        j = 0;

        while (dup[j])
        {
            if (dup[j] == ':')
            {
                modded_dup = g_strndup (dup, j);
                break;
            }

            j++;
        }

        g_free (dup);

        if (g_strcmp0 (modded_dup_copy, modded_dup) != 0)
        {
            g_free (modded_dup_copy);

            modded_dup_copy = g_strdup (modded_dup);
            array[k] = g_strdup (modded_dup_copy);
            k++;
        }

        g_free (modded_dup);
        i++;
    }

    array[k] = NULL;

    g_free (modded_dup_copy);

    return array;
}


static void
audio_from_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GMenu *from_section;
    jack_client_t *client;
    jack_port_t *jack_port;
    const gchar **ports_array;
    gchar **modified_array;
    gint i;

    i = 0;
    menu = g_menu_new ();
    from_section = g_menu_new ();
    client = user_data;
    ports_array = jack_get_ports (client,
                                  NULL,
                                  JACK_DEFAULT_AUDIO_TYPE,
                                  JackPortIsOutput);

    modified_array = modified_ports_array (ports_array);

    while (modified_array[i])
    {
        GMenuItem *item;

        item = g_menu_item_new (modified_array[i], NULL);
        g_menu_append_item (G_MENU (from_section), item);

        i++;
    }

    jack_free (ports_array);
    g_strfreev (modified_array);

    g_menu_insert_section (menu,
                           0,
                           "Source",
                           G_MENU_MODEL (from_section));

    popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                          G_MENU_MODEL (menu));

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);

    gtk_widget_show_all (popover);
}

static void
audio_to_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GMenu *to_section;
    jack_client_t *client;
    jack_port_t *jack_port;
    const gchar **ports_array;
    gchar **modified_array;
    gint i;

    i = 0;
    menu = g_menu_new ();
    to_section = g_menu_new ();
    client = user_data;
    ports_array = jack_get_ports (client,
                                  NULL,
                                  JACK_DEFAULT_AUDIO_TYPE,
                                  JackPortIsInput);

    modified_array = modified_ports_array (ports_array);

    while (modified_array[i])
    {
        GMenuItem *item;

        item = g_menu_item_new (modified_array[i], NULL);
        g_menu_append_item (G_MENU (to_section), item);

        i++;
    }

    jack_free (ports_array);
    g_strfreev (modified_array);

    g_menu_insert_section (menu,
                           0,
                           "Destination",
                           G_MENU_MODEL (to_section));

    popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                          G_MENU_MODEL (menu));

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);

    gtk_widget_show_all (popover);
}

static void
midi_from_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GMenu *from_section;
    jack_client_t *client;
    jack_port_t *jack_port;
    const gchar **ports_array;
    gchar **modified_array;
    gint i;

    i = 0;
    menu = g_menu_new ();
    from_section = g_menu_new ();
    client = user_data;
    ports_array = jack_get_ports (client,
                                  NULL,
                                  JACK_DEFAULT_MIDI_TYPE,
                                  JackPortIsOutput);

    modified_array = modified_ports_array (ports_array);

    while (modified_array[i])
    {
        GMenuItem *item;

        item = g_menu_item_new (modified_array[i], NULL);
        g_menu_append_item (G_MENU (from_section), item);

        i++;
    }

    jack_free (ports_array);
    g_strfreev (modified_array);

    g_menu_insert_section (menu,
                           0,
                           "Source",
                           G_MENU_MODEL (from_section));

    popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                          G_MENU_MODEL (menu));

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);

    gtk_widget_show_all (popover);
}

static void
midi_to_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GMenu *to_section;
    jack_client_t *client;
    jack_port_t *jack_port;
    const gchar **ports_array;
    gchar **modified_array;
    gint i;

    i = 0;
    menu = g_menu_new ();
    to_section = g_menu_new ();
    client = user_data;
    ports_array = jack_get_ports (client,
                                  NULL,
                                  JACK_DEFAULT_MIDI_TYPE,
                                  JackPortIsInput);

    modified_array = modified_ports_array (ports_array);

    while (modified_array[i])
    {
        GMenuItem *item;

        item = g_menu_item_new (modified_array[i], NULL);
        g_menu_append_item (G_MENU (to_section), item);

        i++;
    }

    jack_free (ports_array);
    g_strfreev (modified_array);

    g_menu_insert_section (menu,
                           0,
                           "Destination1",
                           G_MENU_MODEL (to_section));

    popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                          G_MENU_MODEL (menu));

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);

    gtk_widget_show_all (popover);
}

/*static gint
create_port (const gchar *port_string,
             gint row,
             GtkPassedJackPortsData *rdata)
{
    GtkWidget *port_button;
    ClutterActor *actor;
    ClutterLayoutManager *layout;
    jack_port_t *port;
    const gchar *port_type;

    port = jack_port_by_name (rdata -> client, port_string);
    port_type = jack_port_type (port);
    port_button = gtk_button_new_with_label (jack_port_short_name (port));
    //actor = gtk_clutter_actor_new_with_contents (port_button);
    //layout = clutter_actor_get_layout_manager (rdata -> audio_stage);

    g_print ("jack_ports.c: %s\n", port_type);

    //clutter_actor_add_action (actor, clutter_drag_action_new ());
    //clutter_actor_set_reactive (actor, TRUE);

    //clutter_actor_add_child (rdata -> audio_stage, actor);

    return 0;
}*/

/*static void
refresh_ports (GtkWidget *button, gpointer user_data)
{
    const gchar **port_list;
    gint i, j, k;
    GtkPassedJackPortsData *rdata;
    ClutterLayoutManager *flow;

    rdata = user_data;
    i = 0;
    j = 0;

    //flow = clutter_actor_get_layout_manager (rdata -> audio_stage);

    port_list = jack_get_ports (rdata -> client,
                                NULL,
                                NULL,
                                0);

    g_object_ref (rdata -> refresh_actor);

    clutter_actor_remove_child (rdata -> audio_stage, rdata -> refresh_actor);
    clutter_actor_destroy_all_children (rdata -> audio_stage);

    //clutter_grid_layout_attach (CLUTTER_GRID_LAYOUT (grid),
                                //rdata -> actor,
                                //0, 0, 1, 1);

    clutter_actor_add_child (rdata -> audio_stage, rdata -> refresh_actor);

    g_object_unref (rdata -> refresh_actor);

    while (port_list[i++]);

    for (j = 0; j < i - 1; j++)
    {
        create_port (port_list[j], j, rdata);

        g_print ("jack_ports.c: %s\n", port_list[j]);
    }
}*/

gint
jack_ports (GtkWidget *stack, jack_client_t *client)
{
    GtkWidget *notebook;
    GtkWidget *audio_grid;
    GtkWidget *midi_grid;
    GtkWidget *audio_from_button;
    GtkWidget *audio_to_button;
    GtkWidget *midi_from_button;
    GtkWidget *midi_to_button;
    GtkWidget *to_button;
    GtkWidget *audio_label;
    GtkWidget *midi_label;
    gint i, j;
    jack_port_t *port;

    i = 0;
    j = 0;
    notebook = gtk_notebook_new ();
    audio_grid = gtk_grid_new ();
    midi_grid = gtk_grid_new ();
    audio_label = gtk_label_new ("Audio");
    midi_label = gtk_label_new ("MIDI");
    audio_from_button = gtk_button_new_with_label ("From");
    audio_to_button = gtk_button_new_with_label ("To");
    midi_from_button = gtk_button_new_with_label ("From");
    midi_to_button = gtk_button_new_with_label ("To");
    to_button = gtk_button_new_with_label ("To");

    gtk_button_set_relief (GTK_BUTTON (audio_from_button), GTK_RELIEF_NONE);
    gtk_button_set_relief (GTK_BUTTON (audio_to_button), GTK_RELIEF_NONE);
    gtk_button_set_relief (GTK_BUTTON (midi_from_button), GTK_RELIEF_NONE);
    gtk_button_set_relief (GTK_BUTTON (midi_to_button), GTK_RELIEF_NONE);

    g_signal_connect (audio_from_button,
                      "clicked",
                      G_CALLBACK (audio_from_button_clicked_cb),
                      client);

    g_signal_connect (audio_to_button,
                      "clicked",
                      G_CALLBACK (audio_to_button_clicked_cb),
                      client);

    g_signal_connect (midi_from_button,
                      "clicked",
                      G_CALLBACK (midi_from_button_clicked_cb),
                      client);

    g_signal_connect (midi_to_button,
                      "clicked",
                      G_CALLBACK (midi_to_button_clicked_cb),
                      client);

    //g_signal_connect (refresh_button, "clicked", G_CALLBACK (refresh_ports), pdata);

    gtk_widget_set_margin_start (audio_from_button, 30);

    gtk_grid_attach (GTK_GRID (audio_grid),
                     audio_from_button,
                     0, 0, 1, 1);

    gtk_grid_attach (GTK_GRID (audio_grid),
                     audio_to_button,
                     1, 0, 1, 1);

    gtk_grid_attach (GTK_GRID (midi_grid),
                     midi_from_button,
                     0, 0, 1, 1);

    gtk_grid_attach (GTK_GRID (midi_grid),
                     midi_to_button,
                     1, 0, 1, 1);

    gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
                              audio_grid,
                              audio_label);

    gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
                              midi_grid,
                              midi_label);

    gtk_stack_add_titled (GTK_STACK (stack),
                          notebook,
                          "ports",
                          "Ports");

    gtk_widget_show_all (stack);

    return 0;
}
