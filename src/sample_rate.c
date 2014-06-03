#include "sample_rate.h"

GtkWidget *srate_button;
GtkWidget *popover;

void
rates_cb (GtkToggleButton *tb, gpointer user_data)
{
	GSList *list;
	gchar arg_rate[10];
	
	list = user_data;

	while (list)
	{
		tb = list -> data;

		if (gtk_toggle_button_get_active (tb) == TRUE)
		{
			gchar *arg_rate_const;
			gchar tooltip[30];			
			
			/* `-r` is the argument for setting the sample rate after driver is set in 
			jackd. */
			arg_rate_const = g_strdup ("-r");			
			
			g_sprintf (arg_rate, "-r%s", gtk_button_get_label (GTK_BUTTON (tb)));
			g_sprintf (tooltip, "Sample Rate = %s", gtk_button_get_label (GTK_BUTTON(tb)));

			gtk_button_set_label (GTK_BUTTON (srate_button), gtk_button_get_label (GTK_BUTTON (tb)));
			gtk_widget_set_tooltip_text (GTK_WIDGET (srate_button) , tooltip);

			//jack_start[5] = g_strconcat (arg_rate_const, gtk_button_get_label (GTK_BUTTON (tb)), NULL);

			g_print ("Button number %s selected\n", gtk_button_get_label (GTK_BUTTON (tb)));
			break;
		}

		list = list -> next;
		gtk_widget_hide (popover);
	}
}

void
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

void
sample_rate (GtkWidget *grid)
{
	srate_button = gtk_button_new_with_mnemonic ("_Rate");

	gtk_widget_set_tooltip_text (GTK_WIDGET (srate_button) , "Choose Sample Rate");
	gtk_widget_set_halign (srate_button, GTK_ALIGN_CENTER);
	gtk_widget_set_size_request (srate_button, 96, 34);	

	gtk_grid_attach (GTK_GRID (grid), srate_button, 1, 5, 1, 1);
	
	g_signal_connect (srate_button, "clicked", G_CALLBACK (sample_rate_popover_cb), NULL);
}
