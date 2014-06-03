#include "rt_priority.h"

gint
digit (gchar **argvp)
{
	gchar *string;
	gint number;
	gint i;
	gint argcp;

	/* obtain arg count not including the `NULL` terminator. */
	while (argvp[argcp])
	{
		argcp++;
	}

	for (i = 0; i <= argcp; i++)
	{
		if (strncmp (argvp[i], "-P", 2) == 0)
		{
			string = argvp[i];
			number = g_ascii_strtoll (&string[2], NULL, 0);
			
			break;
		}
	}

	return number;	
}

void
priority_arg_create (gchar *arg)
{
	gchar **argvp;
	gint argcp;
	gint i;
	gint j;

	argvp = get_arg_vector ();
	argcp = 0;
	i = 0;

	/* Get arg count */
	while (argvp[argcp])
	{
		argcp++;
	}

	/* Here we look for the jack arg `-Px` with the for loop then the if 
	statement. If it does not exist then we add it to arg vector with 
	`else/if` statement. */
	for (i = 0; i < argcp; i++)
	{
		/* If priority arg isn't found before end of `argvp[i]` then create
		it and add it to vector. */
		if ((i == argcp - 1) && (strncmp (argvp[i], "-P", 2) != 0)) 
		{
			/* Add space to arg vector for the jackd arg `-R`. */
			argcp = argcp + 1;
		
			/* If realtime arg exists then place priority arg right 
			after that.*/
			if ((strncmp (argvp[1], "-r", 2) == 0) ||
				(strncmp (argvp[1], "-R", 2) == 0))
			{
				/* Here we move the args over one to place `-Px` as the 
				third arg in the vector. */
				for (j = argcp; j > 2; j--)
				{
					argvp[j] = argvp[j - 1]; 
				}

				argvp[2] = g_strdup (arg);
			}
			else
			{
				/* Here we move the args over one to place `-Px` as the 
				second arg in the vector. */
				for (j = argcp; j > 1; j--)
				{
					argvp[j] = argvp[j - 1]; 
				}

				argvp[1] = g_strdup (arg);
			}

			break;
		}
		/* If priority arg is found and matches break out of the loop. */		
		else if (g_strcmp0 (argvp[i], arg) == 0)
		{
			break;
		} 
		/* If `priority arg` is found but the number does not match 
		execute else/if statement. */
		else if (strncmp (argvp[i], "-P", 2) == 0)
		{		
			argvp[i] = g_strdup (arg);

			break;
		}
	}

	file_input (argvp, argcp);
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
	GtkWidget *label;
	GtkWidget *spin_button;
	GtkWidget *space;
	GtkAdjustment *adjustment;
	gint number_value;
	gint i;
	gchar **argvp;

	label = gtk_label_new ("Priority");
	adjustment = gtk_adjustment_new (75, 0, 99, 1, 0, 0);
	spin_button = gtk_spin_button_new (adjustment, 1, 0);
	i = 0;
	argvp = get_arg_vector ();
	space = gtk_label_new ("");

	/* If arg in vector is priority arg (i.e. '-Px') then grab number string 
	for display in widget. */
	while (argvp[i])
	{
		if (strncmp (argvp[i], "-P", 2) == 0)
		{
			number_value = digit (argvp);
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

	/* Align widgets within `grid` to keep things uniform. */
	gtk_widget_set_halign (label, GTK_ALIGN_CENTER);
	gtk_widget_set_valign (label, GTK_ALIGN_END);
	gtk_widget_set_halign (spin_button, GTK_ALIGN_CENTER);

	/* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
	gtk_grid_attach (GTK_GRID (grid), space, 0, 0, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), label, 1, 0, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), 
							 spin_button, 
							 label, 
							 GTK_POS_BOTTOM, 1, 1);

	g_signal_connect (spin_button, "value-changed", G_CALLBACK (value_cb), NULL);
}
