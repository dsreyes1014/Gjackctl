#include "gjackctl_settings.h"

static void
close_period_window_cb (GtkWidget *widget,
                        GdkEvent  *event,
                        gpointer   user_data)
{
    GtkWidget *toplevel;
    gint period;
    gchar string[10];

    toplevel = gtk_widget_get_toplevel (widget);
    period = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (user_data));
    g_sprintf (string, "%d", period);

    config_file_input ("gjackctl.driver.period",
                       CONFIG_TYPE_STRING,
                       (gpointer) string);

    gtk_widget_destroy (widget);

    gtk_widget_grab_focus (toplevel);

}

static void
close_priority_window_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    GtkWidget *toplevel;
    gint priority;
    gchar string[10];

    toplevel = gtk_widget_get_toplevel (widget);
    priority = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (user_data));        
    g_sprintf (string, "%d", priority);

    config_file_input ("gjackctl.server.priority",
                       CONFIG_TYPE_STRING,
                       (gpointer) string);

    gtk_widget_destroy (widget);
    
    gtk_widget_grab_focus (toplevel);
}

static void
frames_state_cb (GSimpleAction *action,
                 GVariant *parameter,
                 gpointer user_data)
{
    g_simple_action_set_state (action, parameter);

    gsize length;

    config_file_input ("gjackctl.driver.frames",
                       CONFIG_TYPE_STRING,
                       (gpointer) g_variant_get_string (parameter, &length));
}

static void
sample_rate_state_cb (GSimpleAction *action,
                      GVariant *parameter,
                      gpointer user_data)
{
    g_simple_action_set_state (action, parameter);

    gsize length;

    config_file_input ("gjackctl.driver.sample_rate",
                       CONFIG_TYPE_STRING,
                       (gpointer) g_variant_get_string (parameter, &length));
}

static void
alsa_state_cb (GSimpleAction *action,
               GVariant *parameter,
               gpointer user_data)
{	
    g_simple_action_set_state (action, parameter);

    const gchar *string;
    gsize length;

    string = "ALSA";

    config_file_input ("gjackctl.driver.type",
                       CONFIG_TYPE_STRING,
                       (gpointer) string);

    config_file_input ("gjackctl.driver.device",
                       CONFIG_TYPE_STRING,
                       (gpointer) g_variant_get_string (parameter, &length));
}

/*static void
period_size_cb (GSimpleAction *simple,
                GVariant      *parameter,
                gpointer       user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *spin_button;
    GtkAdjustment *adjustment;
    GtkPassedMainData *rdata;
    gint num;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    grid = gtk_grid_new ();
    num = g_ascii_strtoll (get_priority (), NULL, 0);
    adjustment = gtk_adjustment_new (num, 0, 99, 1, 0, 0);
    spin_button = gtk_spin_button_new (adjustment, 1, 0);
    rdata = user_data;

    gtk_grid_attach (GTK_GRID (grid), spin_button, 0, 0, 1, 1);
    gtk_container_add (GTK_CONTAINER (window), grid);

    gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (rdata -> window));
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_modal (GTK_WINDOW (window), TRUE);
    gtk_window_set_decorated (GTK_WINDOW (window), FALSE);
    gtk_widget_set_events (window, GDK_FOCUS_CHANGE_MASK);

    g_signal_connect (window,
                      "focus-out-event",
                      G_CALLBACK (close_period_window_cb),
                      spin_button);

    gtk_widget_show_all (window);

}*/

/*static void
activate_priority_cb (GSimpleAction *simple,
                      GVariant *parameter,
                      gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *spin_button;
    GtkAdjustment *adjustment;
    GtkPassedMainData *rdata;
    gint num;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    grid = gtk_grid_new ();
    num = g_ascii_strtoll (get_priority (), NULL, 0);
    adjustment = gtk_adjustment_new (num, 0, 99, 1, 0, 0);
    spin_button = gtk_spin_button_new (adjustment, 1, 0);
    rdata = user_data;

    gtk_grid_attach (GTK_GRID (grid), spin_button, 0, 0, 1, 1);
    gtk_container_add (GTK_CONTAINER (window), grid);

    gtk_window_set_transient_for (GTK_WINDOW (window), GTK_WINDOW (rdata -> window));
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    gtk_window_set_modal (GTK_WINDOW (window), TRUE);
    gtk_window_set_decorated (GTK_WINDOW (window), FALSE);
    gtk_widget_set_events (window, GDK_FOCUS_CHANGE_MASK);

    g_signal_connect (window,
                      "focus-out-event",
                      G_CALLBACK (close_priority_window_cb),
                      spin_button);
    
    gtk_widget_show_all (window);
}*/

