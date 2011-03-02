#include <olibc/libarg.h>
#include "test.h"

int main (void) {
	char * string = "arg1 arg2 arg3 \"arg4 arg5\"";
	char ** argv;
	//char ** scan;
	//int i;
	int argc;

	oc_test_banner ("argv_make");
	if ( (argv = argv_make (string, &argc)) == null )
		goto go_fail;

	//for ( scan = argv, i=0; *scan != null; scan++, i++ )
	//	printf ("[%d] %s\n", i, *scan);

	ofree_array (argv);

	if ( argc == 4 )
		printf ("ok\n");
	else {
go_fail:
		printf ("failed\n");
	}

	return 0;
}
