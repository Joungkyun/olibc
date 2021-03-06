/*
 *                       Written by JoungKyun.Kim
 *            Copyright (c) 2017 JoungKyun.Kim <http://oops.org>
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

#include <olibc/libpcre.h>
#include "test.h"

int main (int argc, char ** argv) {
	char	* s = "a!bcd!e\nfg!hijA!B",
			* buf,
			* apiname;
	int		r = 0;

	//
	// preg_replace api test
	//
	oc_test_banner ("preg_replace");
	buf = preg_replace ("/!([^!]+)!/", "!aa!", s, &r);
	if ( buf != null ) {
		char * c;
		char * buf_t;
		int cc = 0;

		if ( r == 14 ) {
			buf_t = buf;
			while ( (c = strstr (buf_t, "!aa!")) != null ) {
				cc++;
				buf_t = c + 1;
			}

			if ( cc == 2 )
				Success;
			else
				Failure ("cc != 2");
		} else
			Failure ("r != 14");
	} else
		Failure ("buf == null");

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
				Success;
			else
				Failure ("");
		} else
			Failure ("buf == null");

		//printf ("%s\n", buf);
		//printf ("%s\n", buf + 9);

		ofree (buf);
	}



	return ret;
}
