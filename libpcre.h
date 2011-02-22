/** 
 * @file	libpcre.h
 * @brief	Extended perl compatible regular expression API header file.
 *
 * This file includes proto type of libpcre apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * @date	2011/02/21
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libpcre.h,v 1.11 2011-02-22 12:29:13 oops Exp $ */
#ifndef LIBPCRE_H
#define LIBPCRE_H

#ifdef HAVE_LIBPCRE
#include <pcre.h>
#endif

/*
 * Quote regular expression characters
 *
 * default, if none delimiter, quoted
 * . \ + * ? [ ^ ] $ ( ) { } = ! > < | ,
 *
 * delimiters is additional quoted charactors except default quoted charactor
 *
 * Return value that is not NULL is must free memory
 */
char * preg_quote (CChar * src, CChar * delim);

/*
 * Perform a regular expression search and replace
 *
 * regex   => check regular expression
 * replace => replacement string about regular expression
 * subject => original string
 * retlen  => string length after convert
 *
 * return value must free memory
 * This code see also PHP's preg_replace.
 */
char * preg_replace (char *regex, char *replace, char *subject, int *retlen);

/* preg_replace follows PHP License 2.02 <http://www.php.net/license/2_02.txt>
 * support array with preg_replace
 *
 * regex   => array of check regular expression
 * replace => array of replacement string about regular expression
 * subject => original string
 * retlen  => number of regex array */
char * preg_replace_arr (char ** regex, char ** replace, char * subject, int regarr_no);

/* preg_grep follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * this function operated like system grep with pecre regular expression
 *
 * regex   => array of check regular expression
 * str     => original string
 * opt     => same as -v option of system grep
 *
 * return value must free memory */
char * preg_grep (CChar *regex, CChar *str, bool opt);

/* preg_fgrep follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * this function operated like system grep with pecre regular expression
 *
 * regex   => array of check regular expression
 * path    => input file
 * opt     => same as -v option of system grep
 *
 * return value must free memory */
char * preg_fgrep (CChar * regex, CChar * path, bool opt);

/*
 * Searches subject for a match to the regular expression given
 * in regex.
 *
 * if regex matched, return true, nor unmatched, return false
 *
 * regex   => array of check regular expression
 * str     => original string
 */
bool preg_match (CChar * regex, CChar * subject);

/*
 * returns regex matched count.
 *   >0  Success: value is the number of elements filled in
 *    0   Internal failed
 * 	 -1  Failed to match
 *
 * [in]  regex   => array of check regular expression
 * [in]  str     => original string
 * [out] matches => array of matched string
 */
int preg_match_r (CChar * regex, CChar * subject, CChar *** matches);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
