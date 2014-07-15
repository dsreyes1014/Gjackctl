#include "config_file_init.h"

gint 
config_file_init ()
{
	config_t config;
	config_setting_t *root;
	config_setting_t *root_setting;
	config_setting_t *group_setting;
	config_setting_t *setting;
	gchar config_file[128];
	gchar config_dir[128];

	g_sprintf (config_dir, "%s/.config/gjackctl", g_getenv ("HOME"));
	g_sprintf (config_file, "%s/.config/gjackctl/gjackctl.conf", g_getenv ("HOME"));

	g_mkdir (config_dir, S_IRWXU);

	if (g_file_test (config_file, G_FILE_TEST_EXISTS) == FALSE)
	{
		FILE *file;

		file = g_fopen (config_file, "w+");

		fclose (file);
	
		/* Initialize `config`. */
		config_init (&config);

		/* Check to see if `config` has root setting. */
		root = config_root_setting (&config);
		root_setting = config_setting_get_member (root, "gjackctl");

		if (root_setting == NULL)
		{
			root_setting = config_setting_add (root, "gjackctl", CONFIG_TYPE_GROUP);
		}

		group_setting = config_setting_get_member (root_setting, "server");

		if (group_setting == NULL)
		{
			group_setting = config_setting_add (root_setting, "server", CONFIG_TYPE_GROUP);
		}

		setting = config_setting_get_member (group_setting, "name");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "name", CONFIG_TYPE_STRING);
		}

		setting = config_setting_get_member (group_setting, "priority");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "priority", CONFIG_TYPE_INT);
            config_setting_set_int (setting, 75);
		}

		setting = config_setting_get_member (group_setting, "port_max");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "port_max", CONFIG_TYPE_INT);
		}
	
		setting = config_setting_get_member (group_setting, "realtime");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "realtime", CONFIG_TYPE_BOOL);
		}
	
		setting = config_setting_get_member (group_setting, "no_memlock");
	
		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "no_memlock", CONFIG_TYPE_BOOL);
		}
	
		setting = config_setting_get_member (group_setting, "midi");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "midi", CONFIG_TYPE_BOOL);
		}

		config_set_tab_width (&config, 4);

		g_print ("Successful write to file: %d\n", config_write_file (&config, config_file));

		config_destroy (&config);
	}
	return 0;
}
