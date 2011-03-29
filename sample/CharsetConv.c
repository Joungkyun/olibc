#include <olibc/libstring.h>
#include "test.h"

int main (int argc, char ** argv) {
	char src[5] = { 0, };
	char * p = null,
		  * q = null;

	// follow is EUC-KR "한글"
	sprintf (src, "%c%c%c%c", 0xc7, 0xd1, 0xb1, 0xdb);

	oc_test_banner ("convert_charset");

	// convert EUC-KR to UTF-8
	p = charset_conv (src, "EUC-KR", "UTF-8");
	if ( p == null ) {
		printf ("failed\n");
		goto skip;
	}

	// convert UTF-8 to EUC-KR
	q = charset_conv (p, "UTF-8", "EUC-KR");
	if ( q == null ) {
		printf ("failed\n");
		goto skip;
	}

	// compare revoked string and original string
	if ( strcmp (src, q) == 0 )
		printf ("ok\n");
	else
		printf ("failed\n");

skip:

	if ( p != null )
		free (p);

	if ( q != null )
		free (q);

	return 0;
}
