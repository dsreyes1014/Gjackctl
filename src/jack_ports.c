#include "jack_ports.h"

typedef struct _GtkPassedJackPortsData GtkPassedJackPortsData;

struct _GtkPassedJackPortsData {
    const gchar **string;
    jack_client_t *client;
};

static void
from_button_clicked_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *popover;
    GMenu *menu;
    GMenu *section;
    gint i, j;
    gchar *copy;
    GtkPassedJackPortsData *rdata;
    
    i = 0;
    j = 0;
    menu = g_menu_new ();
    section = g_menu_new ();
    rdata = user_data;
    copy = NULL;

    while (rdata -> string[i++]);

    for (j; j < i; j++)
    {
        if (rdata -> string[j] == NULL)
        {
            break;
        }

        gint k;
        gchar *port_string;
        gchar *modded_string;
        jack_port_t *port;
        
        GMenuItem *item;
        
        modded_string = NULL;
        port_string = NULL;
        k = 0;
        port_string = g_strdup (rdata -> string[j]);
        port = jack_port_by_name (rdata -> client, port_string);
        
        if ((copy == NULL) && (jack_port_flags (port) == JackPortIsOutput))
        { 
            while (port_string[k++])
            {
                if (port_string[k] == ':')
                {
                    modded_string = g_strndup (port_string, k);
                    modded_string[0] = g_ascii_toupper (modded_string[0]);
                    copy = g_strdup (modded_string);
                    g_print ("jack_ports.c: %s\n", modded_string);
                    item = g_menu_item_new (modded_string, NULL);
                    g_menu_append_item (section, item);
                    break;
                }
            }
        }
        else
        {
            while (port_string[k++])
            {
                if (port_string[k] == ':')
                {
                    modded_string = g_strndup (port_string, k);
                    modded_string[0] = g_ascii_toupper (modded_string[0]);
        
                    //g_print ("jack_ports.c: %s\n", modded_string);

                    if (g_strcmp0 (copy, modded_string) != 0)
                    {
                        copy = g_strdup (modded_string);
                        
                        item = g_menu_item_new (modded_string, NULL);
                        g_menu_append_item (section, item);
                    }
                }
            }    
        }

        g_free (modded_string);
        g_free (port_string);
    }

    g_menu_insert_section (menu,
                           0,
                           "Source",
                           G_MENU_MODEL (section));
    
    popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                          G_MENU_MODEL (menu));

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);

    g_free (copy);
    
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
                      pdata);

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
