#include "get_jack_gpid.h"

GPid 
get_jack_gpid (const gchar *string_arg)
{
    FILE *cmd;
    gchar string[32];   
    GPid pid;
    
    cmd = popen ("pgrep jackd", "r");
    pid = -2;

    
    /* Here we get the `GPid pid` from the `cmd` statement and convert 
	it to a gint using the `g_ascii_strtoll ()` function. */
    if (string_arg == NULL)
    {
	    while (fgets (string, sizeof (string), cmd) != NULL)
	    {
		    pid = g_ascii_strtoll (string, NULL, 10);
	    }
    }
    else
    {
        pid = g_ascii_strtoll (string_arg, NULL, 10);
    }
	
	pclose (cmd);

    //g_print ("From `get_jack_gpid.c`: %d\n", pid);

    if (kill (pid, 0) < 0)
    {
        g_print ("jackd process id doesn't exist\n");

        pid = -2;
    }
  
    return pid;
}
