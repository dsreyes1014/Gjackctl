#include "clocksource.h"

static const gchar *
get_clocksource ()
{
    const gchar *string;
    const gchar *copy;
    gchar *file;
    config_t config;

    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

    config_init (&config);
    config_read_file (&config, file);
    if (config_lookup_string (&config, "gjackctl.server.clocksource", &string) == CONFIG_FALSE)
    {
        gchar *value_copy;
        config_setting_t *group;
        config_setting_t *setting;

        g_print ("\'Timeout\' config option not available\n");
        g_print ("Creating config setting now...\n");

        value_copy = g_strdup ("System");
        group = config_lookup (&config, "gjackctl.server");
        setting = config_setting_add (group, "clocksource", CONFIG_TYPE_STRING);
        config_setting_set_string (setting, value_copy);
        config_write_file (&config, file);
        string = g_strdup (value_copy);
        g_free (value_copy);
    }

    copy = g_strdup (string);
    g_free (file);
    config_destroy (&config);

    return copy;
}

static void
clocksource_activate_cb (GSimpleAction *simple,
                         GVariant *parameter,
                         gpointer user_data)
{
    gsize length;

    config_file_input ("gjackctl.server.clocksource",
                       CONFIG_TYPE_STRING,
                       (gpointer) g_variant_get_string (parameter, &length));
}

static GMenu *
create_menu ()
{
    GMenu *menu;
    GMenuItem *clocksource_item1;
    GMenuItem *clocksource_item2;
    GMenuItem *clocksource_item3;

    menu = g_menu_new ();
    clocksource_item1 = g_menu_item_new ("Cycle", NULL);
    clocksource_item2 = g_menu_item_new ("Hpet", NULL);
    clocksource_item3 = g_menu_item_new ("System", NULL);

    g_menu_item_set_action_and_target_value (clocksource_item1,
                                             "pop.clocksource",
                                             g_variant_new_string ("Cycle"));
    g_menu_item_set_action_and_target_value (clocksource_item2,
                                             "pop.clocksource",
                                             g_variant_new_string ("Hpet"));
    g_menu_item_set_action_and_target_value (clocksource_item3,
                                             "pop.clocksource",
                                             g_variant_new_string ("System"));

    g_menu_insert_item (menu, 0, clocksource_item1);
    g_menu_insert_item (menu, 1, clocksource_item2);
    g_menu_insert_item (menu, 2, clocksource_item3);

    return menu;
}

void
clocksource (GtkWidget *grid)
{
    GtkWidget *label;
    GtkWidget *combo_box;
    GtkWidget *mbutton;
    GtkWidget *hbox;
    GtkWidget *label1;
    GtkWidget *img;
    GMenu *main_menu;
    GVariant *clocksource_variant;
    GSimpleAction *clocksource_action;
    GSimpleActionGroup *group;
    GtkPopover *popover;

    label = gtk_label_new ("Clocksource");
    combo_box = gtk_combo_box_text_new ();
    mbutton = gtk_menu_button_new ();
    hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    label1 = gtk_label_new (get_clocksource ());
    img = gtk_image_new_from_icon_name ("pan-down-symbolic",
                                        GTK_ICON_SIZE_BUTTON);

    clocksource_variant = g_variant_new_string (get_clocksource ());
    clocksource_action = g_simple_action_new ("clocksource",
                                              G_VARIANT_TYPE_STRING);

    group = g_simple_action_group_new ();
    g_action_map_add_action (G_ACTION_MAP (group),
                             G_ACTION (clocksource_action));

    main_menu = create_menu ();

    gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (mbutton),
                                    G_MENU_MODEL (main_menu));

    gtk_menu_button_set_use_popover (GTK_MENU_BUTTON (mbutton), TRUE);
    popover = gtk_menu_button_get_popover (GTK_MENU_BUTTON (mbutton));
    gtk_popover_set_position (popover, GTK_POS_BOTTOM);
    gtk_popover_set_relative_to (popover, img);

    gtk_box_pack_start (GTK_BOX (hbox), label1, FALSE, FALSE, 2);
    gtk_box_pack_end (GTK_BOX (hbox), img, FALSE, FALSE, 2);
    gtk_container_add (GTK_CONTAINER (mbutton), hbox);

    gtk_widget_insert_action_group (mbutton,
                                    "pop",
                                    G_ACTION_GROUP (group));

    gtk_grid_attach (GTK_GRID (grid),
                     label,
                     0, 3, 1, 1);

    gtk_grid_attach_next_to (GTK_GRID (grid),
                             mbutton,
                             label,
                             GTK_POS_RIGHT,
                             1, 1);

    gtk_widget_set_halign (label, GTK_ALIGN_START);
    gtk_widget_set_margin_start (mbutton, 20);
    gtk_widget_set_halign (mbutton, GTK_ALIGN_FILL);

    g_signal_connect (clocksource_action,
                      "activate",
                      G_CALLBACK (clocksource_activate_cb),
                      NULL);
}
