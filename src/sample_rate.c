#include "sample_rate.h"

typedef struct _passed_data
{
	GtkWidget *passed_button;
	GtkWidget *passed_popover;
	GSList *passed_list;
}passed_data;

static gchar *
get_sample_rate_string ()
{
	gchar **argv;
	gchar *string;
	gint argc;
	gint i;

	argv = get_arg_vector ();
	argc = 0;
	i = 0;

	/* Get arg count */
	while (argv[argc])
	{
		argc++;
	}

	for (i = 0; i < argc; i++)
	{
		if ((strncmp (argv[i], "-r", 2) == 0) &&
			(strncmp (argv[i - 1], "-d", 2) == 0))
		{
			string = g_strdup (argv[i]);

			break;
		}
		else if (i == argc)
		{
			return NULL;
		}
	}

	g_strfreev (argv);

	return &string[2];
}

static void
rates_cb (GtkToggleButton *tb, gpointer user_data)
{
	passed_data *received_data;

	received_data = user_data;
	
	while (received_data -> passed_list)
	{
		tb = received_data -> passed_list -> data;

		if (gtk_toggle_button_get_active (tb) == TRUE)
		{
			gchar *arg_rate_const;
			gchar **argv;
			gchar *string;
			gint i;
			gint argc;
			const gchar *rate_string;
				
			arg_rate_const = g_strdup ("-r");	
			rate_string = gtk_button_get_label (GTK_BUTTON (tb));		
			string = g_strconcat (arg_rate_const, rate_string, NULL);
			argc = 0;
			argv = get_arg_vector ();

			/* Get arg count */
			while (argv[argc])
			{
				argc++;
			}

			for (i = 0; i < argc; i++)
			{
				/* If sample rate arg does not match and arg element before
				current element being evaluated is driver arg then change 
				sample rate to user-chosen sample rate. */
				if ((i > 0) && 
					(g_strcmp0 (argv[i], string) != 0) &&
					(strncmp (argv[i - 1], "-d", 2) == 0) &&
					(strncmp (argv[i], "-d", 2) != 0))
				{
					argv[i] = g_strdup (string);
					break;
				}
			}
	
			config_file_input (argv, argc);			

			gtk_button_set_label (GTK_BUTTON (received_data -> passed_button), rate_string);

			g_print ("Button number %s selected\n", 
					 gtk_button_get_label (GTK_BUTTON (tb)));
			break;
		}

		received_data -> passed_list = received_data -> passed_list -> next;
	}
	
	gtk_widget_destroy (received_data -> passed_popover);
}

static void
sample_rate_popover_cb (GtkWidget *button, gpointer user_data)
{
	GtkWidget *box;
	GtkWidget *radio1;
	GtkWidget *radio2;
	GtkWidget *radio3;
	GtkWidget *radio4;
	GtkWidget *radio5;
	GtkWidget *radio6;
	GtkWidget *radio7;
	passed_data *received_data;
	gchar *string;

	received_data = user_data;
	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	received_data -> passed_popover = gtk_popover_new (button);
	string = get_sample_rate_string ();
	
	/* `radio1` initiates group of radio buttons. */
	radio1 = gtk_radio_button_new_with_label (NULL, "22050");
	radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "32000");
	radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "44100");
	radio4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "48000");
	radio5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "88200");	
	radio6 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "96000");
	radio7 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "192000");
	received_data -> passed_list = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

	/* Here we check what the string if it matches the label in the radiobutton
	and select that button accordingly. */
	if (g_strcmp0 (string, gtk_button_get_label (GTK_BUTTON (radio1))) == 0)
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio1), TRUE);
	}
	else if (g_strcmp0 (string, gtk_button_get_label (GTK_BUTTON (radio2))) == 0)
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio2), TRUE);
	}
	else if (g_strcmp0 (string, gtk_button_get_label (GTK_BUTTON (radio3))) == 0)
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio3), TRUE);
	}
	else if (g_strcmp0 (string, gtk_button_get_label (GTK_BUTTON (radio4))) == 0)
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio4), TRUE);
	}
	else if (g_strcmp0 (string, gtk_button_get_label (GTK_BUTTON (radio5))) == 0)
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio5), TRUE);
	}
	else if (g_strcmp0 (string, gtk_button_get_label (GTK_BUTTON (radio6))) == 0)
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio6), TRUE);
	}
	else
	{
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radio7), TRUE);
	}

	gtk_box_pack_start (GTK_BOX (box), radio1, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio2, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio3, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio4, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio5, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio6, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio7, FALSE, TRUE, 2);
	gtk_container_add (GTK_CONTAINER (received_data -> passed_popover), box);

	gtk_popover_set_position (GTK_POPOVER (received_data -> passed_popover),
							  GTK_POS_RIGHT);

	g_signal_connect (radio1, "toggled", G_CALLBACK (rates_cb), received_data);
	g_signal_connect (radio2, "toggled", G_CALLBACK (rates_cb), received_data);
	g_signal_connect (radio3, "toggled", G_CALLBACK (rates_cb), received_data);
	g_signal_connect (radio4, "toggled", G_CALLBACK (rates_cb), received_data);
	g_signal_connect (radio5, "toggled", G_CALLBACK (rates_cb), received_data);
	g_signal_connect (radio6, "toggled", G_CALLBACK (rates_cb), received_data);
	g_signal_connect (radio7, "toggled", G_CALLBACK (rates_cb), received_data);

	gtk_widget_show_all (received_data -> passed_popover);
}

void
sample_rate (GtkWidget *grid)
{
	gchar *string;
	passed_data *sample_rate_data;

	sample_rate_data = (passed_data *) g_malloc (sizeof (passed_data));

	string = get_sample_rate_string ();
	sample_rate_data -> passed_button = gtk_button_new_with_label (string);

	gtk_widget_set_tooltip_text (sample_rate_data -> passed_button,
								 "Sample Rate (hz)");
	gtk_widget_set_halign (sample_rate_data -> passed_button, GTK_ALIGN_CENTER);
	gtk_widget_set_size_request (sample_rate_data -> passed_button, 96, 34);	

	gtk_grid_attach (GTK_GRID (grid), sample_rate_data -> passed_button, 1, 6, 1, 1);
	
	g_signal_connect (sample_rate_data -> passed_button, "clicked", 
					  G_CALLBACK (sample_rate_popover_cb), sample_rate_data);
}
