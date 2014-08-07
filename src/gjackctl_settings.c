#include "gjackctl_settings.h"

typedef struct 
{
	GtkWidget *data1;
	GtkApplication *data2;
}pass_data;

typedef struct
{
	GtkWidget *data1;
	GtkWidget *data2;
}pass_data_2;

static void
popup_destroy_cb (GtkWidget *widget, gpointer user_data)
{
	GtkWidget *window;
	
	window = user_data;

	gtk_widget_destroy (widget);
	gtk_widget_show_all (window);
}

void
button_clicked_cb (GtkButton *button, gpointer user_data)
{
	GtkWidget *widget1;
	GtkWidget *widget2;

	/* This is a `struct` that holds variables passed by 
	the `g_signal_connect ()` function through `gpointer user_data`. */
	pass_data_2 *data_received;
	
	data_received = user_data;
	widget1 = data_received -> data1;
	widget2 = data_received -> data2;

	gtk_widget_destroy (widget2);
	gtk_widget_show_all (widget1);
	g_free (data_received);
}

/*static PangoAttrList *
label_settings_attr ()
{
	PangoAttrList *list;
	PangoAttribute *attr;

	list = pango_attr_list_new ();

	attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
	pango_attr_list_insert (list, attr);
	attr = pango_attr_weight_new (PANGO_WEIGHT_ULTRABOLD);
	pango_attr_list_insert (list, attr);	
	attr = pango_attr_size_new (14500);
	pango_attr_list_insert (list, attr);	

	return list;
}*/

static void
gjackctl_settings_cb (GtkButton *button, gpointer user_data)
{	
	GtkWidget *popup;
	GtkWidget *grid;
	GtkWidget *grid2;
    GtkWidget *grid3;
	GtkWidget *header_bar;
	GtkWidget *button1;
	GtkWidget *button2;
	GtkWidget *window;
	GtkWidget *stack;
	GtkWidget *sswitcher;	
	GtkApplication *app;
    GtkWidget *box;
    GtkWidget *box2;
    GtkWidget *box3;
	GtkWidget *box4;
    GtkWidget *box5;
    GtkWidget *box6;
    GtkWidget *frame;

	/* This is a `struct` that holds variables passed by 
	the `g_signal_connect ()` function through `gpointer user_data`. */ 
	pass_data *data_received;
	pass_data_2 *data_to_pass;	

    frame = gtk_frame_new (NULL);
    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);    /* Main settings server box. */
    box2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6); /* First box packed inside of `box`. */ 
    box3 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);   /* */
    box4 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6); /* Second box packed inside `box`. */
    box5 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);   /* First box packed inside of `box4`. */
    box6 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);   /* Second box packed inside of `box4`. */
	stack = gtk_stack_new ();
	sswitcher = gtk_stack_switcher_new ();
	data_to_pass = (pass_data_2 *) g_malloc (sizeof (pass_data_2));
	data_received = user_data;
	grid = gtk_grid_new ();
	grid2 = gtk_grid_new ();
    grid3 = gtk_grid_new ();
	app = data_received -> data2;
	button1 = gtk_button_new_with_label ("OK");
	button2 = gtk_button_new_with_label ("Cancel");
	popup = gtk_application_window_new (GTK_APPLICATION (app));
	header_bar = gtk_header_bar_new ();
	window = data_received -> data1;
	data_to_pass -> data1 = window;
	data_to_pass -> data2 = popup;

	/* `grid` and `grid2` attributes. */
	gtk_grid_set_row_spacing (GTK_GRID (grid), 4);
	gtk_grid_set_column_spacing (GTK_GRID (grid), 20);
	gtk_grid_set_row_spacing (GTK_GRID (grid2), 4);
	gtk_grid_set_column_spacing (GTK_GRID (grid2), 4);
	

	gtk_stack_add_titled (GTK_STACK (stack),
						  box,
                          "server",
                          "Server");
	gtk_stack_add_titled (GTK_STACK (stack),
						  grid2,
                          "driver",
                          "Driver");
	gtk_stack_switcher_set_stack (GTK_STACK_SWITCHER (sswitcher),
                                  GTK_STACK (stack)); 
	gtk_stack_set_transition_type (GTK_STACK (stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);

	/* Pack `grid` into `stack` named `server`. */
    //gtk_container_add (GTK_CONTAINER (frame), box3);
    gtk_box_pack_start (GTK_BOX (box), box2, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box4), box5, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box4), box6, FALSE, FALSE, 2);
    //gtk_box_pack_start (GTK_BOX (box3), box4, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (box), box4, FALSE, FALSE, 2);
	server_name (box2, button1);		
	toggle_rt (box5, button1);
	toggle_no_memlock (box5, button1);
    toggle_midi (box5, button1);
    toggle_no_zombies (box6, button1);
	rt_priority (box2, button1);
    clocksource (box, button1);
    port_max (box, button1);
    timeout (box, button1);
	drivers (grid2, app);
	sample_rate (grid2);

    //gtk_widget_set_margin_start (frame, 40);
    //gtk_widget_set_margin_end (frame, 50);
    gtk_widget_set_size_request (button1, 80, 30);
    
	/* Pack `header_bar`. */
	gtk_header_bar_set_custom_title (GTK_HEADER_BAR (header_bar), sswitcher);
	gtk_header_bar_set_title (GTK_HEADER_BAR (header_bar), "Settings");
	gtk_window_set_titlebar (GTK_WINDOW (popup), header_bar);
	gtk_header_bar_pack_start (GTK_HEADER_BAR (header_bar), button2);
	gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), button1);

	gtk_container_add (GTK_CONTAINER (popup), stack);
	
	g_signal_connect (popup, 
                      "destroy", 
					  G_CALLBACK (popup_destroy_cb), 
                      window);
	g_signal_connect (button1, 
                      "clicked", 
					  G_CALLBACK (button_clicked_cb), 
					  (gpointer) data_to_pass);
	g_signal_connect (button2, 
                      "clicked", 
                      G_CALLBACK (button_clicked_cb), 
                      data_to_pass);

	/* Position `popup` to show wherever current mouse position is located. */
	gtk_window_set_position (GTK_WINDOW (popup), GTK_WIN_POS_MOUSE);
	
	gtk_widget_show_all (popup);
	gtk_widget_hide (window);
}

void
gjackctl_settings (GtkWidget *window,
                   GtkApplication *app,
                   GtkWidget *header_bar)
{
	GtkWidget *settings_button;
	GtkWidget *gear;
	pass_data *data_to_pass;

	data_to_pass = (pass_data *) g_malloc (sizeof (pass_data));

	data_to_pass -> data1 = window;
	data_to_pass -> data2 = app;
	settings_button = gtk_button_new ();
	gear = gtk_image_new_from_file ("/usr/share/icons/gnome/scalable/emblems/emblem-system-symbolic.svg");

	gtk_widget_set_size_request (settings_button, 30, 34);
	gtk_widget_set_tooltip_text (settings_button, "Settings");
	gtk_button_set_image (GTK_BUTTON (settings_button), gear);
	
	gtk_header_bar_pack_end (GTK_HEADER_BAR (header_bar), settings_button);

	g_signal_connect (settings_button, 
					  "clicked", 
					  G_CALLBACK (gjackctl_settings_cb), 
					  (gpointer) data_to_pass);
}

