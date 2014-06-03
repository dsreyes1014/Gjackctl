#include "rt_box.h"

void
rt_arg_create (GtkToggleButton *button, const gchar *arg)
{	
	gchar **argvp;
	gint argcp;
	gint i;
	gint j;
	gboolean check_toggle;

	argcp = 0;
	check_toggle = gtk_toggle_button_get_active (button);
	argvp = get_arg_vector ();

	/* Get arg count */
	while (argvp[argcp])
	{
		argcp++;
	}

	/* Here we look for the jack arg `-R` with the for loop then the if 
	statement. If it does not exist then we add it to arg vector with 
	`else/if` statement at the end. */
	for (i = 0; i < argcp; i++)
	{
		if (g_strcmp0 (argvp[i], arg) == 0)
		{
			g_print ("From `rt_box.c` line 57: %d\n", i);

			break;
		}
		/* If the realtime arg is found but doesn't correlate to the 
		`checkbox` (i.e.: unchecked = '-r' checked = '-R') then add the arg to 
		the vector according to the `checkbox`. */
		else if ((g_strcmp0 (argvp[i], "-r") == 0) && (check_toggle == TRUE))
		{
			argvp[i] = g_strdup (arg);

			break;
		} 
		else if ((g_strcmp0 (argvp[i], "-R") == 0) && (check_toggle == FALSE))
		{
			argvp[i] = g_strdup (arg);

			break;
		} 
		/* If `arg` isn't found before the end of `argcp` is reached 
		execute else/if statement. */
		else if (i == argcp - 1)
		{
			/* Add space to arg vector for the jackd arg `-R`. */
			argcp = argcp + 1;
		
			/* Here we move the args over one to place `-R` as the 
			second arg in the vector. */
			for (j = argcp; j > 1; j--)
			{
				argvp[j] = argvp[j - 1]; 
			} 
		
			argvp[1] = g_strdup (arg);

			break;
		}
	}

	file_input (argvp, argcp);	
}

void
real_time_cb (GtkToggleButton *button, gpointer data)
{
	gboolean check;
		
	check = gtk_toggle_button_get_active (button);

	if (check == TRUE)
	{
		gtk_widget_set_tooltip_text (GTK_WIDGET (button), 
									 "Disable Realtime Audio");

		rt_arg_create (button, "-R");
	}
	else
	{	
		gtk_widget_set_tooltip_text (GTK_WIDGET (button), 
									 "Enable Realtime Audio");

		rt_arg_create (button, "-r");
	}
}

void
rt_box (GtkWidget *grid)
{
	GtkWidget *checkbox;
	GtkWidget *label;
	GtkWidget *space;
	gboolean check;
	gchar **argvp;
	gint argcp;
	gint i;

	checkbox = gtk_check_button_new ();
	label = gtk_label_new ("Realtime");
	check = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbox));
	argvp = get_arg_vector ();	
	i = 0;
	space = gtk_label_new ("");

	/* Initiate tooltip for `checkbox` in the if/else statement. It won't 
	show when app first starts if we don't. */
	if (check == TRUE)
	{
		gtk_widget_set_tooltip_text (checkbox, "Disable Realtime Audio");	
	}
	else
	{	
		gtk_widget_set_tooltip_text (checkbox, "Enable Realtime Audio");
	}

	/* Get arg count */
	while (argvp[argcp])
	{
		argcp++;
	}

	/* Check to see whether `.jackdrc` has realtime enabled/disabled 
	check/uncheck `checkbox` accordingly (i.e. checked = realtime/unchecked = 
	no realtime). */
	while (argvp[i])
	{
		if ((g_strcmp0 (argvp[i], "-R") == 0) ||
			((g_strcmp0 (argvp[i], "-R") != 0) &&
			 (g_strcmp0 (argvp[i], "-r") != 0) &&
			 (i == argcp -1)))
		{
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbox), TRUE);

			break;
		}
		else if (g_strcmp0 (argvp[i], "-r") == 0)
		{
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbox), FALSE);
			break;
		}
		else 
		{
			i++;
		}		
	}

	/* Position `label` inside `grid`. */	
	gtk_widget_set_valign (label, GTK_ALIGN_END);

	/* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
	gtk_grid_attach (GTK_GRID (grid), space, 2, 0, 1, 1);
	gtk_grid_attach (GTK_GRID (grid), label, 3, 0, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), checkbox, 
							 label, GTK_POS_RIGHT, 1, 1);
	
	g_signal_connect (checkbox, "toggled", G_CALLBACK (real_time_cb), NULL);	
}
