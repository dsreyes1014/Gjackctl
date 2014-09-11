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
visible_child_cb (GtkWidget *stack, GParamSpec *pspec, gpointer user_data)
{
    if (g_strcmp0 (gtk_stack_get_visible_child_name (GTK_STACK (stack)), "server") == 0)
    {
        gtk_window_resize (GTK_WINDOW (user_data), 550, 350);
    }
   
    if (g_strcmp0 (gtk_stack_get_visible_child_name (GTK_STACK (stack)), "driver") == 0)
    {
        gtk_window_resize (GTK_WINDOW (user_data), 530, 200);
    } 
}

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

static void
gjackctl_settings_cb (GtkButton *button, gpointer user_data)
{	
	GtkWidget *popup;
	GtkWidget *header_bar;
	GtkWidget *button1;
	GtkWidget *button2;
	GtkWidget *window;
	GtkWidget *stack;
	GtkWidget *sswitcher;	
	GtkApplication *app;
    GtkWidget *sbox0;
    GtkWidget *sbox1;
    GtkWidget *sbox2;
    GtkWidget *sbox3;
	GtkWidget *sbox4;
    GtkWidget *sbox5;
    GtkWidget *sbox6;
    GtkWidget *sbox7;
    GtkWidget *dbox;
    GtkWidget *dbox2;
    GtkWidget *separator;

	/* This is a `struct` that holds variables passed by 
	the `g_signal_connect ()` function through `gpointer user_data`. */ 
	pass_data *data_received;
	pass_data_2 *data_to_pass;	

    sbox0 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);    
    sbox1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);    
    sbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2); 
    sbox3 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);   
    sbox4 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2); 
    sbox5 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);   
    sbox6 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    sbox7 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);  
    dbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    dbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);    
    stack = gtk_stack_new ();
	sswitcher = gtk_stack_switcher_new ();
	data_to_pass = (pass_data_2 *) g_malloc (sizeof (pass_data_2));
	data_received = user_data;
	app = data_received -> data2;
	button1 = gtk_button_new_with_label ("OK");
	button2 = gtk_button_new_with_label ("Cancel");
	popup = gtk_application_window_new (GTK_APPLICATION (app));
	header_bar = gtk_header_bar_new ();
	window = data_received -> data1;
	data_to_pass -> data1 = window;
	data_to_pass -> data2 = popup;
    separator = gtk_separator_new (GTK_ORIENTATION_VERTICAL);	

    gtk_stack_add_titled (GTK_STACK (stack),
						  sbox0,
                          "server",
                          "Server");
	gtk_stack_add_titled (GTK_STACK (stack),
						  dbox,
                          "driver",
                          "Driver");
    
	gtk_stack_switcher_set_stack (GTK_STACK_SWITCHER (sswitcher),
                                  GTK_STACK (stack)); 
	gtk_stack_set_transition_type (GTK_STACK (stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);

	/* Pack `box` into `stack` named `server`. */
    gtk_box_pack_start (GTK_BOX (sbox0), sbox3, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (sbox0), separator, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (sbox0), sbox1, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (sbox1), sbox2, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (sbox4), sbox5, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (sbox4), sbox6, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (sbox1), sbox4, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (sbox3), sbox7, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (dbox), dbox2, FALSE, FALSE, 2);    
    /**************************************************************************/
	server_name (sbox2, button1);		
	toggle_rt (sbox5, button1);
	toggle_no_memlock (sbox5, button1);
    toggle_midi (sbox5, button1);
    toggle_no_zombies (sbox6, button1);
    toggle_unlock_libs (sbox6, button1);
	rt_priority (sbox2, button1);
    clocksource (sbox7, button1);
    port_max (sbox7, button1);
    timeout (sbox7, button1);
    /**************************************************************************/    
	drivers (dbox2, app, button1);
	sample_rate (dbox2, button1);
    frames (dbox2, button1);    
    period (dbox2, button1);

    gtk_window_set_default_size (GTK_WINDOW (popup), 600, 220);    
    gtk_widget_set_size_request (button1, 80, 30);
    gtk_widget_set_margin_start (sbox5, 60);
    
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

    g_signal_connect (stack,
                      "notify::visible-child-name",
                      G_CALLBACK (visible_child_cb),
                      popup);

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

