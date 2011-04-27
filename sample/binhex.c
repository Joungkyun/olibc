#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	char * src = "11111101";
	char * buf;

	/*
	 * bin2hex
	 */
	{
		oc_test_banner ("bin2hex");
		buf = bin2hex (src);

		if ( strcmp ("fd", buf) )
			printf ("failed\n");
		else
			printf ("ok\n");
	}

	/*
	 * bin2hex_r
	 */
	{
		size_t l;
		oc_test_banner ("bin2hex_r");
		l = bin2hex_r (src, &buf);

		if ( l != 2 )
			goto go_bin2hex_fail;

		if ( ! strcmp ("fd", buf) )
			printf ("ok\n");
		else {
go_bin2hex_fail:
			printf ("failed\n");
		}
	}

	/*
	 * hex2bin
	 */
	{
		char * tmp;
		oc_test_banner ("hex2bin");
		tmp = hex2bin (buf);
		ofree (buf);

		if ( ! strcmp (src, tmp) )
			printf ("ok\n");
		else
			printf ("failed\n");
		ofree (tmp);
	}

	return 0;
}
