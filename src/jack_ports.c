#include "jack_ports.h"

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

        //col = gtk_tree_view_get_n_columns (GTK_TREE_VIEW (view));
    }

    return col;
}

static gint
disconnect_midi_jack_port (const gchar *path_str,
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
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> fview2));
    from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_midi)));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (rdata -> tview2), col_num);
    to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_midi)));

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
disconnect_audio_jack_port (const gchar *path_str,
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
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> fview1));
    from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_audio)));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (rdata -> tview1), col_num);
    to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_audio)));

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
connect_midi_jack_port (const gchar *path_str,
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
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> fview2));
    from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_midi)));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (rdata -> tview2), col_num);
    to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_midi)));

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

static gint
connect_audio_jack_port (const gchar *path_str,
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
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> fview1));
    from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_audio)));
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (rdata -> tview1), col_num);
    to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_audio)));

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
toggled_midi_cb (GtkCellRendererToggle *cell,
                 gchar                 *path,
                 gpointer               user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gboolean toggled;
    GtkPassedJackPortsData *rdata;
    gint column;

    rdata = user_data;

    /* This is how we obtain the column num. */
    column = GPOINTER_TO_UINT (g_object_get_data (G_OBJECT (cell), "column"));

    model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> fview2));

    gtk_tree_model_get_iter (model,
                             &iter,
                             gtk_tree_path_new_from_string (path));

    gtk_tree_model_get (model, &iter, column + 1, &toggled, -1);

    if (toggled == FALSE)
    {
        toggled = TRUE;
        connect_midi_jack_port (path,
                                column,
                                rdata);
    }
    else
    {
        toggled = FALSE;
        disconnect_midi_jack_port (path,
                                   column,
                                   rdata);
    }

    g_print ("MIDI Column Number Selected: %d\n", column);
    gtk_list_store_set (GTK_LIST_STORE (model),
                        &iter,
                        column + 1,
                        toggled,
                        -1);
}

static void
toggled_audio_cb (GtkCellRendererToggle *cell,
                  gchar                 *path,
                  gpointer               user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gboolean toggled;
    GtkPassedJackPortsData *rdata;
    gint column;

    rdata = user_data;

    /* This is how we obtain the column num. */
    column = GPOINTER_TO_UINT (g_object_get_data (G_OBJECT (cell), "column"));

    model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> fview1));

    gtk_tree_model_get_iter (model,
                             &iter,
                             gtk_tree_path_new_from_string (path));

    /* We add one to column number because both views use the same model. */
    gtk_tree_model_get (model, &iter, column + 1, &toggled, -1);

    if (toggled == FALSE)
    {
        toggled = TRUE;
        connect_audio_jack_port (path,
                                 column,
                                 rdata);
    }
    else
    {
        toggled = FALSE;
        disconnect_audio_jack_port (path,
                                    column,
                                    rdata);
    }

    g_print ("Column Number Selected: %d\n", column);
    /* We add one to column number because both views use the same model. */
    gtk_list_store_set (GTK_LIST_STORE (model),
                        &iter,
                        column + 1,
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

static gboolean
query_tooltip_treeview_cb (GtkWidget  *widget,
                           gint        x,
                           gint        y,
                           gboolean    keyboard_mode,
                           GtkTooltip *tooltip,
                           gpointer    user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    GtkTreePath *path;
    gchar *tt_string;
    GtkTreeViewColumn *column;

    model = gtk_tree_view_get_model (GTK_TREE_VIEW (widget));
    path = NULL;
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (widget), 0);

    if (!gtk_tree_view_get_tooltip_context (GTK_TREE_VIEW (widget), &x, &y,
                                            keyboard_mode, &model, &path, &iter))
    {
        return FALSE;
    }

    gtk_tree_model_get (model, &iter, 0, &tt_string, -1);

    gtk_tooltip_set_text (tooltip, g_strdup (tt_string));

    gtk_tree_view_set_tooltip_cell (GTK_TREE_VIEW (widget),
                                    tooltip,
                                    path,
                                    column,
                                    NULL);

    g_free (tt_string);
    gtk_tree_path_free (path);

    return TRUE;
}

static gboolean
check_midi_connections (GtkWidget   *view,
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
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> fview2));
    from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_midi)));
    to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_midi)));

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
                                    rdata -> col_num + 1,
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

