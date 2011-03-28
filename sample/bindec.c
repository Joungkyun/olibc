#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	size_t	len;
	int		i;
	char	* buf;
	char	u[32];
	char	* unit[4] = { "32bit", "32bit negative", "64bit", "64bit negative" };
	char	* str[4] = { "1000", "-1000", "4294968296", "-4294968296" };
	ULong64 dec[4] = { 1000, -1000, 4294968296, -4294968296 };
	char	* bin[4] = {
						"1111101000",
						"11111111111111111111110000011000",
						"100000000000000000000001111101000",
						"1111111111111111111111111111111011111111111111111111110000011000"
	};

	/*
	 * dec2bin test
	 */
	{
		for ( i=0; i<4; i++ ) {
			sprintf (u, "dec2bin%s", unit[i]);
			oc_test_banner (u);

			if ( (buf= dec2bin(str[i], &len)) == null ) {
				printf ("failed\n");
			} else {
				if ( ! strcmp (bin[i], buf) ) {
					printf ("ok\n");
					ofree (buf);
				} else
					printf ("failed\n");
			}
		}
	}

	/*
	 * long2bin test
	 */
	{
		for ( i=0; i<4; i++ ) {
			sprintf (u, "long2bin %s", unit[i]);
			oc_test_banner (u);

			if ( (buf= long2bin (dec[i], &len)) == null ) {
				printf ("failed\n");
			} else {
				if ( ! strcmp (bin[i], buf) ) {
					printf ("ok\n");
					ofree (buf);
				} else
					printf ("failed\n");
			}
		}
	}

	/*
	 * bin2dec test
	 */
	{
		for ( i=0; i<2; i++ ) {
			sprintf (u, "bin2dec %s", unit[i]);
			oc_test_banner (u);

			if ( bin2dec (bin[i]) == dec[i] )
				printf ("ok\n");
			else
				printf ("failed\n");
		}
	}

	/*
	 * bin2long test
	 */
	{
		for ( i=0; i<4; i++ ) {
			sprintf (u, "bin2long %s", unit[i]);
			oc_test_banner (u);

			if ( bin2long (bin[i]) == dec[i] )
				printf ("ok\n");
			else
				printf ("failed\n");
		}
	}

	return 0;
}
