#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	long long src = 123456789;
	char * buf;

	/*
	 * numberFormat test
	 */
	{
		oc_test_banner ("numberFormat");
		buf = numberFormat (src, 0, '.', ',', 0);
		if ( strcmp ("123,456,789", buf) )
			printf ("failed\n");
		else
			printf ("ok\n");
	}

	/*
	 * human_size test
	 */
	{
		oc_test_banner ("human_size");
		buf = human_size (src, false, true);
		if ( strcmp ("117.73 Mb", buf) )
			printf ("failed\n");
		else
			printf ("ok\n");
	}

	return 0;
}