static gboolean
check_audio_connections (GtkWidget   *view,
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
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (rdata -> fview1));
    from_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> from_audio)));
    to_port_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (rdata -> to_audio)));
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
                                    rdata -> col_num + 1,
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
    gtk_widget_set_size_request (label, 30, 150);
    gtk_label_set_ellipsize (GTK_LABEL (label), PANGO_ELLIPSIZE_MIDDLE);
    gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
    gtk_widget_set_tooltip_text (label, string);

    gtk_widget_show_all (label);

    return label;
}

static gint
create_midi_to_jack_port_filtered_model (GtkTreeViewColumn  *column,
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
    rdata -> col_num = 0;

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
                                                               rdata -> col_num + 1,
                                                               NULL);

            gtk_tree_view_column_set_widget (column, label);
            gtk_tree_view_column_set_fixed_width (column, 40);

            /*
             * Setup 'cell' to be able to later confirm the column number
             * it's associated with in our toggled signal callback.
             */
            g_object_set_data (G_OBJECT (cell),
                               "column",
                               GUINT_TO_POINTER (rdata -> col_num));
            //g_object_set (cell, "indicator-size", 20, NULL);

            gtk_tree_model_get_iter_first (model, &iter);

            /* We add one to column number because both views use the same model. */
            gtk_list_store_set (GTK_LIST_STORE (model),
                                &iter,
                                rdata -> col_num + 1,
                                FALSE,
                                -1);

            gtk_tree_view_append_column (GTK_TREE_VIEW (view), column);

            check_audio_connections (view,
                                     jack_port_string,
                                     rdata);

            g_signal_connect (cell,
                              "toggled",
                              G_CALLBACK (toggled_midi_cb),
                              rdata);
            g_print ("Number Of MIDI Columns: %d\n", rdata -> col_num);
            rdata -> col_num++;
        }

        i++;
    }

    /* Clean up. */
    jack_free (array);

    return 0;
}

static gint
create_audio_to_jack_port_filtered_model (GtkTreeViewColumn  *column,
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
    rdata -> col_num = 0;

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
                                                               rdata -> col_num + 1,
                                                               NULL);

            gtk_tree_view_column_set_widget (column, label);
            gtk_tree_view_column_set_fixed_width (column, 40);

            /*
             * Setup 'cell' to be able to later confirm the column number
             * it's associated with in our toggled signal callback.
             */
            g_object_set_data (G_OBJECT (cell),
                               "column",
                               GUINT_TO_POINTER (rdata -> col_num));
            //g_object_set (cell, "indicator-size", 20, NULL);

            gtk_tree_model_get_iter_first (model, &iter);

            /* We add one to column number because both views use the same model. */
            gtk_list_store_set (GTK_LIST_STORE (model),
                                &iter,
                                rdata -> col_num + 1,
                                FALSE,
                                -1);

            gtk_tree_view_append_column (GTK_TREE_VIEW (view), column);

            check_audio_connections (view,
                                     jack_port_string,
                                     rdata);

            g_signal_connect (cell,
                              "toggled",
                              G_CALLBACK (toggled_audio_cb),
                              rdata);
            g_print ("Number Of Columns: %d\n", rdata -> col_num);
            rdata -> col_num++;
        }

        i++;
    }

    /* Clean up. */
    jack_free (array);

    return 0;
}

static gint
create_midi_from_jack_port_filtered_model (GtkWidget          *view,
                                           GtkTreeViewColumn  *column,
                                           GtkCellRenderer    *cell,
                                           const gchar        *from_port_clicked,
                                           const gchar       **array,
                                           gpointer            user_data)
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

            g_object_set (G_OBJECT (view), "has-tooltip", TRUE, NULL);
            g_signal_connect (view,
                              "query-tooltip",
                              G_CALLBACK (query_tooltip_treeview_cb),
                              NULL);
        }

        i++;
    }

    /* Clean up. */
    jack_free (array);

    return 0;
}

static gint
create_audio_from_jack_port_filtered_model (GtkWidget          *view,
                                            GtkTreeViewColumn  *column,
                                            GtkCellRenderer    *cell,
                                            const gchar        *from_port_clicked,
                                            const gchar       **array,
                                            gpointer            user_data)
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

            g_object_set (G_OBJECT (view), "has-tooltip", TRUE, NULL);
            g_signal_connect (view,
                              "query-tooltip",
                              G_CALLBACK (query_tooltip_treeview_cb),
                              NULL);
        }

        i++;
    }

    /* Clean up. */
    jack_free (array);

    return 0;
}

