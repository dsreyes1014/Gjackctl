#include "sample_rate.h"

GtkWidget *srate_button;
GtkWidget *popover;

static void
rates_cb (GtkToggleButton *tb, gpointer user_data)
{
	GSList *list;
	
	list = user_data;
	
	while (list)
	{
		tb = list -> data;

		if (gtk_toggle_button_get_active (tb) == TRUE)
		{
			gchar *arg_rate_const;
			gchar **argvp;
			gchar *string;
			gint i;
			gint argcp;
			const gchar *rate_string;
				
			arg_rate_const = g_strdup ("-r");	
			rate_string = gtk_button_get_label (GTK_BUTTON (tb));		
			string = g_strconcat (arg_rate_const, rate_string, NULL);
			argcp = 0;
			argvp = get_arg_vector ();

			/* Get arg count */
			while (argvp[argcp])
			{
				argcp++;
			}

			for (i = 0; i < argcp; i++)
			{
				/* If sample rate arg does not match and arg element before
				current element being evaluated is driver arg then change 
				sample rate to user-chosen sample rate. */
				if ((i > 0) && 
					(g_strcmp0 (argvp[i], string) != 0) &&
					(strncmp (argvp[i - 1], "-d", 2) == 0) &&
					(strncmp (argvp[i], "-d", 2) != 0))
				{
					argvp[i] = g_strdup (string);
					break;
				}
			}
	
			file_input (argvp, argcp);			

			gtk_button_set_label (GTK_BUTTON (srate_button), rate_string);

			g_print ("Button number %s selected\n", 
					 gtk_button_get_label (GTK_BUTTON (tb)));
			break;
		}

		list = list -> next;
	}
	
	gtk_widget_hide (popover);
}

static void
sample_rate_popover_cb (GtkWidget *button, gpointer data)
{
	GtkWidget *box;
	GtkWidget *radio1;
	GtkWidget *radio2;
	GtkWidget *radio3;
	GtkWidget *radio4;
	GtkWidget *radio5;
	GtkWidget *radio6;
	GtkWidget *radio7;
	GSList *group;

	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	popover = gtk_popover_new (button);

	/* `radio1` initiates group of radio buttons. */
	radio1 = gtk_radio_button_new_with_label (NULL, "22050");
	radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "32000");
	radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "44100");
	radio4 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "48000");
	radio5 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "88200");	
	radio6 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "96000");
	radio7 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "192000");
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radio1));

	gtk_box_pack_start (GTK_BOX (box), radio1, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio2, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio3, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio4, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio5, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio6, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (box), radio7, FALSE, TRUE, 2);
	gtk_container_add (GTK_CONTAINER (popover), box);

	gtk_popover_set_position (GTK_POPOVER (popover), GTK_POS_RIGHT);

	g_signal_connect (radio1, "toggled", G_CALLBACK (rates_cb), group);
	g_signal_connect (radio2, "toggled", G_CALLBACK (rates_cb), group);
	g_signal_connect (radio3, "toggled", G_CALLBACK (rates_cb), group);
	g_signal_connect (radio4, "toggled", G_CALLBACK (rates_cb), group);
	g_signal_connect (radio5, "toggled", G_CALLBACK (rates_cb), group);
	g_signal_connect (radio6, "toggled", G_CALLBACK (rates_cb), group);
	g_signal_connect (radio7, "toggled", G_CALLBACK (rates_cb), group);

	gtk_widget_show_all (popover);
}

static gchar *
get_sample_rate ()
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

	g_print ("From `sample_rate.c` line 138: %s\n", argv[argc]);

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

void
sample_rate (GtkWidget *grid)
{
	gchar *string;

	string = get_sample_rate ();
	srate_button = gtk_button_new_with_label (string);

	gtk_widget_set_tooltip_text (GTK_WIDGET (srate_button), "Sample Rate (hz)");
	gtk_widget_set_halign (srate_button, GTK_ALIGN_CENTER);
	gtk_widget_set_size_request (srate_button, 96, 34);	

	gtk_grid_attach (GTK_GRID (grid), srate_button, 1, 6, 1, 1);
	
	g_signal_connect (srate_button, "clicked", 
					  G_CALLBACK (sample_rate_popover_cb), NULL);
}
