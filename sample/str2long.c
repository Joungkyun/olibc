#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	/*
	 * str2long test
	 */
	{
		char	* src = "1234567";
		Long64	buf;

		oc_test_banner ("str2long");
		buf = str2long (src);
		printf ("%s\n", buf == 1234567 ? "ok" : "failed");
	}

	/*
	 * str2double test
	 */
	{
		char	* src = "1234567.12345",
				src_t[16] = { 0, };
		double	buf;

		oc_test_banner ("str2double");
		buf = str2double (src);
		sprintf (src_t, "%.5f", buf);
		printf ("%s\n", ! strcmp (src, src_t) ? "ok" : "failed");
	}

	/*
	 * char2int test
	 */
	{
		char	src = '7';
		int		buf;

		oc_test_banner ("char2int");
		buf = char2int (src);
		printf ("%s\n", buf == 7 ? "ok" : "failed");
	}

	/*
	 * is_strint test
	 */
	{
		char	src = 'a';
		int		buf;

		oc_test_banner ("is_strint");
		buf = is_strint (src);
		printf ("%s\n", buf == false ? "ok" : "failed");
	}

	return 0;
}