static GtkTreeModel *
get_midi_model (GtkWidget     *fview,
                GtkWidget     *tview,
                GtkListStore  *store,
                const gchar  **array)
{
    /*
     * This will either get an already existing model and remove it
     * or create a new one for our 'GtkTreeView'.
     */

    GtkTreeModel *model;
    gint num_col;
    gint i;

    num_col = 0;
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (fview));

    if (model != NULL)
    {
        clear_tree_view (fview);
        clear_tree_view (tview);
        gtk_tree_view_set_model (GTK_TREE_VIEW (fview), NULL);
        gtk_tree_view_set_model (GTK_TREE_VIEW (tview), NULL);
    }

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

    gtk_tree_view_set_model (GTK_TREE_VIEW (fview),
                             GTK_TREE_MODEL (store));

    gtk_tree_view_set_model (GTK_TREE_VIEW (tview),
                             GTK_TREE_MODEL (store));

    /* Clean up. */
    g_object_unref (store);

    model = gtk_tree_view_get_model (GTK_TREE_VIEW (fview));

    return model;
}

static GtkTreeModel *
get_audio_model (GtkWidget     *fview,
                 GtkWidget     *tview,
                 GtkListStore  *store,
                 const gchar  **array)
{
    /*
     * This will either get an already existing model and remove it
     * or create a new one for our 'GtkTreeView'.
     */

    GtkTreeModel *model;
    gint num_col;
    gint i;

    num_col = 0;
    model = gtk_tree_view_get_model (GTK_TREE_VIEW (fview));


    if (model != NULL)
    {
        clear_tree_view (fview);
        clear_tree_view (tview);
        gtk_tree_view_set_model (GTK_TREE_VIEW (fview), NULL);
        gtk_tree_view_set_model (GTK_TREE_VIEW (tview), NULL);
    }


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

    gtk_tree_view_set_model (GTK_TREE_VIEW (fview),
                             GTK_TREE_MODEL (store));

    gtk_tree_view_set_model (GTK_TREE_VIEW (tview),
                             GTK_TREE_MODEL (store));

    /* Clean up. */
    g_object_unref (store);

    model = gtk_tree_view_get_model (GTK_TREE_VIEW (fview));

    return model;
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
    GtkWidget *label;
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
    label = create_label ("");

    column = gtk_tree_view_column_new_with_attributes (NULL,
                                                       renderer,
                                                       "text", 0,
                                                       NULL);

    gtk_tree_view_column_set_fixed_width (column, 160);
    gtk_tree_view_column_set_widget (column, label);

    g_object_set (G_OBJECT (renderer),
                  "ellipsize",
                  PANGO_ELLIPSIZE_MIDDLE,
                  NULL);

    g_object_set (G_OBJECT (renderer),
                  "xalign",
                  0.5,
                  NULL);

    g_object_set (renderer, "height", 20, NULL);

    from_ports_array = jack_get_ports (rdata -> client,
                                       NULL,
                                       JACK_DEFAULT_MIDI_TYPE,
                                       JackPortIsOutput);

    to_ports_array = jack_get_ports (rdata -> client,
                                     NULL,
                                     JACK_DEFAULT_MIDI_TYPE,
                                     JackPortIsInput);

    if (from_ports_array == NULL)
    {
        return -1;
    }
    model = get_midi_model (rdata -> fview2,
                            rdata -> tview2,
                            rdata -> list_store2,
                            to_ports_array);

    create_midi_from_jack_port_filtered_model (rdata -> fview2,
                                               column,
                                               renderer,
                                               from_port_clicked,
                                               from_ports_array,
                                               rdata);

    gtk_tree_view_append_column (GTK_TREE_VIEW (rdata -> fview2), column);

    create_midi_to_jack_port_filtered_model (column,
                                             toggle,
                                             model,
                                             rdata -> tview2,
                                             to_port_clicked,
                                             to_ports_array,
                                             rdata);

    return 0;
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
    GtkWidget *label;
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
    label = create_label ("");

    column = gtk_tree_view_column_new_with_attributes (NULL,
                                                       renderer,
                                                       "text", 0,
                                                       NULL);

    gtk_tree_view_column_set_fixed_width (column, 160);
    gtk_tree_view_column_set_widget (column, label);

    g_object_set (G_OBJECT (renderer),
                  "ellipsize",
                  PANGO_ELLIPSIZE_MIDDLE,
                  NULL);

    g_object_set (G_OBJECT (renderer),
                  "xalign",
                  0.5,
                  NULL);

    g_object_set (renderer, "height", 20, NULL);

    from_ports_array = jack_get_ports (rdata -> client,
                                       NULL,
                                       JACK_DEFAULT_AUDIO_TYPE,
                                       JackPortIsOutput);

    to_ports_array = jack_get_ports (rdata -> client,
                                     NULL,
                                     JACK_DEFAULT_AUDIO_TYPE,
                                     JackPortIsInput);

    if (from_ports_array == NULL)
    {
        return -1;
    }

    model = get_audio_model (rdata -> fview1,
                             rdata -> tview1,
                             rdata -> list_store,
                             to_ports_array);

    create_audio_from_jack_port_filtered_model (rdata -> fview1,
                                                column,
                                                renderer,
                                                from_port_clicked,
                                                from_ports_array,
                                                rdata);

    gtk_tree_view_append_column (GTK_TREE_VIEW (rdata -> fview1), column);

    create_audio_to_jack_port_filtered_model (column,
                                              toggle,
                                              model,
                                              rdata -> tview1,
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

    if (create_midi_jack_ports_view (gtk_label_get_text (GTK_LABEL (rdata -> from_midi)),
                                     gtk_label_get_text (GTK_LABEL (rdata -> to_midi)),
                                     rdata) == -1)
    {
        g_print ("No midi ports available.\nPlease check the MIDI option in settings\n");
    }

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

static GMenu *
create_menu_model (jack_client_t        *client,
                   enum GtkJackPortType  type)
{
    GMenu *menu;
    gchar **port_prefix;
    const gchar **ports;
    gint i;

    ports = NULL;
    menu = g_menu_new ();
    i = 0;

    switch (type)
    {
        case GTK_JACK_PORT_AUDIO_FROM:
            ports = jack_get_ports (client,
                                    NULL,
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsOutput);
            break;
        case GTK_JACK_PORT_AUDIO_TO:
            ports = jack_get_ports (client,
                                    NULL,
                                    JACK_DEFAULT_AUDIO_TYPE,
                                    JackPortIsInput);
            break;
        case GTK_JACK_PORT_MIDI_FROM:
            ports = jack_get_ports (client,
                                    NULL,
                                    JACK_DEFAULT_MIDI_TYPE,
                                    JackPortIsOutput);
            break;
        case GTK_JACK_PORT_MIDI_TO:
            ports = jack_get_ports (client,
                                    NULL,
                                    JACK_DEFAULT_MIDI_TYPE,
                                    JackPortIsInput);
            break;
    }

    if (ports == NULL)
    {
        g_print ("Ports equal NULL\n");
        return NULL;
    }

    port_prefix = modified_ports_array (ports);

    while (port_prefix[i])
    {
        GMenuItem *item;

        item = g_menu_item_new (port_prefix[i], NULL);


        if (type == GTK_JACK_PORT_AUDIO_FROM || type == GTK_JACK_PORT_AUDIO_TO)
        {
            g_menu_item_set_action_and_target_value (item,
                                                     "ports.audio-ports-view",
                                                     g_variant_new_string (port_prefix[i]));
        }
        else
        {
            g_menu_item_set_action_and_target_value (item,
                                                     "ports.midi-ports-view",
                                                     g_variant_new_string (port_prefix[i]));
        }

        g_menu_append_item (G_MENU (menu), item);

        g_object_unref (item);

        i++;
    }

    jack_free (ports);
    g_strfreev (port_prefix);

    return menu;
}

static void
audio_from_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GtkPassedJackPortsData *rdata;

    rdata = user_data;
    rdata -> port_type = GTK_JACK_PORT_AUDIO_FROM;
    menu = create_menu_model (rdata -> client, rdata -> port_type);

    popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                          G_MENU_MODEL (menu));


    gtk_widget_insert_action_group (GTK_WIDGET (button),
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
    GtkPassedJackPortsData *rdata;

    rdata = user_data;
    rdata -> port_type = GTK_JACK_PORT_AUDIO_TO;
    menu = create_menu_model (rdata -> client, rdata -> port_type);

    popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                          G_MENU_MODEL (menu));


    gtk_widget_insert_action_group (GTK_WIDGET (button),
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
midi_from_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GtkPassedJackPortsData *rdata;

    rdata = user_data;
    rdata -> port_type = GTK_JACK_PORT_MIDI_FROM;
    menu = create_menu_model (rdata -> client, rdata -> port_type);

    if (menu)
    {
        popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                              G_MENU_MODEL (menu));


        gtk_widget_insert_action_group (GTK_WIDGET (button),
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
}

static void
midi_to_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GtkPassedJackPortsData *rdata;

    rdata = user_data;
    rdata -> port_type = GTK_JACK_PORT_MIDI_TO;
    menu = create_menu_model (rdata -> client, rdata -> port_type);

    if (menu)
    {
        popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                              G_MENU_MODEL (menu));


        gtk_widget_insert_action_group (GTK_WIDGET (button),
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
}

static GtkWidget *
create_menu_button (GtkWidget              *label,
                    enum GtkJackPortType    type)
{
    GtkWidget *button;
    GtkWidget *img;
    GtkWidget *hbox;

    button = gtk_button_new ();
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    img = gtk_image_new_from_icon_name ("pan-down-symbolic",
                                        GTK_ICON_SIZE_BUTTON);

    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (hbox), img, FALSE, FALSE, 2);

    gtk_container_add (GTK_CONTAINER (button), hbox);

    return button;
}

static gboolean
delete_event_cb (GtkWidget *window,
                 GdkEvent  *event,
                 gpointer   user_data)
{
    gtk_widget_hide_on_delete (window);

    return TRUE;
}

static void
popup_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkPassedJackPortsData *pdata;

    pdata = user_data;
    if (pdata -> pwindow == NULL)
    {
        GtkWidget *header_bar;
        GtkWidget *stack;
        GtkWidget *sswitcher;
        GtkWidget *stack_box;
        GtkWidget *audio_box;
        GtkWidget *midi_box;
        GtkWidget *audio_bbox;
        GtkWidget *midi_bbox;
        GtkWidget *audio_sub_hbox;
        GtkWidget *midi_sub_hbox;
        GtkWidget *fsc_window1;
        GtkWidget *tsc_window1;
        GtkWidget *fsc_window2;
        GtkWidget *tsc_window2;
        GtkWidget *label_from;
        GtkWidget *label_to;
        gdouble page_size;
        GtkAdjustment *vadj1;
        GtkAdjustment *vadj2;

        label_from = gtk_label_new ("From");
        label_to = gtk_label_new ("To");
        pdata = user_data;
        pdata -> pwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        header_bar = gtk_header_bar_new ();
        stack = gtk_stack_new ();
        sswitcher = gtk_stack_switcher_new ();
        stack_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
        audio_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
        midi_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
        audio_bbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
        midi_bbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
        audio_sub_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
        midi_sub_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
        fsc_window1 = gtk_scrolled_window_new (NULL, NULL);
        tsc_window1 = gtk_scrolled_window_new (NULL, NULL);
        fsc_window2 = gtk_scrolled_window_new (NULL, NULL);
        tsc_window2 = gtk_scrolled_window_new (NULL, NULL);

        gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header_bar), TRUE);
        gtk_header_bar_set_custom_title (GTK_HEADER_BAR (header_bar), sswitcher);

        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (fsc_window1),
                                        GTK_POLICY_AUTOMATIC,
                                        GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (tsc_window1),
                                        GTK_POLICY_AUTOMATIC,
                                        GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (fsc_window2),
                                        GTK_POLICY_AUTOMATIC,
                                        GTK_POLICY_AUTOMATIC);
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (tsc_window2),
                                        GTK_POLICY_AUTOMATIC,
                                        GTK_POLICY_AUTOMATIC);

        gtk_container_add (GTK_CONTAINER (fsc_window1), pdata -> fview1);
        gtk_container_add (GTK_CONTAINER (tsc_window1), pdata -> tview1);
        gtk_container_add (GTK_CONTAINER (fsc_window2), pdata -> fview2);
        gtk_container_add (GTK_CONTAINER (tsc_window2), pdata -> tview2);

        gtk_stack_switcher_set_stack (GTK_STACK_SWITCHER (sswitcher),
                                      GTK_STACK (stack));

        gtk_box_pack_start (GTK_BOX (audio_bbox), label_from, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (audio_bbox),
                            pdata -> audio_from_button,
                            FALSE,
                            FALSE,
                            2);

        gtk_box_pack_start (GTK_BOX (audio_bbox), label_to, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (audio_bbox),
                            pdata -> audio_to_button,
                            FALSE,
                            FALSE,
                            2);

        gtk_widget_set_margin_start (label_to, 40);
        label_from = gtk_label_new ("From");
        label_to = gtk_label_new ("To");

        gtk_box_pack_start (GTK_BOX (midi_bbox), label_from, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (midi_bbox),
                            pdata -> midi_from_button,
                            FALSE,
                            FALSE,
                            2);

        gtk_box_pack_start (GTK_BOX (midi_bbox), label_to, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (midi_bbox),
                            pdata -> midi_to_button,
                            FALSE,
                            FALSE,
                            2);

        gtk_box_pack_start (GTK_BOX (audio_box), audio_bbox, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (midi_box), midi_bbox, FALSE, FALSE, 2);
        //gtk_box_pack_start (GTK_BOX (audio_sub_hbox), pdata -> fview1, FALSE, FALSE, 0);
        gtk_box_pack_start (GTK_BOX (audio_sub_hbox), fsc_window1, FALSE, FALSE, 0);
        gtk_box_pack_start (GTK_BOX (audio_sub_hbox), tsc_window1, FALSE, FALSE, 0);
        gtk_box_pack_start (GTK_BOX (audio_box), audio_sub_hbox, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (midi_sub_hbox), fsc_window2, FALSE, FALSE, 0);
        gtk_box_pack_start (GTK_BOX (midi_sub_hbox), tsc_window2, FALSE, FALSE, 0);
        //gtk_box_pack_start (GTK_BOX (audio_box), sc_window, FALSE, FALSE, 2);
        gtk_box_pack_start (GTK_BOX (midi_box), midi_sub_hbox, FALSE, FALSE, 2);
        gtk_stack_add_titled (GTK_STACK (stack), audio_box, "audio", "Audio");
        gtk_stack_add_titled (GTK_STACK (stack), midi_box, "midi", "MIDI");

        gtk_window_set_titlebar (GTK_WINDOW (pdata -> pwindow), header_bar);
        gtk_container_add (GTK_CONTAINER (pdata -> pwindow), stack);
        gtk_widget_set_halign (sswitcher, GTK_ALIGN_CENTER);
        gtk_widget_set_size_request (fsc_window1, 160, 40);
        gtk_widget_set_size_request (tsc_window1, 500, 400);
        gtk_widget_set_size_request (fsc_window2, 160, 40);
        gtk_widget_set_size_request (tsc_window2, 500, 400);
        gtk_widget_set_halign (pdata -> fview1, GTK_ALIGN_END);
        gtk_widget_set_halign (pdata -> tview1, GTK_ALIGN_START);
        gtk_widget_set_halign (pdata -> fview2, GTK_ALIGN_END);
        gtk_widget_set_halign (pdata -> tview2, GTK_ALIGN_START);
        gtk_widget_set_halign (audio_bbox, GTK_ALIGN_CENTER);
        gtk_widget_set_halign (midi_bbox, GTK_ALIGN_CENTER);
        gtk_widget_set_margin_start (label_to, 40);

        vadj1 = gtk_scrollable_get_vadjustment (GTK_SCROLLABLE (pdata -> tview1));
        gtk_scrollable_set_vadjustment (GTK_SCROLLABLE (pdata -> fview1), vadj1);
        vadj2 = gtk_scrollable_get_vadjustment (GTK_SCROLLABLE (pdata -> tview2));
        gtk_scrollable_set_vadjustment (GTK_SCROLLABLE (pdata -> fview2), vadj2);

        g_signal_connect (pdata ->pwindow,
                          "delete-event",
                          G_CALLBACK (delete_event_cb),
                          NULL);

        gtk_widget_show_all (pdata -> pwindow);
    }
    else
    {
        gtk_widget_show_all (pdata -> pwindow);
    }

    gtk_window_set_resizable (GTK_WINDOW (pdata -> pwindow), FALSE);
}

