#include <olibc/libfile.h>
#include <unistd.h>
#include "test.h"

int main (void) {
	char * path = "./test.txt";
	char * buf;
	size_t len;

	oc_test_banner ("readfile");
	len = readfile (path, &buf);

	if ( buf == null || len != 192 )
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
		if ( file_exists (wfile, OC_IS_FILE) )
			unlink (wfile);
		if ( writefile ("./test-write.txt", "1231", 4, true) == false )
			printf ("failed\n");

		len = readfile (wfile, &buf);
		if ( buf != null ) {
			printf ("%s\n", ! strcmp (buf, "1231") ? "ok" : "failed");
			ofree (buf);
		} else
			printf ("%s\n", "failed");

		// removed test file
		if ( file_exists (wfile, OC_IS_FILE) )
			unlink (wfile);
	}

	return 0;
}
