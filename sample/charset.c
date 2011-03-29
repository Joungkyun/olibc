#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	unsigned char euc[5] = { 0, };
	unsigned char utf[7] = { 0, };

	// hangul jeong
	sprintf ((char *) euc, "%c%c", 0xc1, 0xa4);
	sprintf ((char *) utf, "%c%c%c", 0xec, 0xa0, 0x95);

	/*
	 * is_ksc5601 test
	 */

	oc_test_banner ("is_ksc5601");
	printf ("%s\n", is_ksc5601 (euc[0], euc[1]) ? "ok" : "failed");

	oc_test_banner ("is_utf8");
	printf ("%s\n", is_utf8 (utf) ? "ok" : "failed");

	return 0;
}
