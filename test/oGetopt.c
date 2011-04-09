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

/* $Id: oGetopt.c,v 1.1 2011-04-09 15:54:22 oops Exp $ */

#include <olibc/libarg.h>
#include "test.h"

int main (void) {
	/*
	 * Test case
	 * 	 command-line: ./o_getopt -a 10 --best arg1 arg2
	 */
	char	* argv[] = { "./o_ogetopt", "-a",  "10", "--best", "arg1", "arg2" },
			app[1024] = { 0, },
			* apiname;
	int		argc = 6,
			opt,
			bestopt = 0,
			ok = 0;

	// declare long options
	static o_option longopt[] = {
		{ "append", required_arguments, 'a' },
		{ "best", no_arguments, 'b' },
		{ 0, 0, 0 }
	};

	// o_getopt initialize
	_ogetopt_cmd_int = 0;
	_ogetopt_chk_int = -1;

	oc_test_banner ("o_getopt");

	while ( (opt = o_getopt (argc, (const char **) argv, "a:b", longopt)) != -1 ) {
		switch (opt) {
			case 'a' :
				if ( o_optlen > 0 )
					strcpy (app, o_optarg);
				else {
					//fprintf (stderr, "No argument with -%c option\n", opt);
					goto go_ogetopt_fail;
				}
				break;
			case 'b' :
				bestopt++;
				break;
			default :
				//fprintf (stderr, "Usage: %s [-a optarg|-b] arg1 arg2\n", argv[0]);
				goto go_ogetopt_fail;
		}
	}

	// check o_cmdarg length
	if ( _ogetopt_cmd_int != 2 ) {
		//fprintf (stderr, "usage: %s [-a optarg|-b] arg1 arg2\n", argv[0]);
		goto go_ogetopt_fail;
	}

	{
		char ** cmdarg = o_cmdarg;

		if ( ! strcmp (*cmdarg++, "arg1") )
			ok++;
		if ( ! strcmp (*cmdarg, "arg2") )
			ok++;
	}

	if ( bestopt && ! strcmp (app, "10") && ok == 2 )
		Success;
	else {
go_ogetopt_fail:
		Failure ("");
	}

	ofree_array (o_cmdarg);
	return 0;
}
