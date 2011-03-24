#include <olibc/libarg.h>
#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	char    ** sep, **sep_t;
	char    * str = "abcAdefAghi";
	int     array_no,
			ok = 0;

	oc_test_banner ("split");
	sep = split (str, &array_no, "A");

	if ( sep == null )
		goto go_split_fail;

	if ( array_no != 3 ) {
		ofree_array (sep);
		goto go_split_fail;
	}

	sep_t = sep;
	if ( strcmp (*sep_t++, "abc") == 0 )
		ok++;

	if ( strcmp (*sep_t++, "def") == 0 )
		ok++;

	if ( strcmp (*sep_t, "ghi") == 0 )
		ok++;

	ofree_array (sep);

	if ( ok == 3 )
		printf ("ok\n");
	else {
go_split_fail:
		printf ("failed\n");
	}

	{
		char	*sep[4] = { "123", "456", "789", null };
		char	* buf = "123/456/789",
				* res;
		oc_test_banner ("join");

		res = join ("/", (const char **) sep);
		printf ("%s\n", ! strcmp (buf, res) ? "ok" : "failed");
		ofree (res);
	}

	return 0;
}
