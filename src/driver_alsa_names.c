#include <stdio.h>
#include <gtk/gtk.h>
#include <alsa/asoundlib.h>

#include "driver_alsa_names.h"
#include "drivers.h"

/* Fetch ALSA device names for menu selection */
int
driver_alsa_names (GMenu *submenu)
//main ()
{
	//GMenu *submenu;
	gint err;	
	gint card;
	gint count;
	gint card_number;
	
	/* Initialize 'count' to be used by 'names[*][32]' correctly */
	count = 0;	

	gchar names[count][50];	
	snd_ctl_t *handle;
	snd_ctl_card_info_t *info;

	card = -1;
		
	/* First loop to obtain count of available cards */
	for (;;)
	{			
		/* Scans a list of alsa devices. With 'card' 
			at '-1' it will search for the first device. */ 
		if ((err = snd_card_next (&card)) < 0)
		{				
			g_print ("%s.\n", snd_strerror (err));
		
			return -1;
		}
		
		/* If 'card' reaches '-1' after finding devices, there
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
				
		/* Scans a list of alsa devices. With 'card' 
			at '-1' it will search for the first device. */ 
		if ((err = snd_card_next (&card)) < 0)
		{				
			g_print ("%s.\n", snd_strerror (err));
		
			return -1;
		}
		
		/* If 'card' reaches '-1' after finding devices, there
			are no more devices. */
		if (card < 0)
		{
			g_print ("\nNo more sound cards.\n\n");
	
			break;
		}

		/* Setup 'device' with proper syntax needed by other functions. */
		sprintf (device, "hw:%d", card);

		/* Open device to probe for hardware info here */		
		if ((err = snd_ctl_open (&handle, device, 0)) < 0)
		{
			g_print ("%s.\n", snd_strerror (err));

			return -1;
		}

		if ((err = snd_ctl_card_info_malloc (&info)) < 0)
		{
			g_print ("%s.\n", snd_strerror (err));

			return -1;
		}

		//g_print ("%s\n", device);  /* debug */

		/* Get hardware info after opening device */
		if ((err = snd_ctl_card_info (handle, info)) != 0)
		{
			g_print ("%s.\n", snd_strerror (err));

			return -1;
		}

		/* Card hardware info extracted from 'info'. */
		sprintf (card_id, "%s", snd_ctl_card_info_get_id (info));
		sprintf (card_name, "%s", snd_ctl_card_info_get_name (info));
		card_number = snd_ctl_card_info_get_card (info);
	
		snd_ctl_close (handle);
		snd_ctl_card_info_clear (info);
		snd_ctl_card_info_free (info);

		sprintf (names[card], "%s", card_name);
		g_print ("%s\n", names[card]);
		//dev_names (count, card, card_id, card_name);	
		
		/* Pack 'submenu' with alsa names. */
		if (card == 0)
		{
			gchar item_name[50];
			GMenuItem *item1;

			sprintf (item_name, "1. %s", names[card]);
			item1 = g_menu_item_new (item_name, NULL);
			g_menu_insert_item (submenu, 0, item1);	
		}
	
		if (card == 1)
		{
			gchar item_name[50];
			GMenuItem *item2;

			sprintf (item_name, "2. %s", names[card]);
			item2 = g_menu_item_new (item_name, NULL);
			g_menu_insert_item (submenu, 1, item2);
		}

		if (card == 2)
		{
			gchar item_name[50];
			GMenuItem *item3;

			sprintf (item_name, "3. %s", names[card]);
			item3 = g_menu_item_new (item_name, NULL);
			g_menu_insert_item (submenu, 2, item3);
		}

		if (card == 3)
		{
			gchar item_name[50];
			GMenuItem *item4;

			sprintf (item_name, "4. %s", names[card]);
			item4 = g_menu_item_new (item_name, NULL);
			g_menu_insert_item (submenu, 3, item4);
		}

		if (card == 4)
		{
			gchar item_name[50];
			GMenuItem *item5;

			sprintf (item_name, "5. %s", names[card]);
			item5 = g_menu_item_new (item_name, NULL);
			g_menu_insert_item (submenu, 4, item5);
		}

		if (card == 5)
		{
			gchar item_name[50];
			GMenuItem *item6;

			sprintf (item_name, "6. %s", names[card]);
			item6 = g_menu_item_new (item_name, NULL);
			g_menu_insert_item (submenu, 5, item6);
		}

		if (card == 6)
		{
			gchar item_name[50];
			GMenuItem *item7;

			sprintf (item_name, "7. %s", names[card]);
			item7 = g_menu_item_new (item_name, NULL);
			g_menu_insert_item (submenu, 6, item7);
		}
	}	

	return 0;
}