static gint
graph_reorder_cb (gpointer user_data)
{
    /*GtkPassedJackPortsData *pdata;

    pdata = user_data;
    */
    return 0;
}

static void
check_ports (GtkWidget              *fview,
             GtkWidget              *tview,
             gchar                  *port1_str,
             gchar                  *port2_str,
             gint                    connect,
             enum GtkJackPortType    type,
             GtkPassedJackPortsData *pdata)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *port_to_prefix;
    gchar *port_from_prefix;
    gint num_col;
    gint num_rows;
    gint i;
    gint col_num;

    model = gtk_tree_view_get_model (GTK_TREE_VIEW (fview));
    num_col = gtk_tree_view_get_n_columns (GTK_TREE_VIEW (tview));
    num_rows = gtk_tree_model_iter_n_children (model, NULL);
    col_num = -1;

    if (type == GTK_JACK_PORT_AUDIO_FROM || type == GTK_JACK_PORT_AUDIO_TO)
    {
        port_to_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (pdata -> to_audio)));
        port_from_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (pdata -> from_audio)));
    }
    else
    {
        port_to_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (pdata -> to_midi)));
        port_from_prefix = g_strdup (gtk_label_get_text (GTK_LABEL (pdata -> from_midi)));
    }


    /*
     * Get available 'to' ports.  We start on 1 to ignore the first column
     * which is of G_TYPE_STRING.
     */
    gchar *array_to[num_col];
    for (i = 0; i < num_col; i++)
    {
        GtkTreeViewColumn *column;
        GtkWidget *label;

        column = gtk_tree_view_get_column (GTK_TREE_VIEW (tview), i);
        label = gtk_tree_view_column_get_widget (column);
        array_to[i] = g_strconcat (port_to_prefix,
                                   ":",
                                   gtk_label_get_text (GTK_LABEL (label)),
                                   NULL);

        /* Terminate our array with 'NULL' if it reaches. */
        if (i == num_col)
        {
            array_to[i] = NULL;
        }

        g_print ("Available To Ports: %s\n", array_to[i]);
    }

    /*
     * Get the column number of tree view if it matches.  If it
     * doesn't match then 'col_num' will return -1 when it's referenced.  We
     * add 1 if it matches to compensate for the first string column which is
     * ignored.
     */
    for (i = 0; i <= num_col; i++)
    {
        g_print ("Port Name Count: %s\n", array_to[i]);

        if ((g_strcmp0 (array_to[i], port1_str) == 0) ||
            (g_strcmp0 (array_to[i], port2_str) == 0))
        {
            col_num = i;
            g_print ("Column number selected: %d\n", i);
            break;
        }

        g_print ("Port number count: %d\n", i);
    }

    /* Get available 'from' ports. */
    gchar *array_from[num_rows];
    gtk_tree_model_get_iter_first (model, &iter);
    for (i = 0; i < num_rows; i++)
    {
        gchar *string;
        gtk_tree_model_get (model, &iter, 0, &string, -1);
        array_from[i] = g_strconcat (port_from_prefix, ":", string, NULL);

        gtk_tree_model_iter_next (model, &iter);
        g_free (string);
    }

    gtk_tree_model_get_iter_first (model, &iter);
    for (i = 0; i < num_rows; i++)
    {

        /*
         * If our available 'from' port matches the port given
         * in our callback and our col_num isn't -1 then we
         * activate/deactivate the matching ports.
         */
        if (((g_strcmp0 (array_from[i], port1_str) == 0) ||
            (g_strcmp0 (array_from[i], port2_str) == 0)) &&
            (col_num != -1))
        {
            if (connect != 0)
            {

                gtk_list_store_set (GTK_LIST_STORE (model),
                                    &iter,
                                    col_num + 1,
                                    TRUE,
                                    -1);
            }
            else
            {
                gtk_list_store_set (GTK_LIST_STORE (model),
                                    &iter,
                                    col_num + 1,
                                    FALSE,
                                    -1);
            }
        }

        gtk_tree_model_iter_next (model, &iter);
    }

    g_free (port_to_prefix);
    g_free (port_from_prefix);
}

