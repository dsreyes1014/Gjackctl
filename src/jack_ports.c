#include "jack_ports.h"

/*typedef struct _GtkPassedJackPortsData GtkPassedJackPortsData;

enum GtkJackPortType {
    GTK_JACK_PORT_AUDIO_FROM,
    GTK_JACK_PORT_AUDIO_TO,
    GTK_JACK_PORT_MIDI_FROM,
    GTK_JACK_PORT_MIDI_TO
};

struct _GtkPassedJackPortsData {
    jack_client_t *client;
    GtkListStore *list_store;
    GtkWidget *view;
    gint col_num;*/
    GtkWidget *from_audio;  /* GtkLabel */
    GtkWidget *to_audio;    /* GtkLabel */
    GtkWidget *from_midi;   /* GtkLabel */
    GtkWidget *to_midi;     /* GtkLabel */
/*  enum GtkJackPortType port_type;
    GSimpleAction *audio_action;
    GSimpleActionGroup *group;
};*/

static gchar **
modified_ports_array (const gchar **ports_array)
{
    /*
     * This function takes each jack port in the array and returns
     * the prefix of the port only.  For example "system:capture_1",
     * "system" will be sent back in the modified array.
     */

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

    /* NULL Terminate 'array'. */
    array[k] = NULL;

    g_free (modded_dup_copy);

    return array;
}

static const gchar *
get_matched_gtk_jack_port_prefix (const gchar   *string,
                                  const gchar   *port_clicked,
                                  jack_client_t *client)
{
    gchar *modded_string;
    gchar *dup;
    const gchar *string_return;
    gint i;
    jack_port_t *jack_port;

    string_return = NULL;
    modded_string = NULL;
    dup = NULL;
    dup = g_strdup (string);
    i = 0;

    while (dup[i])
    {
        if (dup[i] == ':')
        {
            modded_string = g_strndup (dup, i);

            if (g_strcmp0 (modded_string, port_clicked) == 0)
            {
                jack_port = jack_port_by_name (client, dup);
                string_return = jack_port_short_name (jack_port);
                break;
            }
        }

        i++;
    }

    g_free (modded_string);
    g_free (dup);

    if (string_return == NULL)
    {
        return NULL;
    }
    else
    {
        return string_return;
    }
}

static gint
clear_tree_view (GtkWidget *view)
{
    guint col;
    GtkTreeViewColumn *column;

    col = 1;

    while (col > 0)
    {
        col = gtk_tree_view_get_n_columns (GTK_TREE_VIEW (view));

        if (col == 0)
        {
            break;
        }

        column = gtk_tree_view_get_column (GTK_TREE_VIEW (view), col - 1);

        gtk_tree_view_remove_column (GTK_TREE_VIEW (view), column);

        col = gtk_tree_view_get_n_columns (GTK_TREE_VIEW (view));
    }

    return col;
}

static gint
disconnect_jack_port (const gchar *path_str,
                      gint         col_num,
                      gpointer     user_data)
{
    GtkTreePath *path;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkTreeViewColumn *column;
    GtkPassedJackPortsData *rdata;
    gchar *from_port_str;
    gchar *from_port_prefix;
    gchar *full_from_port_str;
    gchar *to_port_str;
    gchar *to_port_prefix;
    gchar *full_to_port_str;

    gint status;

    rdata = user_data;
    path = gtk_tree_path_new_from_string (path_str);
    if (rdata -> port_type == GTK_JACK_PORT_AUDIO_TO ||
        rdata -> port_type == GTK_JACK_PORT_AUDIO_FROM)
    {
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> view));
        from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_audio)));
        column = gtk_tree_view_get_column (GTK_TREE_VIEW (rdata -> view), col_num);
        to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_audio)));
    }
    else
    {
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> view2));
        from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_midi)));
        column = gtk_tree_view_get_column (GTK_TREE_VIEW (rdata -> view2), col_num);
        to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_midi)));
    }

    gtk_tree_model_get_iter (model, &iter, path);
    gtk_tree_model_get (model, &iter, 0, &from_port_str, -1);

    full_from_port_str = g_strconcat (from_port_prefix,
                                      ":",
                                      from_port_str,
                                      NULL);

    to_port_str = g_strdup (gtk_tree_view_column_get_title (column));

    full_to_port_str = g_strconcat (to_port_prefix,
                                    ":",
                                    to_port_str,
                                    NULL);

    status = jack_disconnect (rdata -> client,
                              full_from_port_str,
                              full_to_port_str);

    g_free (full_from_port_str);
    g_free (full_to_port_str);
    g_free (to_port_str);
    g_free (to_port_prefix);
    g_free (from_port_prefix);
    g_free (from_port_str);
    gtk_tree_path_free (path);

    return status;
}

