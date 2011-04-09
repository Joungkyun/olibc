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
 *  This file is part of olibc.
 */

/** 
 * @file	libarg.h
 * @brief	Command line argument API header file
 *
 * This file includes proto type of command line argument apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-04-09 16:03:05 $
 * $Revision: 1.2 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libarg.h,v 1.2 2011-04-09 16:03:05 oops Exp $ */
#ifndef LIBARG_H
#define LIBARG_H

#include <olibc/oc_type.h>

# ifdef __cplusplus
extern "C"
{
# endif

/**
 * @brief long option structure for o_getopt API
 */
typedef struct {
	char * name;	//!< long option name
	int required;	//!< whether must need value or not
	int value;		//!< short option that correspond to long option
} o_option;

#ifndef ARGLENGTH
#define ARGLENGTH 1024
#endif

#ifndef required_arguments
#define required_arguments 1
#endif

#ifndef no_arguments
#define no_arguments 0
#endif

#ifndef LIBARG_SRC
extern char	o_optarg[ARGLENGTH];
extern int	o_optlen;
extern char	** o_cmdarg;
extern int	_ogetopt_cmd_int;
extern int	_ogetopt_chk_int;
#endif

extern int o_getopt (
	int oargc, CChar ** oargv,
	CChar * opt, const o_option * longopt
);
extern char ** argv_make (CChar * stream, int * oargc);
extern void ofree_array (char ** argv_array);
extern int get_whitespace (CChar * src);

# ifdef __cplusplus
}
# endif

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
