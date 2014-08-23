#include "get_jack_gpid.h"

GPid 
get_jack_gpid ()
{
    FILE *cmd;
    gchar *s;
    GPid pid;
    
    cmd = popen ("pgrep jackd", "r");

    /* Here we get the `GPid pid` from the `cmd` statement and convert 
	it to a gint using the `g_ascii_strtoll ()` function. */
	while (fgets (s, sizeof (s), cmd) != NULL)
	{
		pid = atoi (s);
	}
	
	pclose (cmd);
    
    return pid;
}
