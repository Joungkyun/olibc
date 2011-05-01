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

#include <olibc/libfile.h>
#include <unistd.h>
#include "test.h"

int main (void) {
	char	* path = "./test.txt",
			* buf,
			* apiname;
	size_t	len;

	oc_test_banner ("readfile");
	len = readfile (path, &buf);

	if ( buf == null || len != 192 )
		goto go_fail;

	if ( ! strncmp ("alias ", buf, 6) && strcmp (buf + 190, "ci") )
		Success;
	else {
go_fail:
		Failure ("");
	}
	ofree (buf);

	oc_test_banner ("writefile");
	{
		char * wfile = "./test-write.txt";
		// append mode test
		if ( file_exists (wfile, OC_IS_FILE) )
			unlink (wfile);
		if ( writefile ("./test-write.txt", "1231", 4, true) == false )
			Failure ("");

		len = readfile (wfile, &buf);
		if ( buf != null ) {
			if ( ! strcmp (buf, "1231") )
				Success;
			else
				Failure ("");
			ofree (buf);
		} else
			Failure ("");

		// removed test file
		if ( file_exists (wfile, OC_IS_FILE) )
			unlink (wfile);
	}

	return 0;
}
