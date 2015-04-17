#include "jack_ports.h"

typedef struct _GtkPassedJackPortsData GtkPassedJackPortsData;

struct _GtkPassedJackPortsData {
    const gchar **string;
    jack_client_t *client;
};

typedef enum {
    AUDIO,
    MIDI
}GtkJackPortType;

typedef enum {
    GTK_JACK_PORT_IS_INPUT,
    GTK_JACK_PORT_IS_OUTPUT,
    GTK_JACK_PORT_IS_PHYSICAL,
    GTK_JACK_PORT_CAN_MONITOR,
    GTK_JACK_PORT_IS_TERMINAL
}GtkJackPortFlags;

static const GSList *
get_midi_ports_list (jack_client_t *client)
{
    GSList *list;
    const gchar **ports_array;
    gint i, j;
    gchar *dup;

    i = 0;
    j = 0;
    list = NULL;
    dup = NULL;
    ports_array = jack_get_ports (client, NULL, NULL, 0);

    while (ports_array[i++])
    {
        dup = g_strdup (ports_array[i]);



    }




    return list;
}

static const GSList *
get_ports_list (jack_client_t *client,
                enum JackPortFlags flags,
                GtkJackPortType type)
{
    GSList *list;
    const gchar **ports_array;
    gchar *dup_copy_audio;
    gchar *dup_copy_midi;
    gint i;
    gint j;

    dup_copy_audio = NULL;
    dup_copy_midi = NULL;
    list = NULL;
    i = 0;
    j = 0;
    flags = flags + 20;

    ports_array = jack_get_ports (client, NULL, NULL, 0);

    while (ports_array[++i]);

    for (j = 0; j < i; j++)
    {
        if (ports_array[j] == NULL)
        {
            break;
        }

        gchar *dup;
        gchar *modded_dup_audio;
        gchar *modded_dup_midi;
        jack_port_t *port;
        gint k;

        modded_dup_audio = NULL;
        modded_dup_midi = NULL;
        dup = NULL;
        dup = g_strdup (ports_array[j]);
        port = jack_port_by_name (client, dup);
        k = 0;

        /*if (g_strcmp0 (jack_port_type (port), JACK_DEFAULT_AUDIO_TYPE) == 0)
        {
            g_print ("audio\n");
        }
        else
        {
            g_print ("midi\n");
        }*/

        if ((dup_copy_audio == NULL) &&
            (jack_port_flags (port) == flags) &&
            (g_strcmp0 (jack_port_type (port), JACK_DEFAULT_AUDIO_TYPE) == 0))
        {
            while (dup[k++])
            {
                if (dup[k] == ':')
                {
                    dup_copy_audio = g_strndup (dup, k);

                    /*
                     * We don't want any jack MIDI ports here so if we find
                     * any then we skip to the next array element.
                     */
                    if (g_strcmp0 (jack_port_type (port), JACK_DEFAULT_MIDI_TYPE) == 0)
                    {
                        g_print ("test fail: %s\n", dup_copy_audio);
                        g_free (dup);
                        break;
                    }
                    else
                    {
                        g_print ("test success: %s\n", dup_copy_audio);
                        list = g_slist_prepend (list, dup_copy_audio);
                        g_free (dup);
                        break;
                    }
                }
            }
        }
        else if ((dup_copy_midi == NULL) &&
                 (jack_port_flags (port) == flags) &&
                 (g_strcmp0 (jack_port_type (port), JACK_DEFAULT_MIDI_TYPE) == 0))
        {
            while (dup[k++])
            {
                if (dup[k] == ':')
                {
                    dup_copy_midi = g_strndup (dup, k);

                    /*
                     * We don't want any jack AUDIO ports here so if we find
                     * any then we skip to the next array element.
                     */
                    if (g_strcmp0 (jack_port_type (port), JACK_DEFAULT_AUDIO_TYPE) == 0)
                    {
                        g_print ("test fail: %s\n", dup_copy_midi);
                        g_free (dup);
                        break;
                    }
                    else
                    {
                        g_print ("test success: %s\n", dup_copy_midi);

                        list = g_slist_prepend (list, dup_copy_midi);
                        g_free (dup);
                        break;
                    }
                }
            }
        }
        else if ((dup_copy_audio != NULL) &&
                 (jack_port_flags (port) == flags) &&
                 (g_strcmp0 (jack_port_type (port), JACK_DEFAULT_AUDIO_TYPE) == 0))
        {
            while (dup[k++])
            {
                if (dup[k] == ':')
                {
                    modded_dup_audio = g_strndup (dup, k);

                    /*
                     * We don't want any jack MIDI ports here so if we find
                     * any then we skip to the next array element.
                     *
                    if (g_strcmp0 (modded_dup_audio, "alsa_midi:") == 0)
                    {
                        g_free (dup);
                        break;
                    }*/

                    /*
                     * If 'modded_dup' and 'dup_copy' are the same skip to the
                     * next array element in 'ports_array'.
                     */
                    if (g_strcmp0 (dup_copy_audio, modded_dup_audio) == 0)
                    {
                        g_print ("test fail: %s\n", dup_copy_audio);

                        g_free (modded_dup_audio);
                        g_free (dup);
                        break;
                    }
                    else
                    {
                        /*
                         * Clear 'dup_copy' by re-initializing with 'NULL'
                         * I think and add new string to it.
                         */
                        dup_copy_audio = NULL;
                        dup_copy_audio = g_strndup (dup, k);

                        g_print ("test success: %s\n", dup_copy_audio);

                        list = g_slist_prepend (list, dup_copy_audio);

                        g_free (modded_dup_audio);
                        g_free (dup);
                    }
                }
            }
        }
        else if ((dup_copy_midi != NULL) &&
                 (jack_port_flags (port) == flags) &&
                 (g_strcmp0 (jack_port_type (port), JACK_DEFAULT_MIDI_TYPE) == 0))
        {
            while (dup[k++])
            {
                if (dup[k] == ':')
                {
                    modded_dup_midi = g_strndup (dup, k);

                    /*
                     * We don't want any jack AUDIO ports here so if we find
                     * any then we skip to the next array element.

                    if (g_strcmp0 (modded_dup, "alsa_midi:") != 0)
                    {
                        g_free (dup);
                        break;
                    }*/

                    /*
                     * If 'modded_dup' and 'dup_copy' are the same skip to the
                     * next array element in 'ports_array'.
                     */
                    if (g_strcmp0 (dup_copy_midi, modded_dup_midi) == 0)
                    {
                        g_print ("test fail: %s\n", dup_copy_midi);

                        g_free (modded_dup_midi);
                        g_free (dup);
                        break;
                    }
                    else
                    {
                        /*
                         * Clear 'dup_copy' by re-initializing with 'NULL'
                         * I think and add new string to it.
                         */
                        dup_copy_midi = NULL;
                        dup_copy_midi = g_strndup (dup, k);

                        g_print ("test success: %s\n", dup_copy_midi);

                        list = g_slist_prepend (list, dup_copy_midi);

                        g_free (modded_dup_midi);
                        g_free (dup);
                    }
                }
            }
        }
    }

    jack_free (ports_array);
    g_print ("right before returning: %s\n", dup_copy_midi);

    //g_free (dup_copy_audio);
    //g_free (dup_copy_midi);
    return list;
}

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
                           "Destination",
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
    GtkPassedJackPortsData *pdata;
    jack_port_t *port;

    pdata = g_slice_new (GtkPassedJackPortsData);

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

    pdata -> string = jack_get_ports (client,
                                      NULL,
                                      NULL,
                                      0);

    gtk_button_set_relief (GTK_BUTTON (audio_from_button), GTK_RELIEF_NONE);
    gtk_button_set_relief (GTK_BUTTON (audio_to_button), GTK_RELIEF_NONE);
    gtk_button_set_relief (GTK_BUTTON (midi_from_button), GTK_RELIEF_NONE);
    gtk_button_set_relief (GTK_BUTTON (midi_to_button), GTK_RELIEF_NONE);

    pdata -> client = client;

    while (pdata -> string[i++]);

    for (j = 0; j < i - 1; j++)
    {
        //create_port (pdata -> string[j], j, pdata);

        g_print ("jack_ports.c: %s\n", pdata -> string[j]);

        port = jack_port_by_name (client, pdata -> string[j]);

        //g_print ("port: %d\n", jack_port_flags (port));
    }

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