static gint
connect_jack_port (const gchar *path_str,
                   gint         col_num,
                   gpointer     user_data)
{
    GtkTreePath *path;
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkTreeViewColumn *column;
    GtkPassedJackPortsData *rdata;
    gchar *from_port_str;
    gchar *from_port_prefix;
    gchar *full_from_port_str;
    gchar *to_port_str;
    gchar *to_port_prefix;
    gchar *full_to_port_str;

    gint status;

    rdata = user_data;
    path = gtk_tree_path_new_from_string (path_str);
    if (rdata -> port_type == GTK_JACK_PORT_AUDIO_TO ||
        rdata -> port_type == GTK_JACK_PORT_AUDIO_FROM)
    {
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> view));
        from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_audio)));
        column = gtk_tree_view_get_column (GTK_TREE_VIEW (rdata -> view), col_num);
        to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_audio)));
    }
    else
    {
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> view2));
        from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_midi)));
        column = gtk_tree_view_get_column (GTK_TREE_VIEW (rdata -> view2), col_num);
        to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_midi)));
    }

    gtk_tree_model_get_iter (model, &iter, path);
    gtk_tree_model_get (model, &iter, 0, &from_port_str, -1);

    full_from_port_str = g_strconcat (from_port_prefix,
                                      ":",
                                      from_port_str,
                                      NULL);

    to_port_str = g_strdup (gtk_tree_view_column_get_title (column));

    full_to_port_str = g_strconcat (to_port_prefix,
                                    ":",
                                    to_port_str,
                                    NULL);

    status = jack_connect (rdata -> client,
                           full_from_port_str,
                           full_to_port_str);

    g_free (full_from_port_str);
    g_free (full_to_port_str);
    g_free (to_port_str);
    g_free (to_port_prefix);
    g_free (from_port_prefix);
    g_free (from_port_str);
    gtk_tree_path_free (path);

    return status;
}

static void
toggled_cb (GtkCellRendererToggle *cell,
            gchar                 *path,
            gpointer               user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gboolean toggled;
    GtkPassedJackPortsData *rdata;
    gint column;
    GType type;

    /* This is how we obtain the column num. */
    column = GPOINTER_TO_UINT (g_object_get_data (G_OBJECT (cell), "column"));
    rdata = user_data;
    if (rdata -> port_type == GTK_JACK_PORT_MIDI_TO ||
        rdata -> port_type == GTK_JACK_PORT_MIDI_FROM)
    {
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> view2));
    }
    else
    {
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> view));
    }

    type = gtk_tree_model_get_column_type (model, column);

    gtk_tree_model_get_iter (model,
                             &iter,
                             gtk_tree_path_new_from_string (path));

    gtk_tree_model_get (model, &iter, column, &toggled, -1);

    if (toggled == FALSE)
    {
        toggled = TRUE;
        connect_jack_port (path,
                           column,
                           rdata);
    }
    else
    {
        toggled = FALSE;
        disconnect_jack_port (path,
                              column,
                              rdata);
    }

    gtk_list_store_set (GTK_LIST_STORE (model),
                        &iter,
                        column,
                        toggled,
                        -1);
}

static gint
get_n_columns (const gchar **array)
{
    gint i;

    i = 0;

    while (array[i])
    {
        i++;
    }

    /*
     * We add 1 for the first string column that's not accounted
     * for in the array.
     */
    return i + 1;
}

