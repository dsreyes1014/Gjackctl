#include "rt_box.h"

void
file_input (gchar **string, gint argc)
{
	gchar file[128];	
	gint i;
	FILE *jackdrc;
	
	g_sprintf (file, "%s/.jackdrc", g_getenv ("HOME"));

	jackdrc = g_fopen (file, "w+");

	for (i = 0; i < argc; i++)
	{
		g_fprintf (jackdrc, "%s ", string[i]);
	}
		
	fclose (jackdrc);
}

void
rt_arg_create (GtkToggleButton *button, gchar *arg)
{
	gchar cmd[128];
	gchar *contents;
	gchar **argvp;
	gint argcp;
	gint i;
	gint j;
	gsize size;
	gboolean check_toggle;

	argcp = 0;
	contents = NULL;
	check_toggle = gtk_toggle_button_get_active (button);

	/* Create path to file `.jackdrc` using `g_sprintf ()`. */
	g_sprintf (cmd, "%s/.jackdrc", g_getenv ("HOME"));

	/* Check if file path exists. */
	if (g_file_test (cmd, G_FILE_TEST_EXISTS) == FALSE)
	{
		g_print ("File doesn't exist");
	}

	g_file_get_contents (cmd, &contents, &size, NULL);
	g_shell_parse_argv (contents, &argcp, &argvp, NULL);

	/* Here we look for the jack arg `-R` with the for loop then the if 
	statement. If it does not exist then we add it to arg vector with 
	`else/if` statement. */
	for (i = 0;; i++)
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
			argvp[i] = arg;

			break;
		} 
		else if ((g_strcmp0 (argvp[i], "-R") == 0) && (check_toggle == FALSE))
		{
			argvp[i] = arg;

			break;
		} 
		/* If `arg` isn't found before the end of `argcp` is reached 
		execute else/if statement. */
		else if (i == argcp + 1)
		{
			/* Add space to arg vector for the jackd arg `-R`. */
			argcp = argcp + 1;
		
			/* Here we move the args over one to place `-R` as the 
			second arg in the vector. */
			for (j = argcp; j >= 1; j--)
			{
				argvp[j] = argvp[j - 1]; 
			} 
		
			argvp[1] = arg;

			break;
		}
	}

	file_input (argvp, argcp);

	g_print ("From `rt_arg_create ()` line 77: %s %s\n", argvp[1], argvp[2]);
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
	gboolean check;

	checkbox = gtk_check_button_new ();
	label = gtk_label_new ("Realtime");
	check = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (checkbox));	

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

	/* Set `GtkWidget checkbox` to active to enable the `--realtime` 
	argument for `jackd` as default. */
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (checkbox), TRUE);

	/* Pack `GtkGrid grid` which is declared in `gjackctl_settings.c`
	in the `gjackctl_settings_cb` function. */
	gtk_grid_attach (GTK_GRID (grid), checkbox, 0, 0, 1, 1);
	gtk_grid_attach_next_to (GTK_GRID (grid), label, 
							 checkbox, GTK_POS_RIGHT, 1, 1);
	
	g_signal_connect (checkbox, "toggled", G_CALLBACK (real_time_cb), NULL);	
}
