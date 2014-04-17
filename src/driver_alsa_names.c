#include <stdio.h>
#include <gtk/gtk.h>
#include <alsa/asoundlib.h>

#include "driver_alsa_names.h"
#include "drivers.h"

GMenu *submenu;

void
alsa_driver_activate (GSimpleAction *action,
							GVariant *parameter,
							gpointer data)
{
	g_print("%s\n", g_variant_get_string (parameter, NULL));
}

/* Fetch ALSA device names for menu selection */
void
driver_alsa_names ()
{
	
	gint err;	
	gint card;
	gint count;
	gint card_number;
	
	/* Initialize `count` to be used by 'names[*][32]' correctly */
	count = 0;	
	
	snd_ctl_t *handle;
	snd_ctl_card_info_t *info;

	card = -1;
	submenu = g_menu_new ();
	
		
	/* First loop to obtain count of available cards */
	for (;;)
	{			
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
			g_print ("\nNo more sound cards.\n\n");

			g_print ("%d\n\n", count);
	
			break;
		}
			
		++count;
	}

	for (;;)
	{	
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
			g_print ("\nNo more sound cards.\n\n");
	
			break;
		}

		/* Setup `device` with proper syntax needed by other functions. */
		sprintf (device, "hw:%d", card);

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

		//g_print ("%s\n", device);  /* debug */

		/* Get hardware info after opening device */
		if ((err = snd_ctl_card_info (handle, info)) != 0)
		{
			g_print ("%s.\n", snd_strerror (err));

			break;
		}

		/* Card hardware info extracted from `info`. */
		sprintf (card_id, "%s", snd_ctl_card_info_get_id (info));
		sprintf (card_name, "%s", snd_ctl_card_info_get_name (info));
		card_number = snd_ctl_card_info_get_card (info);
	
		/* Close `handle` and free space in memory used by `info`. */
		snd_ctl_close (handle);
		snd_ctl_card_info_clear (info);
		snd_ctl_card_info_free (info);

		/* Pack `submenu` with alsa names. */
		if (card == 0)
		{
			GMenuItem *item1;
			GVariant *id;

			id = g_variant_new_string (card_id);
			item1 = g_menu_item_new (card_name, NULL);
			g_menu_item_set_action_and_target_value (item1, "app.alsa_driver", id);
			g_menu_insert_item (submenu, 0, item1);	
		}
	
		if (card == 1)
		{
			GMenuItem *item2;
			GVariant *id;

			id = g_variant_new_string (card_id);
			item2 = g_menu_item_new (card_name, NULL);
			g_menu_item_set_action_and_target_value (item2, "app.alsa_driver", id);
			g_menu_insert_item (submenu, 1, item2);
		}

		if (card == 2)
		{
			GMenuItem *item3;
			GVariant *id;

			id = g_variant_new_string (card_id);
			item3 = g_menu_item_new (card_name, NULL);
			g_menu_item_set_action_and_target_value (item3, "app.alsa_driver", id);
			g_menu_insert_item (submenu, 2, item3);
		}

		if (card == 3)
		{
			GMenuItem *item4;
			GVariant *id;

			id = g_variant_new_string (card_id);
			item4 = g_menu_item_new (card_name, NULL);
			g_menu_item_set_action_and_target_value (item4, "app.alsa_driver", id);
			g_menu_insert_item (submenu, 3, item4);
		}

		if (card == 4)
		{
			GMenuItem *item5;
			GVariant *id;

			id = g_variant_new_string (card_id);
			item5 = g_menu_item_new (card_name, NULL);
			g_menu_item_set_action_and_target_value (item5, "app.alsa_driver", id);
			g_menu_insert_item (submenu, 4, item5);
		}

		if (card == 5)
		{
			GMenuItem *item6;
			GVariant *id;

			id = g_variant_new_string (card_id);
			item6 = g_menu_item_new (card_name, NULL);
			g_menu_item_set_action_and_target_value (item6, "app.alsa_driver", id);
			g_menu_insert_item (submenu, 5, item6);
		}

		if (card == 6)
		{
			GMenuItem *item7;
			GVariant *id;

			id = g_variant_new_string (card_id);
			item7 = g_menu_item_new (card_name, NULL);
			g_menu_item_set_action_and_target_value (item7, "app.alsa_driver", id);
			g_menu_insert_item (submenu, 6, item7);
		}
	}	
}
