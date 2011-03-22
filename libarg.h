/** 
 * @file	libarg.h
 * @brief	Command line argument API header file
 *
 * This file includes proto type of command line argument apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-22 16:05:11 $
 * $Revision: 1.14 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libarg.h,v 1.14 2011-03-22 16:05:11 oops Exp $ */
#ifndef LIBARG_H
#define LIBARG_H

#include <olibc/oc_type.h>

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
extern char ** split (CChar * src, int * oargc, CChar * delimiter);
extern void ofree_array (char ** argv_array);
extern int get_whitespace (CChar * src);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
