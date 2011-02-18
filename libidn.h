/* $Id: libidn.h,v 1.3 2011-02-18 09:50:21 oops Exp $ */
#ifndef LIBIDN_H
#define LIBIDN_H

#include <olibc/oc_type.h>

#define ENDANSI		0
#define GRAY		1
#define BGRAY		2
#define RED			3
#define BRED		4
#define GREEN		5
#define BGREEN		6
#define YELLOW		7
#define BYELLOW		8
#define BLUE		9
#define BBLUE		10
#define MAGENTA		11
#define BMAGENTA	12
#define CYAN		13
#define BCYAN		14
#define WHITE		15
#define BWHITE		16

extern const char *stringprep_locale_charset_cache;

/* convert_racecode follow BPL License v.1 <http://devel.oops.org/document/bpl>
 * encode and decode race code for multibyte domain (com/net/org)
 *
 * str   => original domain
 * mode  => 0 : encode to racecode
 *          1 : decode from racecode
 * debug => verbose mode */
char * convert_racecode (char *str, int mode, int debug);

/* convert_punycode follows GPL License v2
 * encode and decode punycode for local multibyte domain
 *
 * src  => original domain
 * dst  => converted domain (need free() after use)
 * mode => false : encode to punycode
 *         true  : decode from punycode
 *
 * return value => length of dst
 */
UInt convert_punycode (CChar * src, UChar ** dst, bool mode);

/* convert_punycode follows GPL License v2
 * encode and decode punycode for local multibyte domain
 *
 * domain => original domain
 * mode   => 0 : encode to punycode
 *           1 : decode from punycode
 * debug => verbose mode */
char * convert_punycode (char * domain, int mode, int debug);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