static gint
create_from_jack_port_filtered_model (GtkWidget    *view,
                                      const gchar  *from_port_clicked,
                                      const gchar **array,
                                      gpointer      user_data)
{
    /*
     * This will build the from ports part of the 'gtk_tree_model'
     * or 'gtk_list_store' which will always be the first column.
     */

    GtkPassedJackPortsData *rdata;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gint i;

    i = 0;
    rdata = user_data;
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (view));

    while (array[i])
    {
        const gchar *jack_port_string;

        jack_port_string = get_matched_gtk_jack_port_prefix (array[i],
                                                             from_port_clicked,
                                                             rdata -> client);

        if (jack_port_string != NULL)
        {
            gtk_list_store_append (GTK_LIST_STORE (model), &iter);
            gtk_list_store_set (GTK_LIST_STORE (model),
                                &iter,
                                0,
                                jack_port_string,
                                -1);
        }

        i++;
    }

    /* Clean up. */
    jack_free (array);

    return 0;
}

static gboolean
check_connections (GtkWidget   *view,
                   const gchar *port_str,
                   gpointer     user_data)
{
    GtkPassedJackPortsData *rdata;
    GtkTreeModel *model;
    gchar *from_port_prefix;
    gchar *to_port_prefix;
    gchar *short_port;
    gchar *full_from_port;
    gchar *full_to_port;
    const gchar **connections;
    GtkTreeIter iter;
    gboolean check;
    jack_port_t *port;
    gint i;
    gint j;
    gint num_rows;

    i = 0;
    j = 0;
    rdata = user_data;

    if (rdata -> port_type == GTK_JACK_PORT_AUDIO_FROM ||
        rdata -> port_type == GTK_JACK_PORT_AUDIO_TO)
    {
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> view));
        from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_audio)));
        to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_audio)));
    }
    else
    {
        model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> view2));
        from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_midi)));
        to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_midi)));
    }

    full_to_port = g_strconcat (to_port_prefix, ":", port_str, NULL);
    port = jack_port_by_name (rdata -> client , full_to_port);
    connections = jack_port_get_all_connections (rdata -> client, port);
    num_rows = gtk_tree_model_iter_n_children (model, NULL);

    gtk_tree_model_get_iter_first (model, &iter);

    for (i = 0; i < num_rows; i++)
    {
        gtk_tree_model_get (model, &iter, 0, &short_port, -1);
        full_from_port = g_strconcat (from_port_prefix, ":", short_port, NULL);

        if (connections == NULL)
        {
            g_free (full_from_port);
            g_free (short_port);
            g_free (full_to_port);
            g_free (to_port_prefix);
            g_free (from_port_prefix);

            return FALSE;
        }

        /* Reinitialize 'j' to 0 so we won't exit function prematurely. */
        j = 0;
        while (connections[j])
        {
            if (g_strcmp0 (connections[j], full_from_port) == 0)
            {
                gtk_list_store_set (GTK_LIST_STORE (model),
                                    &iter,
                                    rdata -> col_num,
                                    TRUE,
                                    -1);

                g_print ("Connected port: %s\n", connections[j]);
            }

            j++;
        }

        g_free (short_port);
        g_free (full_from_port);

        gtk_tree_model_iter_next (model, &iter);
    }

    g_free (full_to_port);
    g_free (to_port_prefix);
    g_free (from_port_prefix);
    jack_free (connections);

    return TRUE;
}

static GtkWidget *
create_label (const gchar *string)
{
    GtkWidget *label;

    label = gtk_label_new (string);

    gtk_label_set_angle (GTK_LABEL (label), 90);

    gtk_widget_show_all (label);

    return label;
}