static void
port_max_state_cb (GSimpleAction *simple,
                   GVariant *parameter,
                   gpointer user_data)
{
    g_simple_action_set_state (simple, parameter);

    gsize length;

    config_file_input ("gjackctl.server.port_max",
                       CONFIG_TYPE_STRING,
                       (gpointer) g_variant_get_string (parameter, &length));
}

static void
timeout_state_cb (GSimpleAction *simple,
                  GVariant *parameter,
                  gpointer user_data)
{
    g_simple_action_set_state (simple, parameter);

    gsize length;

    config_file_input ("gjackctl.server.timeout",
                       CONFIG_TYPE_STRING,
                       (gpointer) g_variant_get_string (parameter, &length));
}

static void
clocksource_state_cb (GSimpleAction *simple,
                      GVariant *parameter,
                      gpointer user_data)
{
    g_simple_action_set_state (simple, parameter);

    gsize length;

    config_file_input ("gjackctl.server.clocksource",
                       CONFIG_TYPE_STRING,
                       (gpointer) g_variant_get_string (parameter, &length));
}

static void
unlock_libs_state_cb (GSimpleAction *simple,
                      GVariant *parameter,
                      gpointer user_data)
{
    g_simple_action_set_state (simple, parameter);

    gboolean value;

    value =  g_variant_get_boolean (parameter);

    config_file_input ("gjackctl.server.unlock_libs",
                       CONFIG_TYPE_BOOL,
                       (gpointer) &value);
}

static void
no_zombies_state_cb (GSimpleAction *simple,
                     GVariant *parameter,
                     gpointer user_data)
{
    g_simple_action_set_state (simple, parameter);

    gboolean value;

    value =  g_variant_get_boolean (parameter);

    config_file_input ("gjackctl.server.no_zombies",
                       CONFIG_TYPE_BOOL,
                       (gpointer) &value);
}

static void
verbose_state_cb (GSimpleAction *simple,
                  GVariant *parameter,
                  gpointer user_data)
{
    g_simple_action_set_state (simple, parameter);

    gboolean value;

    value =  g_variant_get_boolean (parameter);

    config_file_input ("gjackctl.server.verbose",
                       CONFIG_TYPE_BOOL,
                       (gpointer) &value);
}

static void
no_memlock_state_cb (GSimpleAction *simple,
                     GVariant *parameter,
                     gpointer user_data)
{
    g_simple_action_set_state (simple, parameter);

    gboolean value;

    value =  g_variant_get_boolean (parameter);

    config_file_input ("gjackctl.server.no_memlock",
                       CONFIG_TYPE_BOOL,
                       (gpointer) &value);
}

static void
midi_state_cb (GSimpleAction *simple,
               GVariant *parameter,
               gpointer user_data)
{
    g_simple_action_set_state (simple, parameter);

    gboolean value;

    value =  g_variant_get_boolean (parameter);

    config_file_input ("gjackctl.server.midi",
                       CONFIG_TYPE_BOOL,
                       (gpointer) &value);
}

static void
rt_state_cb (GSimpleAction *simple,
             GVariant *parameter,
             gpointer user_data)
{
    g_simple_action_set_state (simple, parameter);

    gboolean value;

    value =  g_variant_get_boolean (parameter);
    config_file_input ("gjackctl.server.realtime",
                       CONFIG_TYPE_BOOL,
                       (gpointer) &value);
}

static gboolean
popover_destroy_cb (GtkWidget *popover,
                    GdkEvent  *event,
                    gpointer   user_data)
{
    gtk_widget_destroy (popover);

    return TRUE;
}

