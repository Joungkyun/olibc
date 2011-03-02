#include <olibc/libpcre.h>
#include "test.h"

int main (int argc, char ** argv) {
	char * s = "a!bcd!e\nfg!hijA!B";
	char * buf;
	int ret = 0;

	//
	// preg_replace api test
	//
	oc_test_banner ("preg_replace");
	buf = preg_replace ("/!([^!]+)!/", "!aa!", s, &ret);
	if ( buf != null ) {
		char * c;
		char * buf_t;
		int cc = 0;

		if ( ret == 14 ) {
			buf_t = buf;
			while ( (c = strstr (buf_t, "!aa!")) != null ) {
				cc++;
				buf_t = c + 1;
			}

			printf ("%s\n", cc == 2 ? "ok" : "failed\n");
		} else
			printf ("failed\n");
	} else
		printf ("failed\n");

	ofree (buf);

	//
	// preg_replace_arr api test
	//

	{
		char *regex[3] = { "/X/", "/Y/", "/Z/" };
		char *rep[3] = { "x", "y", "z" };

		oc_test_banner ("preg_replace_arr");
		buf = preg_replace_arr (regex, rep, "opqrstuvwXYZ", 3);

		if ( buf != null ) {
			if ( ! strcmp (buf + 9, "xyz") )
				printf ("ok\n");
			else
				printf ("failed\n");
		} else
			printf ("failed\n");

		//printf ("%s\n", buf);
		//printf ("%s\n", buf + 9);

		ofree (buf);
	}



	return 0;
}
