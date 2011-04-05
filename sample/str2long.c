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

/* $Id: str2long.c,v 1.5 2011-04-05 06:09:59 oops Exp $ */

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
