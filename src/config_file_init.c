#include "config_file_init.h"

gint 
config_file_init ()
{
	config_t config;
	config_setting_t *root;
	config_setting_t *root_setting;
	config_setting_t *group_setting;
	config_setting_t *setting;
	gchar *file;

    file = g_strconcat (g_getenv ("HOME"),
                        "/.config/gjackctl/gjackctl.conf",
                        NULL);

	g_mkdir (g_strconcat (g_getenv ("HOME"),
                          "/.config/gjackctl",
                          NULL),
             S_IRWXU);

	if (g_file_test (file, G_FILE_TEST_EXISTS) == FALSE)
	{
		FILE *fp;

		fp = g_fopen (file, "w+");

		fclose (fp);
	
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
			setting = config_setting_add (group_setting, "priority", CONFIG_TYPE_STRING);
            config_setting_set_string (setting, "75");
		}

		setting = config_setting_get_member (group_setting, "port_max");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "port_max", CONFIG_TYPE_STRING);
            config_setting_set_string (setting, "256");
		}

        setting = config_setting_get_member (group_setting, "timeout");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "timeout", CONFIG_TYPE_STRING);
            config_setting_set_string (setting, "500");
		}

        setting = config_setting_get_member (group_setting, "clocksource");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "clocksource", CONFIG_TYPE_STRING);
            config_setting_set_string (setting, "System");
		}
	
		setting = config_setting_get_member (group_setting, "realtime");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "realtime", CONFIG_TYPE_BOOL);
		}
    
        setting = config_setting_get_member (group_setting, "unlock_libs");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "unlock_libs", CONFIG_TYPE_BOOL);
		}
	
        setting = config_setting_get_member (group_setting, "no_zombies");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "no_zombies", CONFIG_TYPE_BOOL);
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

        group_setting = config_setting_get_member (root_setting, "driver");

		if (group_setting == NULL)
		{
			group_setting = config_setting_add (root_setting, "driver", CONFIG_TYPE_GROUP);
		}

        setting = config_setting_get_member (group_setting, "type");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "type", CONFIG_TYPE_STRING);
            config_setting_set_string (setting, "Dummy");
		}

        setting = config_setting_get_member (group_setting, "device");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "device", CONFIG_TYPE_STRING);
		}
        
        setting = config_setting_get_member (group_setting, "sample_rate");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "sample_rate", CONFIG_TYPE_STRING);
		}

        setting = config_setting_get_member (group_setting, "period");

		if (setting == NULL)
		{
			setting = config_setting_add (group_setting, "period", CONFIG_TYPE_STRING);
		}

		config_set_tab_width (&config, 4);

		g_print ("Successful write to file: %d\n", config_write_file (&config, file));

		config_destroy (&config);
	}
	return 0;
}
