#include "gjackctl_settings.h"

typedef struct 
{
	GtkWidget *window;
	GtkApplication *app;
}GtkSettingsPassedData;

static void
close_window_cb (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    gint priority;
    gchar string[10];

    priority = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (user_data));        
    g_sprintf (string, "%d", priority);

    config_file_input ("gjackctl.server.priority",
                       CONFIG_TYPE_STRING,
                       (gpointer) string);

    gtk_widget_destroy (widget);
}

static void
activate_priority_cb (GSimpleAction *simple,
                      GVariant *parameter,
                      gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *spin_button;
    GtkAdjustment *adjustment;
    GtkSettingsPassedData *rdata;
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
                      G_CALLBACK (close_window_cb),
                      spin_button);
    
    gtk_widget_show_all (window);
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

static void
gjackctl_settings_cb (GtkButton *button, gpointer user_data)
{	
    GtkSettingsPassedData *rdata;

    rdata = user_data;

    GtkWidget *popover;

    GMenu *main_menu;
    GMenu *server_menu;
    GMenu *driver_menu;
    GMenu *server_section;
    GMenu *driver_section;
    GMenu *clocksource_submenu;
    GMenu *priority_submenu;

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

    GSimpleActionGroup *group;
    GSimpleAction *rt_action;
    GSimpleAction *midi_action;
    GSimpleAction *no_memlock_action;
    GSimpleAction *verbose_action;
    GSimpleAction *no_zombies_action;
    GSimpleAction *unlock_libs_action;
    GSimpleAction *clocksource_action;
    GSimpleAction *priority_action;

    GVariant *rt_variant;
    GVariant *midi_variant;
    GVariant *no_memlock_variant;
    GVariant *verbose_variant;
    GVariant *no_zombies_variant;
    GVariant *unlock_libs_variant;
    GVariant *clocksource_variant;

    main_menu = g_menu_new ();
    server_menu = g_menu_new ();
    driver_menu = g_menu_new ();
    server_section = g_menu_new ();
    driver_section = g_menu_new ();
    clocksource_submenu = g_menu_new ();
    priority_submenu = g_menu_new ();
    group = g_simple_action_group_new ();

    rt_variant = g_variant_new_boolean (get_realtime ());
    midi_variant = g_variant_new_boolean (get_midi ());
    no_memlock_variant = g_variant_new_boolean (get_no_memlock ());
    verbose_variant = g_variant_new_boolean (get_verbose ());
    no_zombies_variant = g_variant_new_boolean (get_no_zombies ());
    unlock_libs_variant = g_variant_new_boolean (get_unlock_libs ());
    clocksource_variant = g_variant_new_string (get_clocksource ());

    /* Our actions. */
    rt_action = g_simple_action_new_stateful ("realtime",
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

    priority_action = g_simple_action_new ("priority",
                                           NULL);

    /* Add our actions to the action group. */
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (rt_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (midi_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (no_memlock_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (verbose_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (no_zombies_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (unlock_libs_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (clocksource_action));
    g_action_map_add_action (G_ACTION_MAP (group), G_ACTION (priority_action)); 

    

    /* Attach the action group to a widget, in this case our GtkButton. */
    gtk_widget_insert_action_group (GTK_WIDGET (button),
                                    "settings",
                                    G_ACTION_GROUP (group)); 

    rt_item = g_menu_item_new ("Realtime", "settings.realtime");
    midi_item = g_menu_item_new ("MIDI", "settings.midi");
    no_memlock_item = g_menu_item_new ("No Memlock", "settings.no-memlock");
    verbose_item = g_menu_item_new ("Verbose", "settings.verbose");
    no_zombies_item = g_menu_item_new ("No Zombies", "settings.no-zombies");
    unlock_libs_item = g_menu_item_new ("Unlock Libraries", "settings.unlock-libs");
    clocksource_item1 = g_menu_item_new ("Cycle", NULL);
    clocksource_item2 = g_menu_item_new ("Hpet", NULL);  
    clocksource_item3 = g_menu_item_new ("System", NULL);
    priority_item = g_menu_item_new (get_priority (), "settings.priority");

    /* Here we create radio buttons for the clocksource submenu. */
    g_menu_item_set_action_and_target_value (clocksource_item1,
                                             "settings.clocksource",
                                             g_variant_new_string ("Cycle"));
    g_menu_item_set_action_and_target_value (clocksource_item2,
                                             "settings.clocksource",
                                             g_variant_new_string ("Hpet"));
    g_menu_item_set_action_and_target_value (clocksource_item3,
                                             "settings.clocksource",
                                             g_variant_new_string ("System"));

    /* 'server_section' is a GMenu that contains the server options for JACK. */
    g_menu_insert_section (main_menu,
                           0,
                           "Server Settings",
                           G_MENU_MODEL (server_section)); 

    /* 'driver_section' is a GMenu that contains the driver options for JACK. */
    g_menu_insert_section (main_menu,
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

    g_menu_insert_item (clocksource_submenu,
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

    g_menu_insert_submenu (server_section,
                           4,
                           "Other Options",
                           G_MENU_MODEL (server_menu));

    g_menu_insert_item (server_menu,
                        0,
                        no_memlock_item);

    g_menu_insert_item (server_menu,
                        1,
                        verbose_item);

    g_menu_insert_item (server_menu,
                        2,
                        no_zombies_item);

    g_menu_insert_item (server_menu,
                        3,
                        unlock_libs_item);

    g_menu_insert_submenu (driver_section,
                           0,
                           "Options",
                           G_MENU_MODEL (driver_menu));

    popover = gtk_popover_new_from_model (GTK_WIDGET (button),
                                          G_MENU_MODEL (main_menu));

    gtk_widget_insert_action_group (popover,
                                    "settings",
                                    G_ACTION_GROUP (group));

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

    g_signal_connect (priority_action,
                      "activate",
                      G_CALLBACK (activate_priority_cb),
                      rdata);        

    gtk_widget_show_all (popover);
}

void
gjackctl_settings (GtkWidget *window,
                   GtkApplication *app,
                   GtkWidget *header_bar)
{
	GtkWidget *settings_button;
	GtkWidget *gear;
	GtkSettingsPassedData *pdata;

	pdata = g_slice_new (GtkSettingsPassedData);

	pdata -> window = window;
	pdata -> app = app;
	settings_button = gtk_button_new ();
	gear = gtk_image_new_from_file ("/usr/share/icons/Adwaita/scalable/actions/open-menu-symbolic.svg");

	gtk_widget_set_size_request (settings_button, 30, 34);
	gtk_widget_set_tooltip_text (settings_button, "Settings");
	gtk_button_set_image (GTK_BUTTON (settings_button), gear);
	
	gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), settings_button);

	g_signal_connect (settings_button, 
					  "clicked", 
					  G_CALLBACK (gjackctl_settings_cb), 
					  pdata);
}

