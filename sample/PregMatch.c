#include <olibc/libpcre.h>
#include "test.h"

int main (int argc, char ** argv) {
	char * s = "abcde\\nfghijAB";
	int ret = 0;

	/*
	 * preg_match API test
	 */
	{
		oc_test_banner ("preg_match");
		ret = preg_match ("/(c).*(gh).*/si", s);
		printf ("%s\n", ret ? "ok" : "falied");
	}

	/*
	 * preg_match_r API test
	 */
	{
		const char ** matches = null;

		oc_test_banner ("preg_match_r");
		ret = preg_match_r ("/(b).*(gh)/si", s, &matches);
		if ( ret > 0 ) {
			if (
					! strcmp ("bcde\\nfgh", matches[0]) &&
					! strcmp ("b", matches[1]) &&
					! strcmp ("gh", matches[2])
			   )
				printf ("%s\n", "ok");
			else
				printf ("%s\n", "falied");
			ofree (matches);
		} else {
			printf ("%s\n", "falied");
		}
	}

	return 0;
}
