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

int main (int argc, char ** argv) {
	char	src[5] = { 0, };
	char	* p = null,
		 	* q = null,
			* apiname;

	// follow is EUC-KR "한글"
	sprintf (src, "%c%c%c%c", 0xc7, 0xd1, 0xb1, 0xdb);

	oc_test_banner ("convert_charset");

	// convert EUC-KR to UTF-8
	p = charset_conv (src, "EUC-KR", "UTF-8");
	if ( p == null ) {
		Failure ("EUC-KR => UTF-8");
		goto skip;
	}

	// convert UTF-8 to EUC-KR
	q = charset_conv (p, "UTF-8", "EUC-KR");
	if ( q == null ) {
		Failure ("UTF-8 => EUC-KR");
		goto skip;
	}

	// compare revoked string and original string
	if ( strcmp (src, q) == 0 )
		Success;
	else
		Failure ("EUC-KR <==> UTF-8");

skip:

	if ( p != null )
		free (p);

	if ( q != null )
		free (q);

	return ret;
}