static gint
create_to_jack_port_filtered_model (GtkTreeViewColumn  *column,
                                    GtkCellRenderer    *cell,
                                    GtkTreeModel       *model,
                                    GtkWidget          *view,
                                    const gchar        *to_port_clicked,
                                    const gchar       **array,
                                    gpointer            user_data)
{
    /*
     * This will build the to ports part of the 'gtk_tree_model'
     * or 'gtk_list_store'.
     */

    GtkPassedJackPortsData *rdata;
    GtkTreeIter iter;
    gint i;
    gint num_col;

    num_col = get_n_columns (array);
    i = 0;
    rdata = user_data;
    rdata -> col_num = 1;

    while (array[i])
    {
        const gchar *jack_port_string;

        jack_port_string = get_matched_gtk_jack_port_prefix (array[i],
                                                             to_port_clicked,
                                                             rdata -> client);

        if (jack_port_string != NULL)
        {
            GtkWidget *label;

            label = create_label (jack_port_string);

            cell = gtk_cell_renderer_toggle_new ();

            gtk_cell_renderer_toggle_set_radio (GTK_CELL_RENDERER_TOGGLE (cell),
                                                TRUE);

            gtk_cell_renderer_toggle_set_activatable (GTK_CELL_RENDERER_TOGGLE (cell),
                                                      TRUE);

            column = gtk_tree_view_column_new_with_attributes (jack_port_string,
                                                               cell,
                                                               "active",
                                                               rdata -> col_num,
                                                               NULL);

            gtk_tree_view_column_set_widget (column, label);
            gtk_tree_view_column_set_fixed_width (column, 40);
            gtk_tree_view_column_set_clickable (column, TRUE);
            gtk_tree_view_column_set_alignment (column, 0.0);
            gtk_tree_view_column_set_expand (column, FALSE);


            /*
             * Setup 'cell' to be able to later confirm the column number
             * it's associated with.
             */
            g_object_set_data (G_OBJECT (cell),
                               "column",
                               GUINT_TO_POINTER (rdata -> col_num));

            gtk_tree_model_get_iter_first (model, &iter);

            gtk_list_store_set (GTK_LIST_STORE (model),
                                &iter,
                                rdata -> col_num,
                                FALSE,
                                -1);

            gtk_tree_view_append_column (GTK_TREE_VIEW (view), column);

            check_connections (view,
                               jack_port_string,
                               rdata);

            g_signal_connect (cell,
                              "toggled",
                              G_CALLBACK (toggled_cb),
                              rdata);

            rdata -> col_num++;
        }

        i++;
    }

    /* Clean up. */
    jack_free (array);

    return 0;
}

static GtkTreeModel *
get_model (GtkWidget     *view,
           GtkListStore  *store,
           const gchar  **array)
{
    /*
     * This will either get an already existing model or create/init a new
     * one for our 'GtkTreeView'.
     */

    GtkTreeModel *model;
    gint num_col;
    gint i;

    num_col = 0;
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (view));


    if (model != NULL)
    {
        gtk_list_store_clear (GTK_LIST_STORE (model));
        clear_tree_view (view);
    }
    else
    {
        num_col = get_n_columns (array);

        /* Here we dynamically create an array for the 'GType *types'. */
        GType types[num_col];

        for (i = 0; i <= num_col; i++)
        {
            if ( i== 0)
            {
                types[i] = G_TYPE_STRING;

            }
            else
            {
                types[i] = G_TYPE_BOOLEAN;
            }
        }

        store = gtk_list_store_newv (num_col, types);

        gtk_tree_view_set_model (GTK_TREE_VIEW (view),
                                 GTK_TREE_MODEL (store));

        /* Clean up. */
        g_object_unref (store);

        model = gtk_tree_view_get_model (GTK_TREE_VIEW (view));
    }

    return model;
}