static void
check_connect_cb (jack_port_id_t a,
                  jack_port_id_t b,
                  int            connect,
                  gpointer       user_data)
{
    GtkPassedJackPortsData *pdata;
    GtkTreeIter iter;
    jack_port_t *port1;
    jack_port_t *port2;
    gchar *port1_str;
    gchar *port2_str;

    pdata = user_data;
    port1 = jack_port_by_id (pdata -> client, a);
    port2 = jack_port_by_id (pdata -> client, b);
    port1_str = g_strdup (jack_port_name (port1));
    port2_str = g_strdup (jack_port_name (port2));


    if (g_strcmp0 (jack_port_type (port1), JACK_DEFAULT_AUDIO_TYPE) == 0)
    {
        check_ports (pdata -> fview1,
                     pdata -> tview1,
                     port1_str,
                     port2_str,
                     connect,
                     GTK_JACK_PORT_AUDIO_FROM,
                     pdata);
    }
    else
    {
        check_ports (pdata -> fview2,
                     pdata -> tview2,
                     port1_str,
                     port2_str,
                     connect,
                     GTK_JACK_PORT_MIDI_FROM,
                     pdata);
    }

    g_free (port1_str);
    g_free (port2_str);
}

gint
jack_ports (GtkWidget     *button_box,
            gpointer       user_data)
{
    GtkPassedJackPortsData *pdata;

    pdata = user_data;

    pdata -> button = gtk_button_new_from_icon_name ("media-playlist-shuffle-symbolic",
                                                     GTK_ICON_SIZE_BUTTON);

    gtk_box_pack_start (GTK_BOX (button_box), pdata -> button, FALSE, FALSE, 2);

    gtk_widget_set_tooltip_text (pdata -> button,
                                 "Audio and MIDI port connections");

    gtk_widget_show_all (button_box);

    pdata -> pwindow = NULL;
    pdata -> fview1 = gtk_tree_view_new ();
    pdata -> tview1 = gtk_tree_view_new ();
    pdata -> fview2 = gtk_tree_view_new ();
    pdata -> tview2 = gtk_tree_view_new ();
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

    pdata -> audio_from_button = create_menu_button (pdata -> from_audio,
                                                     GTK_JACK_PORT_AUDIO_FROM);

    pdata -> audio_to_button = create_menu_button (pdata -> to_audio,
                                                   GTK_JACK_PORT_AUDIO_TO);

    pdata -> midi_from_button = create_menu_button (pdata -> from_midi,
                                                    GTK_JACK_PORT_MIDI_FROM);

    pdata -> midi_to_button = create_menu_button (pdata -> to_midi,
                                                  GTK_JACK_PORT_MIDI_TO);

    g_action_map_add_action (G_ACTION_MAP (pdata -> group),
                             G_ACTION (pdata -> audio_action));
    g_action_map_add_action (G_ACTION_MAP (pdata -> group),
                             G_ACTION (pdata -> midi_action));

    gtk_label_set_use_underline (GTK_LABEL (pdata -> to_midi), FALSE);
    gtk_label_set_use_underline (GTK_LABEL (pdata -> from_midi), FALSE);

    gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (pdata -> fview1),
                                  GTK_TREE_VIEW_GRID_LINES_BOTH);
    gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (pdata -> tview1),
                                  GTK_TREE_VIEW_GRID_LINES_BOTH);

    gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (pdata -> fview2),
                                  GTK_TREE_VIEW_GRID_LINES_BOTH);
    gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (pdata -> tview2),
                                  GTK_TREE_VIEW_GRID_LINES_BOTH);

    g_signal_connect (pdata -> button,
                      "clicked",
                      G_CALLBACK (popup_button_clicked_cb),
                      pdata);

    g_signal_connect (pdata -> audio_from_button,
                      "clicked",
                      G_CALLBACK (audio_from_button_clicked_cb),
                      pdata);

    g_signal_connect (pdata -> audio_to_button,
                      "clicked",
                      G_CALLBACK (audio_to_button_clicked_cb),
                      pdata);

    g_signal_connect (pdata -> midi_from_button,
                      "clicked",
                      G_CALLBACK (midi_from_button_clicked_cb),
                      pdata);

    g_signal_connect (pdata -> midi_to_button,
                      "clicked",
                      G_CALLBACK (midi_to_button_clicked_cb),
                      pdata);

    create_audio_jack_ports_view (gtk_label_get_text (GTK_LABEL (pdata -> from_audio)),
                                  gtk_label_get_text (GTK_LABEL (pdata -> to_audio)),
                                  pdata);

    create_midi_jack_ports_view (gtk_label_get_text (GTK_LABEL (pdata -> from_midi)),
                                 gtk_label_get_text (GTK_LABEL (pdata -> to_midi)),
                                 pdata);

    jack_set_port_connect_callback (pdata -> client, check_connect_cb, pdata);
    jack_set_graph_order_callback (pdata -> client, graph_reorder_cb, pdata);

    /* We activate here so we can set up our 'check_connect_cb'. */
    jack_activate (pdata -> client);

    return 0;
}
