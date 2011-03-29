#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	char * src;
	char * dst = "ab!c tt";

	oc_test_banner ("trim");

	oc_strdup (src, "\tab!c tt  \t\n", 12);
	if ( src == null ) {
		printf ("failed\n");
		return 0;
	}

	trim (src);
	printf ("%s\n", strcmp (src, dst) ? "failed" : "ok");
	ofree (src);

	oc_test_banner ("trim_r");
	{
		char * buf;

		oc_strdup (src, "\tab!c tt  \t\n", 12);
		if ( src == null ) {
			printf ("failed\n");
			return 0;
		}

		if ( (buf = trim_r (src, true)) == null ) {
			printf ("failed\n");
			return 0;
		}

		printf ("%s\n", strcmp (buf, dst) ? "failed" : "ok");
		ofree (buf);
	}

	return 0;
}
