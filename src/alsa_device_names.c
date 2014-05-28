#include "alsa_device_names.h"

void
print_alsa_driver_activate (GSimpleAction *action,
							GVariant *parameter,
							gpointer data)
{
	/* Callback function for a `GActionEntry *entry` declared in `main.c`. */
	gchar device_hint[20];	
	gchar *driver_args1;
	gchar *driver_args2;

	/* Arguments for JACK server. */
	//jack_start[3] = "-dalsa";	
	//driver_args1 = g_strdup ("-dalsa ");
	driver_args2 = g_strdup ("-dhw:");
	
	//jack_start[4] = g_strconcat (driver_args2, g_variant_get_string (parameter, NULL), NULL);

	//g_print ("Debug from `print_alsa_driver_activate`: %s\n", jack_start[2]);

	/* For tooltip */
	g_sprintf (device_hint, "hw:%s", g_variant_get_string (parameter, NULL));

	/* GtkLabel *label2 is declared in `drivers.h`. */
	gtk_label_set_text (GTK_LABEL (label_driver), "ALSA");
	gtk_widget_set_tooltip_text (label_driver, device_hint);
	g_print("%s\n", g_variant_get_string (parameter, NULL));
}

void
alsa_device_names ()
{
	/* Fetch ALSA device names for menu selection */	

	gint err;	
	gint card;
	gint count;
	gint card_number;
	
	snd_ctl_t *handle;
	snd_ctl_card_info_t *info;

	card = -1;
	submenu = g_menu_new ();
	count = 0;	
		
	/* First loop to obtain count of available cards */
	for (;;)
	{			
		/* Scans a list of alsa devices. With `card` 
		set at `-1` it will search for the first device. */ 
		if ((err = snd_card_next (&card)) < 0)
		{				
			g_print ("%s.\n", snd_strerror (err));
		
			break;
		}
		
		/* If `card` reaches `-1` after finding devices, there
		are no more devices. */
		if (card < 0)
		{	
			break;
		}
			
		++count;
	}

	for (;;)
	{	
		GMenuItem *item;
		GVariant *id;

		gchar device[20];
		gchar card_name[32]; 
		gchar card_id[32];	
				
		/* Scans a list of alsa devices. With `card` 
		at `-1` it will search for the first device. */ 
		if ((err = snd_card_next (&card)) < 0)
		{				
			g_print ("%s.\n", snd_strerror (err));
		
			break;
		}
		
		/* If `card` reaches `-1` after finding devices, there
		are no more devices. */
		if (card < 0)
		{	
			break;
		}

		/* Setup `device` with proper syntax needed by other functions. */
		g_sprintf (device, "hw:%d", card);

		/* Open device to probe for hardware info here */		
		if ((err = snd_ctl_open (&handle, device, 0)) < 0)
		{
			g_print ("%s.\n", snd_strerror (err));

			break;
		}

		/* This initializes `info` allocates space in memory for it. */
		if ((err = snd_ctl_card_info_malloc (&info)) < 0)
		{
			g_print ("%s.\n", snd_strerror (err));

			break;
		}

		/* Get hardware info after opening device */
		if ((err = snd_ctl_card_info (handle, info)) != 0)
		{
			g_print ("%s.\n", snd_strerror (err));

			break;
		}

		/* Card hardware info extracted from `info`. */
		g_sprintf (card_id, "%s", snd_ctl_card_info_get_id (info));
		g_sprintf (card_name, "%s", snd_ctl_card_info_get_name (info));
		card_number = snd_ctl_card_info_get_card (info);
	
		/* Close `handle` and free space in memory used by `info`. */
		snd_ctl_close (handle);
		snd_ctl_card_info_clear (info);
		snd_ctl_card_info_free (info);

		/* Pack `GMenu submenu` with alsa names. */
		switch (card)
		{
			case 0:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "app.print_alsa_driver", id);
				g_menu_insert_item (submenu, 0, item);	
				break;		
			case 1:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "app.print_alsa_driver", id);
				g_menu_insert_item (submenu, 1, item);
				break;
			case 2:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "app.print_alsa_driver", id);
				g_menu_insert_item (submenu, 2, item);
				break;		
			case 3:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "app.print_alsa_driver", id);
				g_menu_insert_item (submenu, 3, item);
				break;
			case 4:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "app.print_alsa_driver", id);
				g_menu_insert_item (submenu, 4, item);
				break;
			case 5:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "app.print_alsa_driver", id);
				g_menu_insert_item (submenu, 5, item);
				break;
			case 6:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "app.print_alsa_driver", id);
				g_menu_insert_item (submenu, 6, item);
				break;
			default:
				g_print ("There are only %d alsa devices.\n", card);
		}
	}	
}