static gint
create_audio_jack_ports_view (const gchar  *from_port_clicked,
                              const gchar  *to_port_clicked,
                              gpointer      user_data)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkCellRenderer *toggle;
    GtkTreeModel *model;
    jack_port_t *jack_port;
    gint i;
    gint j;
    const gchar **from_ports_array;
    const gchar **to_ports_array;
    GtkTreeIter iter;
    GtkPassedJackPortsData *rdata;

    rdata = user_data;
    i = 0;
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (NULL,
                                                       renderer,
                                                       "text",
                                                       NULL);

    g_print ("Port Clicked: %s:%s\n", from_port_clicked, to_port_clicked);

    from_ports_array = jack_get_ports (rdata -> client,
                                       NULL,
                                       JACK_DEFAULT_AUDIO_TYPE,
                                       JackPortIsOutput);

    to_ports_array = jack_get_ports (rdata -> client,
                                     NULL,
                                     JACK_DEFAULT_AUDIO_TYPE,
                                     JackPortIsInput);

    model = get_model (rdata -> view,
                       rdata -> list_store,
                       to_ports_array);

    create_from_jack_port_filtered_model (rdata -> view,
                                          from_port_clicked,
                                          from_ports_array,
                                          rdata);

    gtk_tree_view_append_column (GTK_TREE_VIEW (rdata -> view), column);

    create_to_jack_port_filtered_model (column,
                                        toggle,
                                        model,
                                        rdata -> view,
                                        to_port_clicked,
                                        to_ports_array,
                                        rdata);

    return 0;
}

static gint
create_midi_jack_ports_view (const gchar  *from_port_clicked,
                             const gchar  *to_port_clicked,
                             gpointer      user_data)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkCellRenderer *toggle;
    GtkTreeModel *model;
    jack_port_t *jack_port;
    gint i;
    gint j;
    const gchar **from_ports_array;
    const gchar **to_ports_array;
    GtkTreeIter iter;
    GtkPassedJackPortsData *rdata;

    rdata = user_data;
    i = 0;
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (NULL,
                                                       renderer,
                                                       "text",
                                                       NULL);

    from_ports_array = jack_get_ports (rdata -> client,
                                       NULL,
                                       JACK_DEFAULT_MIDI_TYPE,
                                       JackPortIsOutput);

    to_ports_array = jack_get_ports (rdata -> client,
                                     NULL,
                                     JACK_DEFAULT_MIDI_TYPE,
                                     JackPortIsInput);

    model = get_model (rdata -> view2,
                       rdata -> list_store2,
                       to_ports_array);

    create_from_jack_port_filtered_model (rdata -> view2,
                                          from_port_clicked,
                                          from_ports_array,
                                          rdata);

    gtk_tree_view_append_column (GTK_TREE_VIEW (rdata -> view2), column);

    create_to_jack_port_filtered_model (column,
                                        toggle,
                                        model,
                                        rdata -> view2,
                                        to_port_clicked,
                                        to_ports_array,
                                        rdata);

    return 0;
}

static void
midi_action_activate_cb (GSimpleAction *action,
                         GVariant      *parameter,
                         gpointer       user_data)
{
    GtkPassedJackPortsData *rdata;
    const gchar *string;

    string = g_variant_get_string (parameter, NULL);
    rdata = user_data;

    if (rdata -> port_type == GTK_JACK_PORT_MIDI_FROM)
    {
        gtk_label_set_text (GTK_LABEL (rdata -> from_midi), string);
    }

    if (rdata -> port_type == GTK_JACK_PORT_MIDI_TO)
    {
        gtk_label_set_text (GTK_LABEL (rdata -> to_midi), string);
    }

    create_midi_jack_ports_view (gtk_label_get_text (GTK_LABEL (rdata -> from_midi)),
                                 gtk_label_get_text (GTK_LABEL (rdata -> to_midi)),
                                 rdata);

}

static void
audio_action_activate_cb (GSimpleAction *action,
                          GVariant      *parameter,
                          gpointer       user_data)
{
    GtkPassedJackPortsData *rdata;
    const gchar *string;

    string = g_variant_get_string (parameter, NULL);
    rdata = user_data;

    if (rdata -> port_type == GTK_JACK_PORT_AUDIO_FROM)
    {
        gtk_label_set_text (GTK_LABEL (rdata -> from_audio), string);
    }

    if (rdata -> port_type == GTK_JACK_PORT_AUDIO_TO)
    {
        gtk_label_set_text (GTK_LABEL (rdata -> to_audio), string);
    }

    create_audio_jack_ports_view (gtk_label_get_text (GTK_LABEL (rdata -> from_audio)),
                                  gtk_label_get_text (GTK_LABEL (rdata -> to_audio)),
                                  rdata);

}

