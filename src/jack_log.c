#include "jack_log.h"

GtkWidget *text_view;

void
timestamp_newline (GtkTextBuffer *buffer)
{
    GDateTime *dt;
    GTimeZone *tz;
    gchar *time_string;
    gchar *modded_time;
    gint line_count;
    gint i;
    GtkTextIter iter;

    line_count = gtk_text_buffer_get_line_count (buffer);

    gtk_text_buffer_get_start_iter (buffer, &iter);
    for (i = 0; i < line_count; i++)
    {
        GtkTextMark *mark;
        GSList *list;

        list = NULL;
        mark = gtk_text_mark_new (NULL, FALSE);
        tz = g_time_zone_new ("America/Chicago");
        dt = g_date_time_new_now (tz);
        time_string = g_date_time_format (dt, "%I:%M:%S %P");
        modded_time = g_strconcat ("[", time_string, "]", " ", NULL);

        gtk_text_buffer_get_iter_at_line (buffer, &iter, i);

        list = gtk_text_iter_get_marks (&iter);
        if (list == NULL)
        {
            gtk_text_buffer_place_cursor (buffer, &iter);
            gtk_text_buffer_insert_at_cursor (buffer, modded_time, -1);
            gtk_text_buffer_get_iter_at_line (buffer, &iter, i);
            gtk_text_buffer_add_mark (buffer, mark, &iter);
        }

        g_slist_free (list);
        g_date_time_unref (dt);
        g_time_zone_unref (tz);
    }

    g_free (time_string);
    g_free (modded_time);
}

static void
info_msg_cb (const gchar *msg)
{
    GtkTextBuffer *buffer;
    GtkTextIter iter;

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));

    gtk_text_buffer_get_end_iter (buffer, &iter);
    gtk_text_buffer_place_cursor (buffer, &iter);
    gtk_text_buffer_insert_at_cursor (buffer, "\n", -1);
    gtk_text_buffer_get_end_iter (buffer, &iter);
    gtk_text_buffer_place_cursor (buffer, &iter);
    gtk_text_buffer_insert_at_cursor (buffer, msg, -1);

    timestamp_newline (buffer);
    //g_print ("From info_cb: \n%s\n", msg);
}

static void
error_msg_cb (const gchar *msg)
{
    GtkTextBuffer *buffer;
    GtkTextIter start;
    GtkTextIter end;
    gchar *copy;

    copy = g_strconcat ("\n", msg, NULL);
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));

    gtk_text_buffer_get_end_iter (buffer, &end);
    gtk_text_buffer_place_cursor (buffer, &end);
    gtk_text_buffer_insert_at_cursor (buffer, "\n", -1);
    gtk_text_buffer_get_end_iter (buffer, &end);
    gtk_text_buffer_place_cursor (buffer, &end);
    gtk_text_buffer_insert_at_cursor (buffer, msg, -1);

    timestamp_newline (buffer);
}

static void
notify_changed_cb (GtkWidget  *buffer,
                   GParamSpec *pspec,
                   gpointer    user_data)
{
    //timestamp_newline (GTK_TEXT_BUFFER (buffer));
}

static gboolean
delete_event_cb (GtkWidget *widget,
                 GdkEvent  *event,
                 gpointer   user_data)
{
    gtk_widget_hide_on_delete (widget);

    return TRUE;
}

static void
button_clicked_cb (GtkButton *button,
                   gpointer   user_data)
{
    GtkWidget *scwindow;
    GtkWidget *header_bar;
    GtkWidget *hsep;
    GtkApplication *app;
    GtkPassedMainData *rdata;
    GtkTextBuffer *buffer;

    rdata = user_data;
    app = gtk_window_get_application (GTK_WINDOW (rdata -> window));
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (rdata -> text));

    if (rdata -> log_window != NULL)
    {
        gtk_widget_show_all (GTK_WIDGET (rdata -> log_window));
    }
    else
    {
        rdata -> log_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        header_bar = gtk_header_bar_new ();
        scwindow = gtk_scrolled_window_new (NULL, NULL);
        hsep = gtk_separator_new (GTK_ORIENTATION_VERTICAL);

        gtk_window_set_application (GTK_WINDOW (rdata -> log_window), app);
        gtk_window_set_titlebar (GTK_WINDOW (rdata -> log_window), header_bar);
        gtk_window_set_transient_for (GTK_WINDOW (rdata -> log_window),
                                      GTK_WINDOW (rdata -> window));

        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scwindow),
									    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

        gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), hsep);
        gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header_bar), TRUE);

        gtk_container_add (GTK_CONTAINER (scwindow), rdata -> text);
        gtk_container_add (GTK_CONTAINER (rdata -> log_window), scwindow);

        gtk_widget_set_size_request (scwindow, 600, 250);
        gtk_widget_show_all (rdata -> log_window);
    }

    g_signal_connect (rdata -> log_window,
                      "delete-event",
                      G_CALLBACK (delete_event_cb),
                      rdata);

    /*g_signal_connect (buffer,
                      "changed",
                      G_CALLBACK (notify_changed_cb),
                      rdata);*/
}

void
jack_log (GtkPassedMainData *pdata)
{
    GtkWidget *button;

    pdata -> log_window = NULL;
    text_view = pdata -> text;
    button = gtk_button_new_from_icon_name ("accessories-text-editor-symbolic",
                                            GTK_ICON_SIZE_BUTTON);

    gtk_text_view_set_editable (GTK_TEXT_VIEW (pdata -> text), FALSE);

    gtk_header_bar_pack_end (GTK_HEADER_BAR (pdata -> header_bar), button);

    gtk_widget_set_tooltip_text (button, "Log messages from the jackd server");

    jack_set_info_function (info_msg_cb);
    jack_set_error_function (error_msg_cb);

    g_signal_connect (button,
                      "clicked",
                      G_CALLBACK (button_clicked_cb),
                      pdata);
}
