#include <olibc/libarg.h>
#include "test.h"

int main (void) {
	/*
	 * Test case
	 * 	 command-line: ./o_getopt -a 10 --best arg1 arg2
	 */
	char * argv[] = { "./o_ogetopt", "-a",  "10", "--best", "arg1", "arg2" };
	int argc = 6;
	int opt;
	char app[1024] = { 0, };
	int bestopt = 0;
	int ok = 0;

	// declare long options
	static struct o_option longopt[] = {
		{ "append", required_arguments, 'a' },
		{ "best", no_arguments, 'b' },
		{ 0, 0, 0 }
	};

	// o_getopt initialize
	_ogetopt_cmd_int = 0;
	_ogetopt_chk_int = -1;

	oc_test_banner ("o_getopt");

	while ( (opt = o_getopt (argc, (const char **) argv, "a:b", longopt)) != -1 ) {
		switch (opt) {
			case 'a' :
				if ( o_optlen > 0 )
					strcpy (app, o_optarg);
				else {
					//fprintf (stderr, "No argument with -%c option\n", opt);
					goto go_ogetopt_fail;
				}
				break;
			case 'b' :
				bestopt++;
				break;
			default :
				//fprintf (stderr, "Usage: %s [-a optarg|-b] arg1 arg2\n", argv[0]);
				goto go_ogetopt_fail;
		}
	}

	// check o_cmdarg length
	if ( _ogetopt_cmd_int != 2 ) {
		//fprintf (stderr, "usage: %s [-a optarg|-b] arg1 arg2\n", argv[0]);
		goto go_ogetopt_fail;
	}

	if ( ! strcmp (*o_cmdarg++, "arg1") )
		ok++;
	if ( ! strcmp (*o_cmdarg, "arg2") )
		ok++;

	if ( bestopt && ! strcmp (app, "10") && ok == 2 )
		printf ("ok\n");
	else {
go_ogetopt_fail:
		printf ("failed\n");
	}
	return 0;
}