static void
audio_from_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GMenu *from_section;
    const gchar **ports_array;
    gchar **modified_array;
    gint i;
    GtkPassedJackPortsData *rdata;

    rdata = user_data;
    i = 0;
    menu = g_menu_new ();
    from_section = g_menu_new ();
    ports_array = jack_get_ports (rdata -> client,
                                  NULL,
                                  JACK_DEFAULT_AUDIO_TYPE,
                                  JackPortIsOutput);

    modified_array = modified_ports_array (ports_array);

    rdata -> port_type = GTK_JACK_PORT_AUDIO_FROM;

    while (modified_array[i])
    {
        GMenuItem *item;

        item = g_menu_item_new (modified_array[i], NULL);
        g_menu_item_set_action_and_target_value (item,
                                                 "ports.audio-ports-view",
                                                 g_variant_new_string (modified_array[i]));

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

    gtk_widget_insert_action_group (popover,
                                    "ports",
                                    G_ACTION_GROUP (rdata -> group));

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);
    gtk_popover_set_transitions_enabled (GTK_POPOVER (popover), TRUE);

    g_signal_connect (rdata -> audio_action,
                      "activate",
                      G_CALLBACK (audio_action_activate_cb),
                      rdata);

    gtk_widget_show_all (popover);
}

static void
audio_to_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GMenu *to_section;
    const gchar **ports_array;
    gchar **modified_array;
    gint i;
    GtkPassedJackPortsData *rdata;

    rdata = user_data;
    i = 0;
    menu = g_menu_new ();
    to_section = g_menu_new ();
    ports_array = jack_get_ports (rdata -> client,
                                  NULL,
                                  JACK_DEFAULT_AUDIO_TYPE,
                                  JackPortIsInput);

    rdata -> port_type = GTK_JACK_PORT_AUDIO_TO;

    modified_array = modified_ports_array (ports_array);

    while (modified_array[i])
    {
        GMenuItem *item;

        item = g_menu_item_new (modified_array[i], NULL);
        g_menu_item_set_action_and_target_value (item,
                                                 "ports.audio-ports-view",
                                                 g_variant_new_string (modified_array[i]));

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

    gtk_widget_insert_action_group (popover,
                                    "ports",
                                    G_ACTION_GROUP (rdata -> group));

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);

    g_signal_connect (rdata -> audio_action,
                      "activate",
                      G_CALLBACK (audio_action_activate_cb),
                      rdata);

    gtk_widget_show_all (popover);
}

static void
midi_from_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GMenu *from_section;
    const gchar **ports_array;
    gchar **modified_array;
    gint i;
    GtkPassedJackPortsData *rdata;

    rdata = user_data;
    i = 0;
    menu = g_menu_new ();
    from_section = g_menu_new ();
    ports_array = jack_get_ports (rdata -> client,
                                  NULL,
                                  JACK_DEFAULT_MIDI_TYPE,
                                  JackPortIsOutput);

    modified_array = modified_ports_array (ports_array);

    rdata -> port_type = GTK_JACK_PORT_MIDI_FROM;

    while (modified_array[i])
    {
        GMenuItem *item;

        item = g_menu_item_new (modified_array[i], NULL);
        g_menu_item_set_action_and_target_value (item,
                                                 "ports.midi-ports-view",
                                                 g_variant_new_string (modified_array[i]));

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

    gtk_widget_insert_action_group (popover,
                                    "ports",
                                    G_ACTION_GROUP (rdata -> group));

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);
    gtk_popover_set_transitions_enabled (GTK_POPOVER (popover), TRUE);

    g_signal_connect (rdata -> midi_action,
                      "activate",
                      G_CALLBACK (midi_action_activate_cb),
                      rdata);

    gtk_widget_show_all (popover);
}