static void
gjackctl_settings_cb (GtkButton *button, gpointer user_data)
{/*
    GtkPassedMainData *rdata;

    rdata = user_data;

    GtkWidget *popover;

    GMenu *main_menu;
    GMenu *options1_submenu;
    GMenu *driver_menu;
    GMenu *server_section;
    GMenu *driver_section;
    GMenu *clocksource_submenu;
    GMenu *priority_submenu;
    GMenu *period_submenu;
    GMenu *timeout_submenu;
    GMenu *port_max_submenu;
    GMenu *driver_submenu;
    GMenu *sample_rate_submenu;
    GMenu *frames_submenu;

    GMenuItem *rt_item;
    GMenuItem *midi_item;
    GMenuItem *no_memlock_item;
    GMenuItem *verbose_item;
    GMenuItem *no_zombies_item;
    GMenuItem *unlock_libs_item;
    GMenuItem *clocksource_item1;
    GMenuItem *clocksource_item2;
    GMenuItem *clocksource_item3;
    GMenuItem *priority_item;
    GMenuItem *period_item;
    GMenuItem *timeout_item1;
    GMenuItem *timeout_item2;
    GMenuItem *timeout_item3;
    GMenuItem *timeout_item4;
    GMenuItem *timeout_item5;
    GMenuItem *timeout_item6;
    GMenuItem *port_max_item1;
    GMenuItem *port_max_item2;
    GMenuItem *port_max_item3;
    GMenuItem *port_max_item4;
    GMenuItem *sample_rate_item1;
    GMenuItem *sample_rate_item2;
    GMenuItem *sample_rate_item3;
    GMenuItem *sample_rate_item4;
    GMenuItem *sample_rate_item5;
    GMenuItem *sample_rate_item6;
    GMenuItem *sample_rate_item7;
    GMenuItem *frames_item1;
    GMenuItem *frames_item2;
    GMenuItem *frames_item3;
    GMenuItem *frames_item4;
    GMenuItem *frames_item5;
    GMenuItem *frames_item6;
    GMenuItem *frames_item7;
    GMenuItem *frames_item8;
    GMenuItem *frames_item9;

    GSimpleActionGroup *group;
    GSimpleAction *rt_action;
    GSimpleAction *midi_action;
    GSimpleAction *no_memlock_action;
    GSimpleAction *verbose_action;
    GSimpleAction *no_zombies_action;
    GSimpleAction *unlock_libs_action;
    GSimpleAction *clocksource_action;
    GSimpleAction *priority_action;
    GSimpleAction *timeout_action;
    GSimpleAction *port_max_action;
    GSimpleAction *alsa_action;
    GSimpleAction *sample_rate_action;
    GSimpleAction *frames_action;
    GSimpleAction *period_action;

    GVariant *rt_variant;
    GVariant *midi_variant;
    GVariant *no_memlock_variant;
    GVariant *verbose_variant;
    GVariant *no_zombies_variant;
    GVariant *unlock_libs_variant;
    GVariant *clocksource_variant;
    GVariant *timeout_variant;
    GVariant *port_max_variant;
    GVariant *alsa_variant;
    GVariant *sample_rate_variant;
    GVariant *frames_variant;

    main_menu = g_menu_new ();
    options1_submenu = g_menu_new ();
    driver_menu = g_menu_new ();
    server_section = g_menu_new ();
    driver_section = g_menu_new ();
    clocksource_submenu = g_menu_new ();
    period_submenu = g_menu_new ();
    priority_submenu = g_menu_new ();
    timeout_submenu = g_menu_new ();
    port_max_submenu = g_menu_new ();
    driver_submenu = g_menu_new ();
    sample_rate_submenu = g_menu_new ();
    frames_submenu = g_menu_new ();
    group = g_simple_action_group_new ();

    rt_variant = g_variant_new_boolean (get_realtime ());
    midi_variant = g_variant_new_boolean (get_midi ());
    no_memlock_variant = g_variant_new_boolean (get_no_memlock ());
    verbose_variant = g_variant_new_boolean (get_verbose ());
    no_zombies_variant = g_variant_new_boolean (get_no_zombies ());
    unlock_libs_variant = g_variant_new_boolean (get_unlock_libs ());
    clocksource_variant = g_variant_new_string (get_clocksource ());
    timeout_variant = g_variant_new_string (get_timeout ());
    port_max_variant = g_variant_new_string (get_port_max ());
    alsa_variant = g_variant_new_string (get_driver_device ());
    sample_rate_variant = g_variant_new_string (get_sample_rate ());
    frames_variant = g_variant_new_string (get_frames ());

    /* Our actions. */
    /*rt_action = g_simple_action_new_stateful ("realtime",
                                              NULL,
                                              rt_variant);

    midi_action = g_simple_action_new_stateful ("midi",
                                                NULL,
                                                midi_variant);

    no_memlock_action = g_simple_action_new_stateful ("no-memlock",
                                                      NULL,
                                                      no_memlock_variant);

    verbose_action = g_simple_action_new_stateful ("verbose",
                                                   NULL,
                                                   verbose_variant);

    no_zombies_action = g_simple_action_new_stateful ("no-zombies",
                                                      NULL,
                                                      no_zombies_variant);
    
    unlock_libs_action = g_simple_action_new_stateful ("unlock-libs",
                                                       NULL,
                                                       unlock_libs_variant);
    
    clocksource_action = g_simple_action_new_stateful ("clocksource",
                                                       G_VARIANT_TYPE_STRING,
                                                       clocksource_variant);

    priority_action = g_simple_action_new ("priority", NULL);

    period_action = g_simple_action_new ("period-size", NULL);

    timeout_action = g_simple_action_new_stateful ("timeout",
                                                   G_VARIANT_TYPE_STRING,
                                                   timeout_variant);

    port_max_action = g_simple_action_new_stateful ("port-max",
                                                    G_VARIANT_TYPE_STRING,
                                                    port_max_variant);

    alsa_action = g_simple_action_new_stateful ("alsa",
                                                G_VARIANT_TYPE_STRING,
                                                alsa_variant);

    sample_rate_action = g_simple_action_new_stateful ("sample-rate",
                                                       G_VARIANT_TYPE_STRING,
                                                       sample_rate_variant);

    frames_action = g_simple_action_new_stateful ("frames",
                                                  G_VARIANT_TYPE_STRING,
                                                  frames_variant);

    /* Add our actions to the action group. */
    /*g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (rt_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (midi_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (no_memlock_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (verbose_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (no_zombies_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (unlock_libs_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (clocksource_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (priority_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (timeout_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (port_max_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (alsa_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (sample_rate_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (frames_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (period_action));

    alsa_device_names (driver_submenu);
    //g_menu_item_set_attribute (G_MENU_ITEM ())

    /* Initialize our menu items. */
    /*rt_item = g_menu_item_new ("Realtime", "settings.realtime");
    midi_item = g_menu_item_new ("MIDI", "settings.midi");
    no_memlock_item = g_menu_item_new ("No Memlock", "settings.no-memlock");
    verbose_item = g_menu_item_new ("Verbose", "settings.verbose");
    no_zombies_item = g_menu_item_new ("No Zombies", "settings.no-zombies");
    unlock_libs_item = g_menu_item_new ("Unlock Libraries", "settings.unlock-libs");
    clocksource_item1 = g_menu_item_new ("Cycle", NULL);
    clocksource_item2 = g_menu_item_new ("Hpet", NULL);  
    clocksource_item3 = g_menu_item_new ("System", NULL);
    priority_item = g_menu_item_new (get_priority (), "settings.priority");
    period_item = g_menu_item_new (get_period (), "settings.period-size");
    timeout_item1 = g_menu_item_new ("200", NULL);
    timeout_item2 = g_menu_item_new ("500", NULL);
    timeout_item3 = g_menu_item_new ("1000", NULL);
    timeout_item4 = g_menu_item_new ("2000", NULL);
    timeout_item5 = g_menu_item_new ("5000", NULL);
    timeout_item6 = g_menu_item_new ("10000", NULL);
    port_max_item1 = g_menu_item_new ("128", NULL);
    port_max_item2 = g_menu_item_new ("256", NULL);
    port_max_item3 = g_menu_item_new ("512", NULL);
    port_max_item4 = g_menu_item_new ("1024", NULL);
    sample_rate_item1 = g_menu_item_new ("22.1kHz", NULL);
    sample_rate_item2 = g_menu_item_new ("32kHz", NULL);
    sample_rate_item3 = g_menu_item_new ("44.1kHz", NULL);
    sample_rate_item4 = g_menu_item_new ("48kHz", NULL);
    sample_rate_item5 = g_menu_item_new ("88.2kHz", NULL);
    sample_rate_item6 = g_menu_item_new ("96kHz", NULL);
    sample_rate_item7 = g_menu_item_new ("192kHz", NULL);
    frames_item1 = g_menu_item_new ("16", NULL);
    frames_item2 = g_menu_item_new ("32", NULL);
    frames_item3 = g_menu_item_new ("64", NULL);
    frames_item4 = g_menu_item_new ("128", NULL);
    frames_item5 = g_menu_item_new ("256", NULL);
    frames_item6 = g_menu_item_new ("512", NULL);
    frames_item7 = g_menu_item_new ("1024", NULL);
    frames_item8 = g_menu_item_new ("2048", NULL);
    frames_item9 = g_menu_item_new ("4096", NULL);

    /* Here we create radio buttons for the 'clocksource_submenu'. */
    /*g_menu_item_set_action_and_target_value (clocksource_item1,
                                             "settings.clocksource",
                                             g_variant_new_string ("Cycle"));
    g_menu_item_set_action_and_target_value (clocksource_item2,
                                             "settings.clocksource",
                                             g_variant_new_string ("Hpet"));
    g_menu_item_set_action_and_target_value (clocksource_item3,
                                             "settings.clocksource",
                                             g_variant_new_string ("System"));

    /* Here we create radio buttons for the 'timeout_submenu'. */
    /*g_menu_item_set_action_and_target_value (timeout_item1,
                                             "settings.timeout",
                                             g_variant_new_string ("200"));
    g_menu_item_set_action_and_target_value (timeout_item2,
                                             "settings.timeout",
                                             g_variant_new_string ("500"));
    g_menu_item_set_action_and_target_value (timeout_item3,
                                             "settings.timeout",
                                             g_variant_new_string ("1000"));
    g_menu_item_set_action_and_target_value (timeout_item4,
                                             "settings.timeout",
                                             g_variant_new_string ("2000"));
    g_menu_item_set_action_and_target_value (timeout_item5,
                                             "settings.timeout",
                                             g_variant_new_string ("5000"));
    g_menu_item_set_action_and_target_value (timeout_item6,
                                             "settings.timeout",
                                             g_variant_new_string ("10000"));

    

    /* Here we create radio buttons for the 'port_max_ submenu'. */
    /*g_menu_item_set_action_and_target_value (port_max_item1,
                                             "settings.port-max",
                                             g_variant_new_string ("128"));
    g_menu_item_set_action_and_target_value (port_max_item2,
                                             "settings.port-max",
                                             g_variant_new_string ("256"));
    g_menu_item_set_action_and_target_value (port_max_item3,
                                             "settings.port-max",
                                             g_variant_new_string ("512"));
    g_menu_item_set_action_and_target_value (port_max_item4,
                                             "settings.port-max",
                                             g_variant_new_string ("1024"));  

    /* Here we create radio buttons for the 'sample_rate_submenu'. */ 
    /*g_menu_item_set_action_and_target_value (sample_rate_item1,
                                             "settings.sample-rate",
                                             g_variant_new_string ("22050"));
    g_menu_item_set_action_and_target_value (sample_rate_item2,
                                             "settings.sample-rate",
                                             g_variant_new_string ("32000"));
    g_menu_item_set_action_and_target_value (sample_rate_item3,
                                             "settings.sample-rate",
                                             g_variant_new_string ("44100"));
    g_menu_item_set_action_and_target_value (sample_rate_item4,
                                             "settings.sample-rate",
                                             g_variant_new_string ("48000"));
    g_menu_item_set_action_and_target_value (sample_rate_item5,
                                             "settings.sample-rate",
                                             g_variant_new_string ("88200"));
    g_menu_item_set_action_and_target_value (sample_rate_item6,
                                             "settings.sample-rate",
                                             g_variant_new_string ("96000"));
    g_menu_item_set_action_and_target_value (sample_rate_item7,
                                             "settings.sample-rate",
                                             g_variant_new_string ("192000"));

    g_menu_item_set_action_and_target_value (frames_item1,
                                             "settings.frames",
                                             g_variant_new_string ("16"));
    g_menu_item_set_action_and_target_value (frames_item2,
                                             "settings.frames",
                                             g_variant_new_string ("32"));
    g_menu_item_set_action_and_target_value (frames_item3,
                                             "settings.frames",
                                             g_variant_new_string ("64"));
    g_menu_item_set_action_and_target_value (frames_item4,
                                             "settings.frames",
                                             g_variant_new_string ("128"));
    g_menu_item_set_action_and_target_value (frames_item5,
                                             "settings.frames",
                                             g_variant_new_string ("256"));
    g_menu_item_set_action_and_target_value (frames_item6,
                                             "settings.frames",
                                             g_variant_new_string ("512"));
    g_menu_item_set_action_and_target_value (frames_item7,
                                             "settings.frames",
                                             g_variant_new_string ("1024"));
    g_menu_item_set_action_and_target_value (frames_item8,
                                             "settings.frames",
                                             g_variant_new_string ("2048"));
    g_menu_item_set_action_and_target_value (frames_item9,
                                             "settings.frames",
                                             g_variant_new_string ("4096"));                                      

    /* 'server_section' is a GMenu that contains the server options for JACK. */
    /*g_menu_insert_section (main_menu,
                           0,
                           "Server Settings",
                           G_MENU_MODEL (server_section)); 

    /* 'driver_section' is a GMenu that contains the driver options for JACK. */
    /*g_menu_insert_section (main_menu,
                           1,
                           "Driver Settings",
                           G_MENU_MODEL (driver_section));   

    g_menu_insert_item (server_section,
                        1,
                        rt_item);

    g_menu_insert_item (server_section,
                        2,
                        midi_item);

    g_menu_insert_item (priority_submenu,
                        0,
                        priority_item);

    g_menu_insert_submenu (server_section,
                           0,
                           "RT Priority",
                           G_MENU_MODEL (priority_submenu));

    /* Pack 'clocksource_submenu'. */
    /*g_menu_insert_item (clocksource_submenu,
                        0,
                        clocksource_item1);

    g_menu_insert_item (clocksource_submenu,
                        1,
                        clocksource_item2);

    g_menu_insert_item (clocksource_submenu,
                        2,
                        clocksource_item3);    

    g_menu_insert_submenu (server_section,
                           3,
                           "Clocksource",
                           G_MENU_MODEL (clocksource_submenu));
    
    /* Pack 'timeout_submenu'. */
    /*g_menu_insert_item (timeout_submenu,
                        0,
                        timeout_item1);
    
    g_menu_insert_item (timeout_submenu,
                        1,
                        timeout_item2);

    g_menu_insert_item (timeout_submenu,
                        2,
                        timeout_item3);

    g_menu_insert_item (timeout_submenu,
                        3,
                        timeout_item4);

    g_menu_insert_item (timeout_submenu,
                        4,
                        timeout_item5);

    g_menu_insert_item (timeout_submenu,
                        5,
                        timeout_item6);

    /* Add 'timeout_submenu' to 'options1_submenu'. */
    /*g_menu_insert_submenu (options1_submenu,
                           4,
                           "Timeout",
                           G_MENU_MODEL (timeout_submenu));   

    /* Pack 'port_max_submenu'. */ 
    /*g_menu_insert_item (port_max_submenu,
                        0,
                        port_max_item1);  

    g_menu_insert_item (port_max_submenu,
                        1,
                        port_max_item2);

    g_menu_insert_item (port_max_submenu,
                        2,
                        port_max_item3);

    g_menu_insert_item (port_max_submenu,
                        3,
                        port_max_item4);

    /* Add 'port_max_submenu' to 'options1_submenu'. */
    /*g_menu_insert_submenu (options1_submenu,
                           5,
                           "Port Max",
                           G_MENU_MODEL (port_max_submenu)); 


    g_menu_insert_submenu (server_section,
                           4,
                           "Other Server Options",
                           G_MENU_MODEL (options1_submenu));

    g_menu_insert_item (options1_submenu,
                        0,
                        no_memlock_item);

    g_menu_insert_item (options1_submenu,
                        1,
                        verbose_item);

    g_menu_insert_item (options1_submenu,
                        2,
                        no_zombies_item);

    g_menu_insert_item (options1_submenu,
                        3,
                        unlock_libs_item);

    g_menu_insert_submenu (driver_section,
                           0,
                           "Driver",
                           G_MENU_MODEL (driver_submenu));
    
    g_menu_insert_item (sample_rate_submenu,
                        0,
                        sample_rate_item1);

    g_menu_insert_item (sample_rate_submenu,
                        1,
                        sample_rate_item2);

    g_menu_insert_item (sample_rate_submenu,
                        2,
                        sample_rate_item3);

    g_menu_insert_item (sample_rate_submenu,
                        3,
                        sample_rate_item4);

    g_menu_insert_item (sample_rate_submenu,
                        4,
                        sample_rate_item5);

    g_menu_insert_item (sample_rate_submenu,
                        5,
                        sample_rate_item6);

    g_menu_insert_item (sample_rate_submenu,
                        6,
                        sample_rate_item7);

    g_menu_insert_submenu (driver_section,
                           1,
                           "Sample Rate",
                           G_MENU_MODEL (sample_rate_submenu));

    g_menu_insert_item (frames_submenu,
                        0,
                        frames_item1);

    g_menu_insert_item (frames_submenu,
                        1,
                        frames_item2);
    
    g_menu_insert_item (frames_submenu,
                        2,
                        frames_item3);

    g_menu_insert_item (frames_submenu,
                        3,
                        frames_item4);

    g_menu_insert_item (frames_submenu,
                        4,
                        frames_item5);

    g_menu_insert_item (frames_submenu,
                        5,
                        frames_item6);

    g_menu_insert_item (frames_submenu,
                        6,
                        frames_item7);

    g_menu_insert_item (frames_submenu,
                        7,
                        frames_item8);

    g_menu_insert_item (frames_submenu,
                        8,
                        frames_item9);

    g_menu_insert_submenu (driver_section,
                           2,
                           "Frames",
                           G_MENU_MODEL (frames_submenu));

    g_menu_insert_item (period_submenu,
                        0,
                        period_item);

    g_menu_insert_submenu (driver_section,
                           3,
                           "Period Size",
                           G_MENU_MODEL (period_submenu));

    g_menu_insert_submenu (driver_section,
                           4,
                           "Other Driver Options",
                           G_MENU_MODEL (driver_menu));




    popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                          G_MENU_MODEL (main_menu));

    gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_BOTTOM);

    gtk_widget_insert_action_group (popover,
                                    "settings",
                                    G_ACTION_GROUP (group));

    g_object_unref (driver_section);
    g_object_unref (server_section);
    g_object_unref (period_submenu);
    g_object_unref (frames_submenu);
    g_object_unref (sample_rate_submenu);
    g_object_unref (driver_submenu);
    g_object_unref (options1_submenu);
    g_object_unref (port_max_submenu);
    g_object_unref (timeout_submenu);
    g_object_unref (clocksource_submenu);
    g_object_unref (priority_submenu);
    g_object_unref (main_menu);

    g_signal_connect (rt_action,
                      "change-state",
                      G_CALLBACK (rt_state_cb),
                      NULL);

    g_signal_connect (midi_action,
                      "change-state",
                      G_CALLBACK (midi_state_cb),
                      NULL);
    
    g_signal_connect (no_memlock_action,
                      "change-state",
                      G_CALLBACK (no_memlock_state_cb),
                      NULL);

    g_signal_connect (verbose_action,
                      "change-state",
                      G_CALLBACK (verbose_state_cb),
                      NULL);

    g_signal_connect (no_zombies_action,
                      "change-state",
                      G_CALLBACK (no_zombies_state_cb),
                      NULL);    

    g_signal_connect (unlock_libs_action,
                      "change-state",
                      G_CALLBACK (unlock_libs_state_cb),
                      NULL);

    g_signal_connect (clocksource_action,
                      "change-state",
                      G_CALLBACK (clocksource_state_cb),
                      NULL);

    g_signal_connect (timeout_action,
                      "change-state",
                      G_CALLBACK (timeout_state_cb),
                      NULL);

    g_signal_connect (port_max_action,
                      "change-state",
                      G_CALLBACK (port_max_state_cb),
                      NULL);

    g_signal_connect (priority_action,
                      "activate",
                      G_CALLBACK (activate_priority_cb),
                      rdata);        
    
    g_signal_connect (alsa_action,
                      "change-state",
                      G_CALLBACK (alsa_state_cb),
                      NULL);

    g_signal_connect (sample_rate_action,
                      "change-state",
                      G_CALLBACK (sample_rate_state_cb),
                      NULL);

    g_signal_connect (frames_action,
                      "change-state",
                      G_CALLBACK (frames_state_cb),
                      NULL);

    g_signal_connect (period_action,
                      "activate",
                      G_CALLBACK (period_size_cb),
                      rdata);

    g_signal_connect (popover,
                      "focus-out-event",
                      G_CALLBACK (popover_destroy_cb),
                      NULL);

    gtk_widget_show_all (popover);*/

    GtkWidget *settings_window;
    GtkWidget *stack;
    GtkWidget *sswitcher;
    GtkWidget *server_grid;
    GtkWidget *driver_grid;
    GtkWidget *header_bar;
    GtkWidget *sep;
    GSimpleActionGroup *group;
    GtkPassedMainData *rdata;
    GtkWidget *server_features_frame;
    GtkWidget *server_rt_frame;
    GtkWidget *features_frame_grid;
    GtkWidget *rt_frame_grid;

    rdata = user_data;
    settings_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    stack = gtk_stack_new ();
    sswitcher = gtk_stack_switcher_new ();
    server_grid = gtk_grid_new ();
    driver_grid = gtk_grid_new ();
    header_bar = gtk_header_bar_new ();
    sep = gtk_separator_new (GTK_ORIENTATION_VERTICAL);
    group = g_simple_action_group_new ();
    server_features_frame = gtk_frame_new ("Features");
    server_rt_frame = gtk_frame_new ("Realtime Options");
    features_frame_grid = gtk_grid_new ();
    rt_frame_grid = gtk_grid_new ();

    gtk_window_set_transient_for (GTK_WINDOW (settings_window),
                                  GTK_WINDOW (rdata -> window));

    gtk_window_set_attached_to (GTK_WINDOW (settings_window),
                                GTK_WIDGET (button));

    gtk_stack_add_titled (GTK_STACK (stack),
                          server_grid,
                          "server",
                          "Server");

    gtk_stack_add_titled (GTK_STACK (stack),
                          driver_grid,
                          "driver",
                          "Driver");

    gtk_stack_switcher_set_stack (GTK_STACK_SWITCHER (sswitcher),
                                  GTK_STACK (stack));

    gtk_widget_insert_action_group (GTK_WIDGET (button),
                                    "settings",
                                    G_ACTION_GROUP (group));

    /* Pack server grid. */
    server_name (server_grid);
    timeout (server_grid);
    port_max (server_grid);
    clocksource (server_grid);
    toggle_rt (features_frame_grid);
    toggle_midi (features_frame_grid);
    toggle_no_memlock (features_frame_grid);
    toggle_no_zombies (features_frame_grid);
    toggle_unlock_libs (features_frame_grid);
    toggle_verbose (features_frame_grid);
    rt_priority (rt_frame_grid);
    gtk_container_add (GTK_CONTAINER (server_features_frame), features_frame_grid);
    gtk_container_add (GTK_CONTAINER (server_rt_frame), rt_frame_grid);
    gtk_grid_attach (GTK_GRID (server_grid), server_features_frame, 0, 4, 2, 1);
    gtk_grid_attach (GTK_GRID (server_grid), server_rt_frame, 0, 5, 2, 1);

    /* Pack driver grid. */
    drivers (driver_grid);
    sample_rate (driver_grid);

    gtk_frame_set_shadow_type (GTK_FRAME (server_features_frame),
                               GTK_SHADOW_NONE);
    gtk_frame_set_shadow_type (GTK_FRAME (server_rt_frame),
                               GTK_SHADOW_NONE);

    gtk_grid_set_row_spacing (GTK_GRID (server_grid), 4);
    gtk_grid_set_row_spacing (GTK_GRID (driver_grid), 4);
    gtk_widget_set_margin_start (server_grid, 10);
    gtk_widget_set_margin_end (server_grid, 8);
    gtk_widget_set_margin_start (driver_grid, 8);
    gtk_widget_set_margin_end (driver_grid, 8);
    gtk_widget_set_margin_top (server_grid, 8);
    gtk_widget_set_margin_top (driver_grid, 8);
    gtk_widget_set_margin_start (server_features_frame, 10);
    gtk_widget_set_margin_start (server_rt_frame, 10);
    gtk_widget_set_margin_top (server_features_frame, 10);

    gtk_header_bar_set_custom_title (GTK_HEADER_BAR (header_bar), sswitcher);
    gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), sep);
    gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header_bar), TRUE);
    gtk_header_bar_set_decoration_layout (GTK_HEADER_BAR (header_bar), ":close");
    gtk_window_set_titlebar (GTK_WINDOW (settings_window), header_bar);

    gtk_window_set_default_size (GTK_WINDOW (settings_window), 350, 200);
    gtk_container_add (GTK_CONTAINER (settings_window), stack);

    gtk_widget_show_all (settings_window);
}

void
gjackctl_settings (GtkPassedMainData *pdata)
{
	GtkWidget *settings_button;
	//GtkWidget *gear;

	settings_button = gtk_button_new_from_icon_name ("open-menu-symbolic",
                                                     GTK_ICON_SIZE_SMALL_TOOLBAR);

	//gear = gtk_image_new_from_file ("/usr/share/icons/Adwaita/scalable/actions/open-menu-symbolic.svg");

	gtk_widget_set_size_request (settings_button, 30, 34);
	gtk_widget_set_tooltip_text (settings_button, "Settings");
	//gtk_button_set_image (GTK_BUTTON (settings_button), gear);

	gtk_header_bar_pack_end (GTK_HEADER_BAR (pdata -> header_bar), settings_button);

	g_signal_connect (settings_button, 
					  "clicked", 
					  G_CALLBACK (gjackctl_settings_cb), 
					  pdata);
}

