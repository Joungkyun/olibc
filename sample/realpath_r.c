#include <olibc/libfile.h>
#include <errno.h>
#include "test.h"

int main (void) {
	char	* path = "test.ln",
			path_c[PATH_MAX + 1] = { 0, },
			* buf = null;

	oc_test_banner ("realpath_r");

	if ( getcwd (path_c, PATH_MAX) == null )
		goto go_fail;

	strcat (path_c, "/");
	strcat (path_c, "test.txt");

	if ( (buf = realpath_r (path)) == null )
		goto go_fail;

	if ( ! strcmp (path_c, buf) )
		printf ("ok\n");
	else {
go_fail:
		printf ("failed\n");
	}
	ofree (buf);

	return 0;
}
