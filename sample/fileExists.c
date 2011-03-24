#include <olibc/libfile.h>
#include "test.h"

int main (void) {
	char *path = "/var";
	bool a, b, c;

	oc_test_banner ("file_exists");
	a = file_exists (path, OC_IS_NCHK);
	b = file_exists (path, OC_IS_FILE);
	c = file_exists (path, OC_IS_DIR);

	printf ("%s\n", (a && ! b && c) ? "ok" : "failed\n");

	return 0;
}
