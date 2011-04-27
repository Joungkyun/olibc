#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	oc_test_banner ("setansi");

	setansi (stdout, RED, false);
	printf ("red\n");
	setansi (stdout, ENDANSI, false);

	return 0;
}
