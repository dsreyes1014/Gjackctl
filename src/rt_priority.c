#include "rt_priority.h"

gint
digit (gchar **argv)
{
	gchar *string;
	gint number;
	gint i;
	gint argc;

	/* obtain arg count not including the `NULL` terminator. */
	while (argv[argc])
	{
		argc++;
	}

	for (i = 0; i <= argc; i++)
	{
		if (strncmp (argv[i], "-P", 2) == 0)
		{
			string = argv[i];
			number = g_ascii_strtoll (&string[2], NULL, 0);
			
			break;
		}
	}

	g_strfreev (argv);

	return number;	
}

void
priority_arg_create (gchar *arg)
{
	gchar **argv;
	gint argc;
	gint i;
	gint j;

	argv = get_arg_vector ();
	argc = 0;
	i = 0;

	/* Get arg count */
	while (argv[argc])
	{
		argc++;
	}

	/* Here we look for the jack arg `-Px` with the for loop then the if 
	statement. If it does not exist then we add it to arg vector with 
	`else/if` statement. */
	for (i = 0; i < argc; i++)
	{
		/* If priority arg isn't found before end of `argv[i]` then create
		it and add it to vector. */
		if ((i == argc - 1) && (strncmp (argv[i], "-P", 2) != 0)) 
		{
			/* Add space to arg vector for the jackd arg `-R`. */
			argc = argc + 1;
		
			/* If realtime arg exists then place priority arg right 
			after that.*/
			if ((strncmp (argv[1], "-r", 2) == 0) ||
				(strncmp (argv[1], "-R", 2) == 0))
			{
				/* Here we move the args over one to place `-Px` as the 
				third arg in the vector. */
				for (j = argc; j > 2; j--)
				{
					argv[j] = argv[j - 1]; 
				}

				argv[2] = g_strdup (arg);
			}
			else
			{
				/* Here we move the args over one to place `-Px` as the 
				second arg in the vector. */
				for (j = argc; j > 1; j--)
				{
					argv[j] = argv[j - 1]; 
				}

				argv[1] = g_strdup (arg);
			}

			break;
		}
		/* If priority arg is found and matches break out of the loop. */		
		else if (g_strcmp0 (argv[i], arg) == 0)
		{
			break;
		} 
		/* If `priority arg` is found but the number does not match 
		execute else/if statement. */
		else if (strncmp (argv[i], "-P", 2) == 0)
		{		
			argv[i] = g_strdup (arg);

			break;
		}
	}

	config_file_input (argv, argc);
}

void
value_cb (GtkSpinButton *spin_button, gpointer data)
{
	gint priority_num;
	const gchar *priority_arg;
	gchar number[10];
	gchar *full_arg;

	priority_num = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (spin_button));
	priority_arg = "-P";

	g_sprintf (number, "%d", priority_num);
	full_arg = g_strconcat (priority_arg, number, NULL);

	priority_arg_create (full_arg);
}

void
rt_priority (GtkWidget *grid)
{
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */

	GtkWidget *label;
	GtkWidget *spin_button;
	GtkAdjustment *adjustment;
	gint number_value;
	gint i;
	gchar **argv;

	label = gtk_label_new ("Priority");
	adjustment = gtk_adjustment_new (75, 0, 99, 1, 0, 0);
	spin_button = gtk_spin_button_new (adjustment, 1, 0);
	i = 0;
	argv = get_arg_vector ();

	/* If arg in vector is priority arg (i.e. '-Px') then grab number string 
	for display in widget. */
	while (argv[i])
	{
		if (strncmp (argv[i], "-P", 2) == 0)
		{
			number_value = digit (argv);
			gtk_spin_button_set_value (GTK_SPIN_BUTTON (spin_button),
									   number_value);			
			break;
		}
		else
		{
			/* Increase `i` until the vector lands on `NULL` or finds a 
			a match in the `if ()` statement. */
			i++;
		}
	}

	/* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
	gtk_grid_attach (GTK_GRID (grid), label, 2, 0, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), 
							 spin_button, 
							 label, 
							 GTK_POS_RIGHT, 1, 1);
	
	gtk_widget_set_margin_start (label, 15);
	gtk_widget_set_margin_top (label, 5);
	gtk_widget_set_margin_top (spin_button, 10);
	gtk_widget_set_margin_end (spin_button, 10);

	g_signal_connect (spin_button, "value-changed", G_CALLBACK (value_cb), NULL);
}
