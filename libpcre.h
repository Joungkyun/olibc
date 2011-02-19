/* $Id: libpcre.h,v 1.5 2011-02-19 14:38:43 oops Exp $ */
#ifndef LIBPCRE_H
#define LIBPCRE_H

#ifdef HAVE_LIBPCRE
#include <pcre.h>
#endif

/* preg_quote follows PHP License 2.02 <http://www.php.net/license/2_02.txt>
 * Quote regular expression characters
 *
 * default, if none delimiter, quoted
 * . \ + * ? [ ^ ] $ ( ) { } = ! > < | ,
 *
 * delimiters is additional quoted charactors except default quoted charactor
 *
 * return value must free memory */
char * preg_quote (CChar * src, CChar * delim);

/* preg_replace follows PHP License 2.02 <http://www.php.net/license/2_02.txt>
 * Perform a regular expression search and replace
 *
 * regex   => check regular expression
 * replace => replacement string about regular expression
 * subject => original string
 * retlen  => string length after convert
 *
 * return value must free memory */
char * preg_replace (char *regex, char *replace, char *subject, int *retlen);

/* preg_replace follows PHP License 2.02 <http://www.php.net/license/2_02.txt>
 * support array with preg_replace
 *
 * regex   => array of check regular expression
 * replace => array of replacement string about regular expression
 * subject => original string
 * retlen  => number of regex array */
char * preg_replace_arr (char *regex[], char *replace[], char *subject, int regarr_no);

/* preg_grep follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * this function operated like system grep with pecre regular expression
 *
 * regex   => array of check regular expression
 * str     => original string
 * opt     => same as -v option of system grep
 *
 * return value must free memory */
char * preg_grep (char *regex, char *str, int opt);

/* preg_match follows BPL License v.1 <http://devel.oops.org/document/bpl>
 *
 * if regex matched, return 1, nor unmatched, return 0
 *
 * regex   => array of check regular expression
 * str     => original string */
int preg_match (char *regex, char *subject);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
