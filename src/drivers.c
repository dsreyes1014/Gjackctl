#include "drivers.h"

static const gchar *
get_driver_type ()
{
    const gchar *string;
    gchar *copy;
    gchar *file;
    config_t config;
    gint i;

    i = 0;
    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

    config_init (&config);
    config_read_file (&config, file);
    if (config_lookup_string (&config, "gjackctl.driver.type", &string) == CONFIG_FALSE)
    {
        gchar *value_copy;
        config_setting_t *group;
        config_setting_t *setting;

        g_print ("\'Driver Type\' config option not available\n");
        g_print ("Creating config setting now...\n");

        value_copy = g_strdup ("Dummy");
        group = config_lookup (&config, "gjackctl.driver");
        setting = config_setting_add (group, "type", CONFIG_TYPE_STRING);
        config_setting_set_string (setting, value_copy);
        config_write_file (&config, file);
        string = g_strdup (value_copy);
        g_free (value_copy);
    }

    copy = g_strdup (string);
    g_free (file);
    config_destroy (&config);

    /* Upper case 'alsa'. */
    if (g_strcmp0 (copy, "alsa") == 0)
    {
        while (copy[i])
        {
            copy[i] = g_ascii_toupper (copy[i]);
            i++;
        }
    }

    return (const gchar *) copy;
}

const gchar *
get_driver_device ()
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
    if (config_lookup_string (&config, "gjackctl.driver.device", &string) == CONFIG_FALSE)
    {
        gchar *value_copy;
        config_setting_t *group;
        config_setting_t *setting;

        g_print ("\'Driver Device\' config option not available\n");
        g_print ("Creating config setting now...\n");

        value_copy = g_strdup ("");
        group = config_lookup (&config, "gjackctl.device");
        setting = config_setting_add (group, "type", CONFIG_TYPE_STRING);
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

static const gchar *
create_label_string ()
{
    const gchar *string;

    if (g_strcmp0 (get_driver_type (), "dummy") == 0)
    {
        gchar *modded;

        modded = g_strdup (get_driver_type ());
        modded[0] = g_ascii_toupper (modded[0]);
        string = g_strdup (modded);

        g_free (modded);
    }
    else
    {
        string = g_strconcat (get_driver_type (),
                              " - ",
                              "\"",
                              get_driver_device (),
                              "\"",
                              NULL);
    }

    return string;
}

static void
alsa_action_activate_cb (GSimpleAction *action,
                         GVariant *parameter,
                         gpointer user_data)
{
    const gchar *driver_str;
    const gchar *device_str;
    const gchar *label_str;
    gchar *modded_drv_str;
    gsize length;
    gint i;

    i = 0;
    driver_str = g_strdup ("alsa");
    device_str = g_variant_get_string (parameter, &length);
    modded_drv_str = g_strdup (driver_str);

    while (modded_drv_str[i])
    {
        modded_drv_str[i] = g_ascii_toupper (modded_drv_str[i]);
        i++;
    }

    label_str = g_strconcat (modded_drv_str,
                             " - ",
                             "\"",
                             device_str,
                             "\"",
                             NULL);

    gtk_label_set_text (GTK_LABEL (user_data), label_str);

    config_file_input ("gjackctl.driver.type",
                       CONFIG_TYPE_STRING,
                       (gpointer) driver_str);

    config_file_input ("gjackctl.driver.device",
                       CONFIG_TYPE_STRING,
                       (gpointer) g_variant_get_string (parameter, &length));

    g_free (modded_drv_str);
}

static void
dummy_action_activate_cb (GSimpleAction *action,
                          GVariant      *parameter,
                          gpointer       user_data)
{
    const gchar *string;
    const gchar *device;
    gsize length;

    string = g_variant_get_string (parameter, &length);
    device = g_strdup ("");

    config_file_input ("gjackctl.driver.type",
                       CONFIG_TYPE_STRING,
                       (gpointer) string);

    config_file_input ("gjackctl.driver.device",
                       CONFIG_TYPE_STRING,
                       (gpointer) device);

    gtk_label_set_text (GTK_LABEL (user_data), create_label_string ());
}

static GMenuItem *
create_dummy_menu_item ()
{
    GMenuItem *item;

    item = g_menu_item_new ("Dummy", NULL);

    g_menu_item_set_action_and_target_value (item,
                                             "driver.dummy",
                                             g_variant_new_string ("dummy"));

    return item;
}

void
drivers (GtkWidget *grid)
{
	GtkWidget *label;
    GtkWidget *mbutton;
    GtkWidget *mbutton_hbox;
    GtkWidget *mbutton_label;
    GtkWidget *img;
    GtkPopover *popover;
    GMenu *main_menu;
    GMenu *alsa_submenu;
    GSimpleAction *alsa_action;
    GSimpleAction *dummy_action;
    GSimpleActionGroup *group;

    main_menu = g_menu_new ();
    alsa_submenu = g_menu_new ();
    group = g_simple_action_group_new ();
    alsa_action = g_simple_action_new ("alsa", G_VARIANT_TYPE_STRING);
    dummy_action = g_simple_action_new ("dummy", G_VARIANT_TYPE_STRING);
    label = gtk_label_new ("Interface");
    mbutton = gtk_menu_button_new ();
    mbutton_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    mbutton_label = gtk_label_new (create_label_string ());
    img = gtk_image_new_from_icon_name ("pan-down-symbolic",
                                        GTK_ICON_SIZE_BUTTON);

    /* Pack our menu button. */
    gtk_box_pack_start (GTK_BOX (mbutton_hbox),
                        mbutton_label,
                        FALSE,
                        FALSE,
                        2);

    gtk_box_pack_end (GTK_BOX (mbutton_hbox),
                      img,
                      FALSE,
                      FALSE,
                      2);

    gtk_container_add (GTK_CONTAINER (mbutton), mbutton_hbox);
    gtk_widget_set_halign (img, GTK_ALIGN_END);
    gtk_widget_set_halign (mbutton_label, GTK_ALIGN_FILL);

    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (alsa_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (dummy_action));
    gtk_widget_insert_action_group (mbutton,
                                    "driver",
                                    G_ACTION_GROUP (group));

    alsa_device_names (alsa_submenu);

    g_menu_append_submenu (G_MENU (main_menu),
                           "ALSA",
                           G_MENU_MODEL (alsa_submenu));

    g_menu_append_item (G_MENU (main_menu), create_dummy_menu_item ());

    gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (mbutton),
                                    G_MENU_MODEL (main_menu));

    gtk_menu_button_set_use_popover (GTK_MENU_BUTTON (mbutton), TRUE);
    popover = gtk_menu_button_get_popover (GTK_MENU_BUTTON (mbutton));
    gtk_popover_set_position (popover, GTK_POS_BOTTOM);
    gtk_popover_set_relative_to (popover, img);

    gtk_grid_attach (GTK_GRID (grid),
                     label,
                     0, 0, 1, 1);

    gtk_grid_attach_next_to (GTK_GRID (grid),
                             mbutton,
                             label,
                             GTK_POS_RIGHT,
                             2, 1);

    gtk_widget_set_halign (label, GTK_ALIGN_END);
    gtk_widget_set_margin_start (mbutton, 20);
    gtk_widget_set_halign (mbutton, GTK_ALIGN_FILL);

    g_signal_connect (alsa_action,
                      "activate",
                      G_CALLBACK (alsa_action_activate_cb),
                      mbutton_label);

    g_signal_connect (dummy_action,
                      "activate",
                      G_CALLBACK (dummy_action_activate_cb),
                      mbutton_label);
}
