/** 
 * @file	libarg.h
 * @brief	Command line argument API header file
 *
 * This file includes proto type of command line argument apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-02-22 19:11:09 $
 * $Revision: 1.10 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libarg.h,v 1.10 2011-02-22 19:11:09 oops Exp $ */
#ifndef LIBARG_H
#define LIBARG_H

#include <olibc/oc_type.h>

/**
 * @brief long option structure for o_getopt API
 */
struct o_option {
	char * name;	//!< long option name
	int required;	//!< whether must need value or not
	int value;		//!< short option that correspond to long option
};

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
extern char o_optarg[ARGLENGTH];
extern int o_optlen;
extern char ** o_cmdarg;
extern int _ogetopt_cmd_int;
extern int _ogetopt_chk_int;
#endif

int o_getopt (int oargc, char ** oargv, CChar * opt, const struct o_option * longopt);
char ** argv_make (CChar * stream, int * oargc);
char ** split (CChar * src, int * oargc, CChar * delimiter);
void ofree_array (char ** argv_array);
int get_whitespace (CChar * src);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
