/* $Id: libarg.h,v 1.3 2011-02-07 06:42:07 oops Exp $ */
#ifndef LIBARG_H
#define LIBARG_H

struct o_option {
	char * name;
	int required;
	int value;
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
/* global variable of o_getopt function that have option argument */
extern char o_optarg[ARGLENGTH];

/* global variable that length of option argument by o_getopt function */
extern int o_optlen;

/*
 * global variable of o_getopt function
 * this variable have value that removed option string by o_getopt function
 * and must need to free
 */
extern char ** o_cmdarg;

extern int _ogetopt_cmd_int;
extern int _ogetopt_chk_int;
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
int o_getopt (int oargc, char ** oargv, const char * opt, const struct o_option * longopt);

/* argv_make follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * argv_make make array variables from string like argv
 * return value must free with ofree_array */
char ** argv_make (char * stream, int * oargc);

/* split follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * split function make array variables from string with each charactor of given string
 * See the man page split.1.
 * return value must free with ofree_array */
char ** split (char * stream, int * oargc, char * delimiter);

/* argv_free follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * argv_free freed return value of argv_make */
void ofree_array (char ** stream);

/* unconvert_quoted_blank follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * get number of white space on strings */
int get_whitespace (char * stream);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
