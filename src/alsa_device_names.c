#include "alsa_device_names.h"

/*void
alsa_cb (GSimpleAction *action,
         GVariant *parameter,
		 gpointer user_data)
{	
    GtkPassedDriverData *rdata;
    const gchar *string;
    gchar *tt;

    rdata = user_data;
    string = g_variant_get_string (parameter, NULL);
    tt = g_strconcat ("Current device is ",
                      "\"",
                      string,
                      "\"",
                      "\n",
                      "Click to choose driver/device",
                      NULL);

    gtk_button_set_label (GTK_BUTTON (rdata -> pbutton), "ALSA");
    gtk_label_set_text (GTK_LABEL (rdata -> label), string); 
    gtk_widget_set_tooltip_text (rdata -> pbutton, tt);
}*/

void
alsa_device_names (GMenu *submenu)
{
	/* Fetch ALSA device names for menu selection */

    GMenu *alsa_section;
    
	gint err;	
	gint card;
	gint count;
	
	snd_ctl_t *handle;
	snd_ctl_card_info_t *info;

	card = -1;
	count = 0;	
    alsa_section = g_menu_new ();

		
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
		//gchar card_name[64]; 
		//gchar card_id[32];	
        gchar *card_name;
		gchar *card_id;

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
		//g_sprintf (card_id, "%s", snd_ctl_card_info_get_id (info));
		//g_sprintf (card_name, "%s", snd_ctl_card_info_get_name (info));
	    card_id = g_strdup (snd_ctl_card_info_get_id (info));
        card_name = g_strdup (snd_ctl_card_info_get_name (info));        
    
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
				g_menu_item_set_action_and_target_value (item, "settings.alsa", id);
				g_menu_insert_item (alsa_section, 0, item);
				break;		
			case 1:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "settings.alsa", id);
				g_menu_insert_item (alsa_section, 1, item);
                //*id_list = g_slist_prepend (*id_list, card_id);
                //*name_list = g_slist_prepend (*name_list, card_name);
				break;
			case 2:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "settings.alsa", id);
				g_menu_insert_item (alsa_section, 2, item);
                //*id_list = g_slist_prepend (*id_list, card_id);
                //*name_list = g_slist_prepend (*name_list, card_name);
				break;		
			case 3:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "settings.alsa", id);
				g_menu_insert_item (alsa_section, 3, item);
                //*id_list = g_slist_prepend (*id_list, card_id);
                //*name_list = g_slist_prepend (*name_list, card_name);
				break;
			case 4:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "settings.alsa", id);
				g_menu_insert_item (alsa_section, 4, item);
                //*id_list = g_slist_prepend (*id_list, card_id);
                //*name_list = g_slist_prepend (*name_list, card_name);
				break;
			case 5:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "settings.alsa", id);
				g_menu_insert_item (alsa_section, 5, item);
                //*id_list = g_slist_prepend (*id_list, card_id);
                //*name_list = g_slist_prepend (*name_list, card_name);
				break;
			case 6:
				id = g_variant_new_string (card_id);
				item = g_menu_item_new (card_name, NULL);
				g_menu_item_set_action_and_target_value (item, "settings.alsa", id);
				g_menu_insert_item (alsa_section, 6, item);
                //*id_list = g_slist_prepend (*id_list, card_id);
                //*name_list = g_slist_prepend (*name_list, card_name);
				break;
			default:
				g_print ("There are only %d alsa devices.\n", card);
		}
	}	
    g_menu_insert_section (submenu,
                           0,
                           "ALSA",
                           G_MENU_MODEL (alsa_section));
}
