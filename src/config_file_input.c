#include "config_file_input.h"

void
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
}
