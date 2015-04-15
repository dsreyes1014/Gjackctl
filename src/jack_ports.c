#include "jack_ports.h"

typedef struct _GtkPassedJackPortsData GtkPassedJackPortsData;

struct _GtkPassedJackPortsData {
    const gchar **string;
    jack_client_t *client;
};

static const GSList *
get_from_audio (jack_client_t *client)
{
    GSList *list;
    const gchar **ports_array;
    gchar *dup_copy;
    gint i;
    gint j;

    dup_copy = NULL;
    list = NULL;
    i = 0;
    j = 0;

    ports_array = jack_get_ports (client, NULL, NULL, 0);

    while (ports_array[++i]);

    g_print ("jack_ports.c: %s\n", ports_array[i]);

    for (j = 0; j < i; j++)
    {
        if (ports_array[j] == NULL)
        {
            break;
        }

        gchar *dup;
        gchar *modded_dup;
        jack_port_t *port;
        gint k;

        modded_dup = NULL;
        dup = NULL;
        dup = g_strdup (ports_array[j]);
        port = jack_port_by_name (client, dup);
        k = 0;

        if ((dup_copy == NULL) && (jack_port_flags (port) == JackPortIsOutput))
        {
            while (dup[k++])
            {
                if (dup[k] == ':')
                {
                    dup_copy = g_strndup (dup, k);

                    /*
                     * We don't want any jack MIDI ports here so if we find
                     * any then we skip to the next array element.
                     */
                    if (g_strcmp0 (dup_copy, "alsa_midi:") == 0)
                    {
                        g_free (dup);

                        break;
                    }
                    else
                    {
                        list = g_slist_prepend (list, dup_copy);

                        g_free (dup);
                    }
                }
            }
        }
        else
        {
            while (dup[k++])
            {
                if (dup[k] == ':')
                {

                    /*
                     * We don't want any jack MIDI ports here so if we find
                     * any then we skip to the next array element.
                     */
                    if (g_strcmp0 (dup_copy, "alsa_midi:") == 0)
                    {
                        g_free (dup);
                        break;
                    }

                    modded_dup = g_strndup (dup, k);

                    /*
                     * If 'modded_dup' and 'dup_copy' are the same skip to the
                     * next array element in 'ports_array'.
                     */
                    if (g_strcmp0 (dup_copy, modded_dup) == 0)
                    {
                        g_free (modded_dup);
                        g_free (dup);
                        break;
                    }
                    else
                    {
                        /*
                         * Clear 'dup_copy' by re-initializing with 'NULL'
                         * and add new string to it.
                         */
                        dup_copy = NULL;
                        dup_copy = g_strndup (dup, k);

                        list = g_slist_prepend (list, dup_copy);

                        g_free (modded_dup);
                        g_free (dup);
                    }
                }
            }
        }
    }

    jack_free (ports_array);
    g_free (dup_copy);
    return list;
}

static void
from_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GSList *list;

    list = NULL;
    list = (GSList *) get_from_audio (user_data);
    list = g_slist_reverse (list);
    list = g_slist_nth (list, 0);

    GtkWidget *popover;
    GMenu *menu;
    GMenu *from_section;

    menu = g_menu_new ();
    from_section = g_menu_new ();

    while (list != NULL)
    {
        GMenuItem *item;

        item = g_menu_item_new (list -> data, NULL);
        g_menu_append_item (G_MENU (from_section), item);
        g_slist_free_1 (list);

        list = g_slist_next (list);
    }

    g_slist_free (list);

    g_menu_insert_section (menu,
                           0,
                           "Source",
                           G_MENU_MODEL (from_section));

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
    GtkWidget *from_button;
    GtkWidget *to_button;
    GtkWidget *audio_label;
    GtkWidget *midi_label;
    gint i, j;
    GtkPassedJackPortsData *pdata;

    pdata = g_slice_new (GtkPassedJackPortsData);

    i = 0;
    j = 0;
    notebook = gtk_notebook_new ();
    audio_grid = gtk_grid_new ();
    midi_grid = gtk_grid_new ();
    audio_label = gtk_label_new ("Audio");
    midi_label = gtk_label_new ("MIDI");
    from_button = gtk_button_new_with_label ("From");
    to_button = gtk_button_new_with_label ("To");

    pdata -> string = jack_get_ports (client,
                                      NULL,
                                      NULL,
                                      0);

    gtk_button_set_relief (GTK_BUTTON (from_button), GTK_RELIEF_NONE);


    pdata -> client = client;

    /*if (client != NULL)
    {
        g_print ("jack_ports.c: %s\n", jack_get_client_name (client));
    }*/;


    while (pdata -> string[i++]);

    for (j = 0; j < i - 1; j++)
    {
        //create_port (pdata -> string[j], j, pdata);

        g_print ("jack_ports.c: %s\n", pdata -> string[j]);
    }

    g_signal_connect (from_button,
                      "clicked",
                      G_CALLBACK (from_button_clicked_cb),
                      client);

    //g_signal_connect (refresh_button, "clicked", G_CALLBACK (refresh_ports), pdata);

    gtk_widget_set_margin_start (from_button, 30);

    gtk_grid_attach (GTK_GRID (audio_grid),
                     from_button,
                     0, 0, 1, 1);

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
