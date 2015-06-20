#include "jack_log.h"

GtkWidget *text_view;

static GtkWidget *
check_if_parent_container (GtkWidget *child)
{
    GtkWidget *parent;

    parent = gtk_widget_get_parent (child);

    return parent;
}

static void
close_popover_cb (GtkPopover *popover,
                  gpointer    user_data)
{
    GtkPassedMainData *pdata;
    GtkWidget *widget;
    GList *list;

    pdata = user_data;
    widget = gtk_widget_get_parent (pdata -> text);

    gtk_container_remove (GTK_CONTAINER (widget), pdata -> text);

    list = gtk_container_get_children (GTK_CONTAINER (popover));

    g_list_first (list);

    while (list)
    {
        gtk_widget_destroy (list -> data);

        list = g_list_next (list);
    }

    g_list_free (list);

    gtk_widget_destroy (GTK_WIDGET (popover));
}

static void
info_msg_cb (const gchar *msg)
{
    GtkTextBuffer *buffer;
    gchar *copy;

    copy = g_strdup (msg);
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));

    gtk_text_buffer_insert_at_cursor (buffer, msg, -1);
    g_print ("From info_cb: \n%s\n", msg);
}

static void
error_msg_cb (const gchar *msg)
{
    GtkTextBuffer *buffer;
    GtkTextIter iter;
    GtkTextTagTable *table;
    gchar *copy;

    copy = g_strdup (msg);
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));

    if (gtk_text_tag_table_lookup (table, "red_fg") == NULL)
    {
        gtk_text_buffer_create_tag (buffer,
                                    "red_fg",
                                    "foreground",
                                    "red",
                                    NULL);
    }

    gtk_text_buffer_get_end_iter (buffer, &iter);
    gtk_text_buffer_insert_with_tags_by_name (buffer,
                                              &iter,
                                              copy,
                                              -1,
                                              "red_fg",
                                              NULL);
}


static void
button_clicked_cb (GtkButton *button,
                   gpointer   user_data)
{
    GtkWidget *popover;
    GtkWidget *scwindow;
    GtkPassedMainData *rdata;

    rdata = user_data;
    scwindow = gtk_scrolled_window_new (NULL, NULL);
    popover = gtk_popover_new (GTK_WIDGET (button));

    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scwindow),
									GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    g_object_ref (rdata -> text);
    //jack_set_info_function (info_msg_cb);
    //jack_set_error_function (error_msg_cb);

    gtk_container_add (GTK_CONTAINER (scwindow), rdata -> text);

    gtk_widget_set_size_request (scwindow, 600, 250);

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);
    gtk_container_add (GTK_CONTAINER (popover), scwindow);

    g_signal_connect (popover,
                      "closed",
                      G_CALLBACK (close_popover_cb),
                      rdata);

    gtk_widget_show_all (popover);
}

void
jack_log (GtkPassedMainData *pdata)
{
    GtkWidget *button;

    text_view = pdata -> text;
    button = gtk_button_new_from_icon_name ("accessories-text-editor-symbolic",
                                            GTK_ICON_SIZE_BUTTON);

    gtk_header_bar_pack_end (GTK_HEADER_BAR (pdata -> header_bar), button);

    gtk_widget_set_tooltip_text (button, "Log messages from the jackd server");

    g_signal_connect (button,
                      "clicked",
                      G_CALLBACK (button_clicked_cb),
                      pdata);
}
