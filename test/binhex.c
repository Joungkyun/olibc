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

#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	char * src = "11111101";
	char * buf, * apiname;
	size_t len;

	/*
	 * bin2hex
	 */
	{
		oc_test_banner ("bin2hex");
		//if ( (buf = bin2hex (src, null)) == null )
		if ( (buf = bin2hex (src, &len)) == null )
			goto go_bin2hex_fail;

		if ( ! strcmp ("fd", buf) )
			Success;
		else {
go_bin2hex_fail:
			Failure ("");
		}
	}

	/*
	 * hex2bin
	 */
	{
		char * tmp;

		if ( buf == null )
			buf = strdup ("fd");

		oc_test_banner ("hex2bin");
		tmp = hex2bin (buf, null);
		ofree (buf);

		if ( tmp != null && ! strcmp (src, tmp) )
			Success;
		else
			Failure ("");
		ofree (tmp);
	}

	return ret;
}
