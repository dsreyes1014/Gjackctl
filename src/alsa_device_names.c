#include "alsa_device_names.h"

void
print_alsa_driver_activate_cb (GSimpleAction *action,
							   GVariant *parameter,
							   gpointer user_data)
{	
	/* Callback function for a `GActionEntry *entry` declared in `main.c`. 
	This is for alsa devices in the `jackd -d` arg setup. */

	GtkWidget *label_driver;
	gchar **argv;
	gchar *device_arg;
	gchar *tooltip;
	gint argc;
	gint i;
	gint j;
	const gchar *device;
	
	argv = get_arg_vector ();
	argc = 0;
	i = 0;
	j = 0;
	device = g_variant_get_string (parameter, NULL);
	device_arg = g_strconcat ("-dhw:", device, NULL);	
	label_driver = user_data;

	/* Get arg vector count. */
	while (argv[argc])
	{
		argc++;
	}

	g_print ("From `alsa_device_names.c` line 31: %s\n", argv[argc - 1]);

	for (i = 0; i < argc; i++)
	{
		
		/* If device arg already exists from `.jackdrc` break loop. */
		if ((g_strcmp0 (argv[i], "-dalsa") == 0) &&
			(g_strcmp0 (argv[i + 1], device_arg) == 0))
		{
			break;
		}
		/* If the alsa device arg matches but the actual device does not
		change it to user-picked alsa device (i.e. -dalsa -dhw:xxx) then
		break loop. */
		else if ((g_strcmp0 (argv[i], "-dalsa") == 0) &&
				 (g_strcmp0 (argv[i + 1], device_arg) != 0))
		{
			argv[i + 1] = g_strdup (device_arg);
			break;
		}
		/* Here we check to see if the alsa device arg exists at the end of 
		argvp.  If it doesn't then we create it according to the conditions 
		set. */
		else if ((i == argc -1) && (strncmp (argv[i], "-dalsa", 6) != 0))
		{
			argc = argc + 2;

			/* If priority arg exists at `vector [2]` then create alsa device
			args right after that. */
			if (strncmp (argv[2], "-P", 2) == 0)
			{
				/* Move args over 2 spaces to input device args into vector. 
				We move it 2 elements because the alsa device arg requires 
				the `-dalsa` option and then its `-dhw:xxx` device backend
				option. */
				for (j = argc; j > 2; j--)
				{
					argv[j] = argv[j - 2];
				} 

				argv[3] = g_strdup ("-dalsa");
				argv[4] = g_strdup (device_arg);
				break;
			}
			else if ((strncmp (argv[1], "-P", 2) == 0) || 
					 ((g_strcmp0 (argv[1], "-R") == 0) &&
					  (strncmp (argv[2], "-P", 2) != 0)) ||
					 ((g_strcmp0 (argv[1], "-r") == 0) &&
					  (strncmp (argv[2], "-P", 2) != 0)))
			{
				/* Again we move args over 2 elements for reasons stated 
				above. */
				for (j = argc; j > 1; j--)
				{
					argv[j] = argv[j - 2];
				} 

				argv[2] = g_strdup ("-dalsa");
				argv[3] = g_strdup (device_arg);
				break;
			}	
			else if ((strncmp (argv[1], "-P", 2) != 0) ||
					 (g_strcmp0 (argv[1], "-R") != 0) ||
					 (g_strcmp0 (argv[1], "-r") != 0))
			{
				/* Again we move args over 2 elements for reasons stated 
				above. */
				for (j = argc; j > 0; j--)
				{
					argv[j] = argv[j - 2];
				} 

				argv[1] = g_strdup ("-dalsa");
				argv[2] = g_strdup (device_arg);
				break;
			}	
		}
	}

	config_file_input (argv, argc);

	tooltip = g_strconcat ("Soundcard: '", device, "'", NULL);
	gtk_widget_set_tooltip_text (label_driver, tooltip);

	g_print("%s\n", g_variant_get_string (parameter, NULL));
}

void
alsa_device_names (GMenu *submenu)
{
	/* Fetch ALSA device names for menu selection */	

	gint err;	
	gint card;
	gint count;
	
	snd_ctl_t *handle;
	snd_ctl_card_info_t *info;

	card = -1;
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
		gchar card_name[64]; 
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
