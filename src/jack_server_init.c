#include "jack_server_init.h"

gint 
jack_server_init ()
{
	gchar *jack_args[10];

	jack_args[0] = "jackd";
	jack_args[1] = "-ngjackctl";
	jack_args[2] = rt_arg ();
	jack_args[3] = "-P75";
	jack_args[4] = driver_arg1;
	jack_args[5] = driver_arg2;
	jack_args[6] = NULL;
}
