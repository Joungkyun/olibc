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

/* $Id: split.c,v 1.1 2011-04-09 15:54:22 oops Exp $ */

#include <olibc/libarg.h>
#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	char    ** sep, **sep_t,
			* str = "abcAdefAghi",
			* apiname;
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
		Success;
	else {
go_split_fail:
		Failure ("");
	}

	{
		char	*sep[4] = { "123", "456", "789", null };
		char	* buf = "123/456/789",
				* res;
		oc_test_banner ("join");

		res = join ("/", (const char **) sep);
		if ( ! strcmp (buf, res) )
			Success;
		else
			Failure ("");
		ofree (res);
	}

	return 0;
}
