#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	char * src = "11111101";
	char * buf;
	size_t len;

	/*
	 * bin2hex
	 */
	{
		oc_test_banner ("bin2hex");
		//if ( (buf = bin2hex (src, null)) == null )
		if ( (buf = bin2hex (src, &len)) == null )
			goto go_bin2hex_fail;

		if ( ! strcmp ("fd", buf) )
			printf ("ok (%d)\n", len);
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

		if ( buf == null )
			buf = strdup ("fd");

		oc_test_banner ("hex2bin");
		tmp = hex2bin (buf, null);
		ofree (buf);

		if ( tmp != null && ! strcmp (src, tmp) )
			printf ("ok\n");
		else
			printf ("failed\n");
		ofree (tmp);
	}

	return 0;
}
