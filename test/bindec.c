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

/* $Id$ */

#include <olibc/libstring.h>
#include "test.h"

int main (void) {
	size_t	len;
	int		i;
	char	* buf, * apiname;
	char	u[32];
	char	* unit[4] = { "32bit", "32bit negative", "64bit", "64bit negative" };
	char	* str[4] = { "1000", "-1000", "4294968296", "-4294968296" };
	ULong64 dec[4] = { 1000, -1000, 4294968296, -4294968296 };
	char	* bin[4] = {
						"1111101000",
						"11111111111111111111110000011000",
						"100000000000000000000001111101000",
						"1111111111111111111111111111111011111111111111111111110000011000"
	};

	/*
	 * dec2bin test
	 */
	{
		for ( i=0; i<4; i++ ) {
			sprintf (u, "dec2bin %s", unit[i]);
			oc_test_banner (u);

			//if ( (buf= dec2bin(str[i], &len)) == null ) {
			if ( (buf= dec2bin(str[i], null)) == null ) {
				Failure ("return null");
			} else {
				if ( ! strcmp (bin[i], buf) ) {
					Success;
					ofree (buf);
				} else
					RESULT (false, "%s != %s\n", bin[i], buf)
			}
		}
	}

	/*
	 * long2bin test
	 */
	{
		for ( i=0; i<4; i++ ) {
			sprintf (u, "long2bin %s", unit[i]);
			oc_test_banner (u);

			//if ( (buf= long2bin (dec[i], null)) == null ) {
			if ( (buf= long2bin (dec[i], &len)) == null ) {
				Failure ("return null");
			} else {
				if ( ! strcmp (bin[i], buf) ) {
					Success;
					//printf ("ok (%d)\n", len);
					ofree (buf);
				} else
					RESULT (false, "%s != %s\n", bin[i], buf)
			}
		}
	}

	/*
	 * bin2dec test
	 */
	{
		for ( i=0; i<2; i++ ) {
			sprintf (u, "bin2dec %s", unit[i]);
			oc_test_banner (u);

			if ( bin2dec (bin[i]) == dec[i] )
				Success;
			else
#ifdef __x86_64__
				RESULT (false, "%s != %ld\n", bin[i], dec[i])
#else
				RESULT (false, "%s != %lld\n", bin[i], dec[i])
#endif
		}
	}

	/*
	 * bin2long test
	 */
	{
		for ( i=0; i<4; i++ ) {
			sprintf (u, "bin2long %s", unit[i]);
			oc_test_banner (u);

			if ( bin2long (bin[i]) == dec[i] )
				Success;
			else
#ifdef __x86_64__
				RESULT (false, "%s != %ld\n", bin[i], dec[i])
#else
				RESULT (false, "%s != %lld\n", bin[i], dec[i])
#endif
		}
	}

	return ret;
}
