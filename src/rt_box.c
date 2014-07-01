#include "rt_box.h"

static void
rt_arg_create (GtkToggleButton *button, const gchar *arg)
{	
	gchar **argv;
	gint argc;
	gint i;
	gint j;
	gboolean check_toggle;

	argc = 0;
	check_toggle = gtk_toggle_button_get_active (button);
	argv = get_arg_vector ();

	/* Get arg vector count. */
	while (argv[argc])
	{
		argc++;
	}

	/* Here we look for the jack arg `-R` with the for loop then the if 
	statement. If it does not exist then we add it to arg vector with 
	`else/if` statement at the end. */
	for (i = 0; i < argc; i++)
	{
		if (g_strcmp0 (argv[i], arg) == 0)
		{
			g_print ("From `rt_box.c` line 57: %d\n", i);

			break;
		}
		/* If the realtime arg is found but doesn't correlate to the 
		`checkbox` (i.e.: unchecked = '-r' checked = '-R') then add the arg to 
		the vector according to the `checkbox`. */
		else if ((g_strcmp0 (argv[i], "-r") == 0) && (check_toggle == TRUE))
		{
			argv[i] = g_strdup (arg);

			break;
		} 
		else if ((g_strcmp0 (argv[i], "-R") == 0) && (check_toggle == FALSE))
		{
			argv[i] = g_strdup (arg);

			break;
		} 
		/* If `arg` isn't found before the end of `argc` is reached 
		execute else/if statement. */
		else if (i == argc - 1)
		{
			/* Add space to arg vector for the jackd arg `-R`. */
			argc = argc + 1;
		
			/* Here we move the args over one to place `-R` as the 
			second arg in the vector. */
			for (j = argc; j > 1; j--)
			{
				argv[j] = argv[j - 1]; 
			} 
		
			argv[1] = g_strdup (arg);

			break;
		}
	}

	config_file_input (argv, argc);	
}

static void
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
	/* This gets called from `gjackctl_setings_cb` that's in the 
	`gjackctl_settings.c` module. */
	
	GtkWidget *checkbox;
	gchar **argv;
	gint argc;
	gint i;

	checkbox = gtk_check_button_new_with_label ("Realtime");
	argv = get_arg_vector ();	
	i = 0;

	/* Get arg vector count. */
	while (argv[argc])
	{
		argc++;
	}

	/* Check to see whether `.jackdrc` has `--realtime` enabled/disabled 
	check/uncheck `checkbox` accordingly (i.e. checked = realtime/unchecked = 
	no realtime). */
	while (argv[i])
	{
		if ((g_strcmp0 (argv[i], "-R") == 0) ||
			((g_strcmp0 (argv[i], "-R") != 0) &&
			 (g_strcmp0 (argv[i], "-r") != 0) &&
			 (i == argc -1)))
		{
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbox), TRUE);

			break;
		}
		else if (g_strcmp0 (argv[i], "-r") == 0)
		{
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbox), FALSE);
			break;
		}
		else 
		{
			i++;
		}		
	}

	g_strfreev (argv);

	/* Initiate tooltip for `checkbox` in the if/else statement. It won't 
	show when app first starts if we don't. */
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbox)) == TRUE)
	{
		gtk_widget_set_tooltip_text (checkbox, "Disable realtime audio");	
	}
	else
	{	
		gtk_widget_set_tooltip_text (checkbox, "Enable realtime audio");
	}

	/* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
	gtk_grid_attach (GTK_GRID (grid), checkbox, 0, 1, 1, 1);

	gtk_widget_set_margin_start (checkbox, 5);
	gtk_widget_set_margin_top (checkbox, 10);
	
	g_signal_connect (checkbox, "toggled", G_CALLBACK (real_time_cb), NULL);	
}
