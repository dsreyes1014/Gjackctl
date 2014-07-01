#include "no_memlock.h"

static void
no_memlock_arg_create (GtkToggleButton *tb, const gchar *arg)
{
	gchar **argv;
	gint argc;
	gint i;
	gint j;
	gint k;

	argc = 0;
	argv = get_arg_vector ();

	/* Get arg vector count. */
	while (argv[argc])
	{
		argc++;
	}

	for (i = 0; i < argc; i++)
	{
		if (g_strcmp0 (argv[i], arg) == 0)
		{
			break;	
		}
		else if ((i == argc -1) && (g_strcmp0 (argv[i], arg) != 0))
        {
			argc = argc + 1;

			/* Find `--driver` arg to place `-m` arg right before that. */
			for (k = 0; (strncmp (argv[k], "-d", 2) != 0); ++k)
			{
				if (k == argc - 2)
				{
					break;
				}
			}

			for (j = argc; j > k; j--)
			{
				argv[j] = argv[j - 1];
			}

			argv[j] = g_strdup (arg);
			
			break;
		}
	}

	config_file_input (argv, argc);
}

static void
no_memlock_cb (GtkToggleButton *tb, gpointer user_data)
{
	gboolean check;

	check = gtk_toggle_button_get_active (tb);

	if (check == TRUE)
	{
		no_memlock_arg_create (tb, "-m");
	}
}

void
no_memlock (GtkWidget *grid)
{
	GtkWidget *checkbutton;

	checkbutton = gtk_check_button_new_with_label ("No Memory Lock");

	gtk_grid_attach (GTK_GRID (grid), checkbutton, 0, 2, 1, 1);

	gtk_widget_set_margin_start (checkbutton, 5);

	gtk_widget_set_tooltip_text (checkbutton, "Do not attempt to lock memory, even if in `--realtime`.");

	g_signal_connect (checkbutton, "toggled", G_CALLBACK (no_memlock_cb), NULL);
}
