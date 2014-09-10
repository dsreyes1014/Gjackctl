#include "get_jack_gpid.h"

GPid 
get_jack_gpid ()
{
    FILE *cmd;
    gchar s[32];   
    GPid pid;
    
    cmd = popen ("pgrep jackd", "r");
    pid = -2;

    /* Here we get the `GPid pid` from the `cmd` statement and convert 
	it to a gint using the `g_ascii_strtoll ()` function. */
	while (fgets (s, sizeof (s), cmd) != NULL)
	{
		pid = g_ascii_strtoll (s, NULL, 10);
	}
	
	pclose (cmd);

    //g_print ("From `get_jack_gpid.c`: %d\n", pid);

    if (kill (pid, 0) == -1)
    {
        g_print ("jackd process id doesn't exist\n");
    }
  
    return pid;
}
