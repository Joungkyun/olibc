#include <olibc/libfile.h>
#include <unistd.h>
#include "test.h"

int main (void) {
	char * path = "./test.txt";
	char * buf;
	size_t len;

	oc_test_banner ("readfile");
	buf = readfile (path);
	len = strlen (buf);

	if ( len != 192 )
		goto go_fail;

	if ( ! strncmp ("alias ", buf, 6) && strcmp (buf + 190, "ci") )
		printf ("ok\n");
	else {
go_fail:
		printf ("failed\n");
	}
	ofree (buf);

	oc_test_banner ("writefile");
	{
		char * wfile = "./test-write.txt";
		// append mode test
		if ( file_exists (wfile, _IS_FILE) )
			unlink (wfile);
		if ( writefile ("./test-write.txt", "1231", true) == -1 )
			printf ("failed\n");

		buf = readfile (wfile);
		printf ("%s\n", ! strcmp (buf, "1231") ? "ok" : "failed");
		ofree (buf);

		// removed test file
		if ( file_exists (wfile, _IS_FILE) )
			unlink (wfile);
	}

	return 0;
}
