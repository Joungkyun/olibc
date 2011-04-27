#include <olibc/libarg.h>
#include "test.h"

int main (void) {
	char * string = "a b c d e      f";

	oc_test_banner ("argv_make");
	printf ("%s\n", (get_whitespace (string)) == 10 ? "ok" : "failed");

	return 0;
}
