#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	char * src;

	oc_strdup (src, "aAbBcC", 6);

	/*
	 * strtolower test
	 */
	{
		oc_test_banner ("strtolower");
		strtolower (src);
		
		if ( strcmp ("aabbcc", src) )
			printf ("failed\n");
		else
			printf ("ok\n");
	}

	/*
	 * strtoupper test
	 */
	{
		oc_test_banner ("strtoupper");
		strtoupper (src);
		if ( strcmp ("AABBCC", src) )
			printf ("failed\n");
		else
			printf ("ok\n");
	}

	ofree (src);

	return 0;
}
