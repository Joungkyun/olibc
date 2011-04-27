#include <olibc/libtime.h>
#include "test.h"

int main (void) {
	oc_test_banner ("microtime");
	printf ("%s\n", ((ULong) 0 != microtime ()) ? "ok" : "failed");

	return 0;
}

