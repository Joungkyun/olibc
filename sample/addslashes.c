#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	char * src = "'aa', \"bb\" and slash(\\)";
	char * dst = "\\'aa\\', \\\"bb\\\" and slash(\\\\)";
	unsigned char * buf;

	oc_test_banner ("addslashes");

	buf = addslashes (src, false);
	printf ("%s\n", strcmp (buf, dst) ? "failed" : "ok");
	ofree (buf);

	{
		size_t buflen;
		oc_test_banner ("addslashes_r");
		if ( addslashes_r (src, strlen (src), &buf, &buflen) == false ) {
			printf ("failed\n");
			return 0;
		}

		if ( buflen != 28 ) {
			printf ("failed\n");
			return 0;
		}

		printf ("%s\n", strcmp (buf, dst) ? "failed" : "ok");
		ofree (buf);
	}

	return 0;
}
