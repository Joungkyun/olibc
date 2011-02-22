/** 
 * @file	libarg.h
 * @brief	Command line argument API header file
 *
 * This file includes proto type of command line argument apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-02-22 06:33:32 $
 * $Revision: 1.9 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libarg.h,v 1.9 2011-02-22 06:33:32 oops Exp $ */
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
/**
 * Value of current option. Use by o_getopt API
 */
extern char o_optarg[ARGLENGTH];

/**
 * String length of o_optarg variable. Use by o_getopt API
 */
extern int o_optlen;

/**
 * The o_cmdarg variable has command line arguments that
 * removed option arguments. This variable called by o_getopt
 * api and is must memory freed with free() function.
 */
extern char ** o_cmdarg;

extern int _ogetopt_cmd_int; //!< Number of o_cmdarg array
extern int _ogetopt_chk_int; //!< o_getopt internal global variable
#endif

/*
 * This function is analogous with getopt function, but this function
 * has serveral features. see also man page or doc page
 *
 * This function basically support short and long option
 *
 * After use this function, must need to free global variable o_cmdarg
 * with ofree_array
 *
 * Before use this function, must init _ogetopt_chk_int = -1 and
 * _ogetopt_cmd_int = 0.
 */
int o_getopt (int oargc, char ** oargv, CChar * opt, const struct o_option * longopt);

/* argv_make follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * argv_make make array variables from string like argv
 * return value must free with ofree_array */
char ** argv_make (CChar * stream, int * oargc);

/* split follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * split function make array variables from string with each charactor of given string
 * See the man page split.1.
 * return value must free with ofree_array */
char ** split (CChar * src, int * oargc, CChar * delimiter);

/* argv_free follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * argv_free freed return value of argv_make */
void ofree_array (char ** argv_array);

/* unconvert_quoted_blank follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * get number of white space on strings */
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
