#include "gjackctl_settings.h"

static void
destroy_cb (GtkWidget *widget,
            gpointer   user_data)
{
    gtk_widget_destroy (widget);
}

static void
gjackctl_settings_cb (GtkButton *button, gpointer user_data)
{
    GtkWidget *settings_window;
    GtkWidget *stack;
    GtkWidget *sswitcher;
    GtkWidget *server_grid;
    GtkWidget *driver_grid;
    GtkWidget *header_bar;
    GtkWidget *sep;
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
    server_features_frame = gtk_frame_new ("Features");
    server_rt_frame = gtk_frame_new ("Realtime Options");
    features_frame_grid = gtk_grid_new ();
    rt_frame_grid = gtk_grid_new ();

    gtk_window_set_transient_for (GTK_WINDOW (settings_window),
                                  GTK_WINDOW (rdata -> window));

    gtk_window_set_modal (GTK_WINDOW (settings_window), TRUE);

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
    frames (driver_grid);
    period (driver_grid);

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
    gtk_window_set_attached_to (GTK_WINDOW (settings_window),
                                GTK_WIDGET (button));

    g_signal_connect (settings_window, "destroy", G_CALLBACK (destroy_cb), NULL);

    gtk_widget_show_all (settings_window);
}

void
gjackctl_settings (GtkPassedMainData *pdata)
{
	GtkWidget *settings_button;

	settings_button = gtk_button_new_from_icon_name ("open-menu-symbolic",
                                                     GTK_ICON_SIZE_SMALL_TOOLBAR);

	gtk_widget_set_tooltip_text (settings_button, "Settings");

	gtk_header_bar_pack_end (GTK_HEADER_BAR (pdata -> header_bar), settings_button);

	g_signal_connect (settings_button,
					  "clicked",
					  G_CALLBACK (gjackctl_settings_cb),
					  pdata);
}

