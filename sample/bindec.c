#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	size_t	len;
	/*
	 * dec2bin test
	 */
	{
		char	* src = "1000";
		char	* dst;

		oc_test_banner ("dec2bin");
		len = dec2bin (src, &dst);

		if ( dst == null )
			goto go_dec2bin_fail;

		if ( ! strcmp ("1111101000", dst) )
			printf ("ok\n");
		else {
go_dec2bin_fail:
			printf ("failed\n");
		}

		ofree (dst);
	}

	/*
	 * long2bin test
	 */
	{
		Long64	src = 1000;
		char	* dst;

		oc_test_banner ("long2bin");
		len = long2bin (src, &dst);

		if ( dst == null )
			goto go_long2bin_fail;

		if ( ! strcmp ("1111101000", dst) )
			printf ("ok\n");
		else {
go_long2bin_fail:
			printf ("failed\n");
		}

		ofree (dst);
	}

	/*
	 * bin2dec test
	 */
	{
		char	* src = "1111101000";
		UInt	dst;

		oc_test_banner ("bin2dec");
		dst = bin2dec (src);

		if ( dst == 1000 )
			printf ("ok\n");
		else
			printf ("failed\n");
	}

	/*
	 * bin2long test
	 */
	{
		char	* src = "1111101000";
		ULong64	dst;

		oc_test_banner ("bin2long");
		dst = bin2long (src);

		if ( dst == 1000 )
			printf ("ok\n");
		else
			printf ("failed\n");
	}

	return 0;
}
