#include "get_arg_vector.h"

gchar **
get_arg_vector ()
{
	gchar cmd[128];
	gchar **argvp;
	gchar *contents;
	gint argcp;
	gsize size;

	argcp = 0;

	/* Create path to file `.jackdrc` using `g_sprintf ()`. */
	g_sprintf (cmd, "%s/.jackdrc", g_getenv ("HOME"));

	/* Check if file path exists. */
	if (g_file_test (cmd, G_FILE_TEST_EXISTS) == FALSE)
	{
		g_print ("File doesn't exist.  Create file.");

		return NULL;
	}

	/* Get contents of `.jackdrc` and add it to to `gchar **argvp`. */
	g_file_get_contents (cmd, &contents, &size, NULL);
	g_shell_parse_argv (contents, &argcp, &argvp, NULL);

	g_free (contents);

	/* reallocate argvp to provide enough room for adding arguments to the 
	vector. */
	argvp = g_realloc (argvp, (argcp + 8) * sizeof *argvp);

	return argvp;
}