static void
midi_to_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GMenu *to_section;
    const gchar **ports_array;
    gchar **modified_array;
    gint i;
    GtkPassedJackPortsData *rdata;

    rdata = user_data;
    i = 0;
    menu = g_menu_new ();
    to_section = g_menu_new ();
    ports_array = jack_get_ports (rdata -> client,
                                  NULL,
                                  JACK_DEFAULT_MIDI_TYPE,
                                  JackPortIsInput);

    rdata -> port_type = GTK_JACK_PORT_MIDI_TO;

    modified_array = modified_ports_array (ports_array);

    while (modified_array[i])
    {
        GMenuItem *item;

        item = g_menu_item_new (modified_array[i], NULL);
        g_menu_item_set_action_and_target_value (item,
                                                 "ports.midi-ports-view",
                                                 g_variant_new_string (modified_array[i]));

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

    gtk_widget_insert_action_group (popover,
                                    "ports",
                                    G_ACTION_GROUP (rdata -> group));

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);

    g_signal_connect (rdata -> midi_action,
                      "activate",
                      G_CALLBACK (midi_action_activate_cb),
                      rdata);

    gtk_widget_show_all (popover);
}

gint
jack_ports (GtkWidget *stack, gpointer user_data)
{
    GtkWidget *sc_window;
    GtkWidget *sc_window2;
    GtkWidget *notebook;
    GtkWidget *audio_grid;
    GtkWidget *audio_grid2;
    GtkWidget *audio_grid3;
    GtkWidget *midi_grid;
    GtkWidget *midi_grid2;
    GtkWidget *midi_grid3;
    GtkWidget *audio_from_button;
    GtkWidget *audio_to_button;
    GtkWidget *midi_from_button;
    GtkWidget *midi_to_button;
    GtkWidget *to_button;
    GtkWidget *audio_label;
    GtkWidget *midi_label;
    gint i, j;
    jack_port_t *port;
    GtkPassedJackPortsData *pdata;

    pdata = user_data;

    sc_window = gtk_scrolled_window_new (NULL, NULL);
    sc_window2 = gtk_scrolled_window_new (NULL, NULL);
    i = 0;
    j = 0;
    notebook = gtk_notebook_new ();
    audio_grid = gtk_grid_new ();
    audio_grid2 = gtk_grid_new ();
    audio_grid3 = gtk_grid_new ();
    midi_grid = gtk_grid_new ();
    midi_grid2 = gtk_grid_new ();
    midi_grid3 = gtk_grid_new ();
    audio_label = gtk_label_new ("Audio");
    midi_label = gtk_label_new ("MIDI");
    audio_from_button = gtk_button_new_with_label ("From");
    audio_to_button = gtk_button_new_with_label ("To");
    midi_from_button = gtk_button_new_with_label ("From");
    midi_to_button = gtk_button_new_with_label ("To");
    to_button = gtk_button_new_with_label ("To");
    pdata -> view = gtk_tree_view_new ();
    pdata -> view2 = gtk_tree_view_new ();
    pdata -> list_store = NULL;
    pdata -> list_store2 = NULL;
    pdata -> from_audio = gtk_label_new ("system");
    pdata -> to_audio = gtk_label_new ("system");
    pdata -> to_midi = gtk_label_new ("alsa_midi");
    pdata -> from_midi = gtk_label_new ("alsa_midi");
    pdata -> group = g_simple_action_group_new ();
    pdata -> audio_action = g_simple_action_new ("audio-ports-view",
                                                 G_VARIANT_TYPE_STRING);

    pdata -> midi_action = g_simple_action_new ("midi-ports-view",
                                                G_VARIANT_TYPE_STRING);

    g_action_map_add_action (G_ACTION_MAP (pdata -> group),
                             G_ACTION (pdata -> audio_action));
    g_action_map_add_action (G_ACTION_MAP (pdata -> group),
                             G_ACTION (pdata -> midi_action));

    gtk_button_set_relief (GTK_BUTTON (audio_from_button), GTK_RELIEF_NONE);
    gtk_button_set_relief (GTK_BUTTON (audio_to_button), GTK_RELIEF_NONE);
    gtk_button_set_relief (GTK_BUTTON (midi_from_button), GTK_RELIEF_NONE);
    gtk_button_set_relief (GTK_BUTTON (midi_to_button), GTK_RELIEF_NONE);

    gtk_label_set_use_underline (GTK_LABEL (pdata -> to_midi), FALSE);
    gtk_label_set_use_underline (GTK_LABEL (pdata -> from_midi), FALSE);

    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sc_window),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sc_window2),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (pdata -> view),
                                  GTK_TREE_VIEW_GRID_LINES_BOTH);

    gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (pdata -> view2),
                                  GTK_TREE_VIEW_GRID_LINES_BOTH);

    g_signal_connect (audio_from_button,
                      "clicked",
                      G_CALLBACK (audio_from_button_clicked_cb),
                      pdata);

    g_signal_connect (audio_to_button,
                      "clicked",
                      G_CALLBACK (audio_to_button_clicked_cb),
                      pdata);

    g_signal_connect (midi_from_button,
                      "clicked",
                      G_CALLBACK (midi_from_button_clicked_cb),
                      pdata);

    g_signal_connect (midi_to_button,
                      "clicked",
                      G_CALLBACK (midi_to_button_clicked_cb),
                      pdata);

    create_audio_jack_ports_view (gtk_label_get_text (GTK_LABEL (pdata -> from_audio)),
                                  gtk_label_get_text (GTK_LABEL (pdata -> to_audio)),
                                  pdata);

    create_midi_jack_ports_view (gtk_label_get_text (GTK_LABEL (pdata -> from_midi)),
                                 gtk_label_get_text (GTK_LABEL (pdata -> to_midi)),
                                 pdata);

    gtk_container_add (GTK_CONTAINER (sc_window), pdata -> view);
    gtk_container_add (GTK_CONTAINER (sc_window2), pdata -> view2);

    /*gtk_scrolled_window_set_min_content_width (GTK_SCROLLED_WINDOW (sc_window),
                                               400);
    gtk_scrolled_window_set_min_content_height (GTK_SCROLLED_WINDOW (sc_window),
                                                200);*/

    gtk_widget_set_halign (pdata -> view, GTK_ALIGN_CENTER);
    gtk_widget_set_halign (pdata -> view2, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand (audio_from_button, TRUE);
    gtk_widget_set_hexpand (audio_to_button, TRUE);
    gtk_widget_set_size_request (sc_window, 600, 420);
    gtk_widget_set_hexpand (midi_from_button, TRUE);
    gtk_widget_set_hexpand (midi_to_button, TRUE);
    gtk_widget_set_size_request (sc_window2, 600, 420);



    gtk_grid_attach (GTK_GRID (audio_grid),
                     audio_grid2,
                     0, 0, 1, 1);

    gtk_grid_attach (GTK_GRID (audio_grid),
                     audio_grid3,
                     0, 1, 1, 1);

    gtk_grid_attach (GTK_GRID (audio_grid2),
                     audio_from_button,
                     0, 0, 1, 1);

    gtk_grid_attach (GTK_GRID (audio_grid2),
                     audio_to_button,
                     1, 0, 1, 1);

    gtk_grid_attach (GTK_GRID (audio_grid3),
                     sc_window,
                     0, 0, 1, 1);

    gtk_grid_attach (GTK_GRID (midi_grid),
                     midi_grid2,
                     0, 0, 1, 1);

    gtk_grid_attach (GTK_GRID (midi_grid),
                     midi_grid3,
                     0, 1, 1, 1);

    gtk_grid_attach (GTK_GRID (midi_grid2),
                     midi_from_button,
                     0, 0, 1, 1);

    gtk_grid_attach (GTK_GRID (midi_grid2),
                     midi_to_button,
                     1, 0, 1, 1);

    gtk_grid_attach (GTK_GRID (midi_grid3),
                     sc_window2,
                     0, 0, 1, 1);

    /*gtk_grid_attach (GTK_GRID (audio_grid),
                     pdata -> from_audio,
                     0, 1, 1, 1);

    gtk_grid_attach (GTK_GRID (audio_grid),
                     pdata -> to_audio,
                     1, 1, 1, 1);*/

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
