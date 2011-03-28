#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	size_t	len;
	/*
	 * dec2bin test
	 */
	{
		char	* src = "-1000";
		char	* dst;

		oc_test_banner ("dec2bin");
		if ( (dst= dec2bin (src, &len)) == null )
			goto go_dec2bin_fail;

		if ( ! strcmp ("11111111111111111111110000011000", dst) )
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
		//Long64	src = 1000;
		Long64	src = (((Long64) 1)<<32) +1000; // 4294968296
		char	* dst;

		oc_test_banner ("long2bin");
		if ( (dst = long2bin (src, &len)) == null )
			goto go_long2bin_fail;

		if ( ! strcmp ("100000000000000000000001111101000", dst) )
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
		Long32	dst;

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
		//char	* src = "1111111111111111111111111111111111111111111111111111110000011000"; // -1000
		//char	* src = "1111101000"; // 1000
		char	* src = "100000000000000000000001111101000"; // 4294968296
		Long64	dst;

		oc_test_banner ("bin2long");
		dst = bin2long (src);

		if ( dst == (Long64) 4294968296 )
			printf ("ok\n");
		else
			printf ("failed\n");
	}

	return 0;
}
