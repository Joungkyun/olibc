#include <olibc/libpcre.h>
#include "test.h"

int main (int argc, char ** argv) {
	char	* s = "\tabcde\nfghijAB\n\n  12300ase\n\n0psdfe\nasdf\n\n";
	char	* buf = null;
	char	* buf_t;
	int		i = 0;

	/*
	 * preg_grep API test
	 */
	{
		oc_test_banner ("preg_grep");
		buf = preg_grep ("/^[\\s]+[0-9]+/", s, false);
		if ( buf != null ) {
			if ( ! strcmp (buf, "  12300ase") )
				printf ("ok\n");
			else
				printf ("failed\n");
			ofree (buf);
		} else
			printf ("failed\n");

		oc_test_banner ("preg_grep reverse");
		buf = preg_grep ("/^[\\s]+[0-9]+/", s, true);

		i = 0;
		if ( buf != null ) {
			buf_t = strchr (buf, '\n');
			while ( buf_t != null ) {
				char * p;
				i++;
				p = strchr (buf_t + 1, '\n');
				buf_t = p;
			}

			{
				int z = strlen (buf);
				if ( i == 6 && buf[z-1] == '\n' && buf[z-2] == 'f' )
					printf ("ok\n");
				else
					printf ("failed\n");
			}

			// memory free
			ofree (buf);
		} else
			printf ("failed\n");
	}

	/*
	 * preg_fgrep API test
	 */
	{
		const char	* path = "./test.txt";

		oc_test_banner ("preg_fgrep");
		buf = preg_fgrep ("/[\\s]+scsi_host/", path, false);

		i = 0;
		if ( buf != null ) {
			buf_t = strchr (buf, '\n');
			while ( buf_t != null ) {
				char * p;
				i++;
				p = strchr (buf_t + 1, '\n');
				buf_t = p;
			}

			if ( i == 1 && preg_match ("/mptscsih$/ms", buf) )
				printf ("ok\n");
			else
				printf ("failed\n");

			// memory free
			ofree (buf);
		} else
				printf ("failed\n");

		oc_test_banner ("preg_fgrep reverse");
		buf = preg_fgrep ("/^alias[\\s]+/", path, true);
		printf ("%s\n", ! buf ? "ok" : "failed");
		if ( buf != null )
			ofree (buf);
	}

	return 0;
}
