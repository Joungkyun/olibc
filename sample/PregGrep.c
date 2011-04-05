/*
 *                       Written by JoungKyun.Kim
 *            Copyright (c) 2011 JoungKyun.Kim <http://oops.org>
 *
 * -----------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 *     * Neither the name of JoungKyun.Kim nor the url of oops.org
 *       nor the names of their contributors may be used to endorse or
 *       promote products derived from this software without specific prior
 *       written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * -----------------------------------------------------------------------------
 * This file is part of olibc.
 */

/* $Id: PregGrep.c,v 1.3 2011-04-05 06:09:58 oops Exp $ */

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
