#include "config_file_input.h"

/*void
config_file_input (gchar **argv, gint argc)
{
	gchar file[128];	
	gint i;
	FILE *jackdrc;
	
	g_sprintf (file, "%s/.jackdrc", g_getenv ("HOME"));

	jackdrc = g_fopen (file, "w+");

	for (i = 0; i < argc; i++)
	{
		g_fprintf (jackdrc, "%s ", argv[i]);
	}
		
	fclose (jackdrc);

	g_strfreev (argv);
}*/

gint 
config_file_input (const gchar *config_path, 
                   gint type, 
                   gpointer user_data)
{
	gchar config_file[128];
	config_t config;
	config_setting_t *setting;

	g_sprintf (config_file, "%s/.config/gjackctl/gjackctl.conf", g_getenv ("HOME"));	

	config_init (&config);
	config_read_file (&config, config_file);

	setting = config_lookup (&config, config_path);

	if (type == CONFIG_TYPE_INT)
	{
		gint *value = user_data;

		config_setting_set_int (setting, *value);
	}
	else if (type == CONFIG_TYPE_INT64)
	{
		gint64 *value = user_data;

		config_setting_set_int64 (setting, *value);
	}
	else if (type == CONFIG_TYPE_FLOAT)
	{
		gfloat *value = user_data;

		config_setting_set_float (setting, *value);
	}
	else if (type == CONFIG_TYPE_STRING)
	{
		const gchar *value = user_data;

		config_setting_set_string (setting, value);
	}
	else if (type == CONFIG_TYPE_BOOL)
	{
		gboolean *value = user_data;

		config_setting_set_bool (setting, *value);
	}

	config_set_tab_width (&config, 4);
	config_write_file (&config, config_file);
	config_destroy (&config);


	return 0;
}
