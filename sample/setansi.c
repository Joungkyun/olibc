#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	oc_test_banner ("setansi");

	setansi (stdout, OC_RED, false);
	printf ("red\n");
	setansi (stdout, OC_ENDANSI, false);

	return 0;
}
