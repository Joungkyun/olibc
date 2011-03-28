/** 
 * @file	libstring.h
 * @brief	String API header file
 *
 * This file includes proto type of string apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-28 10:43:57 $
 * $Revision: 1.42 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libstring.h,v 1.42 2011-03-28 10:43:57 oops Exp $ */
#ifndef LIBSTRING_H
#define LIBSTRING_H

#include <olibc/oc_type.h>

#define OC_ENDANSI		0	//!< declare ansi end
#define OC_GRAY		1	//!< declare ansi color gray
#define OC_BGRAY		2	//!< declare ansi color bold gray
#define OC_RED			3	//!< declare ansi color red
#define OC_BRED		4	//!< declare ansi color bold red
#define OC_GREEN		5	//!< declare ansi color green
#define OC_BGREEN		6	//!< declare ansi color bold green
#define OC_YELLOW		7	//!< declare ansi color yellow
#define OC_BYELLOW		8	//!< declare ansi color bold yellow
#define OC_BLUE		9	//!< declare ansi color blue
#define OC_BBLUE		10	//!< declare ansi color bold blue
#define OC_MAGENTA		11	//!< declare ansi color magenta
#define OC_BMAGENTA	12	//!< declare ansi color bold magenta
#define OC_CYAN		13	//!< declare ansi color cyan
#define OC_BCYAN		14	//!< declare ansi color bold cyan
#define OC_WHITE		15	//!< declare ansi color white
#define OC_BWHITE		16	//!< declare ansi color bold white

#ifndef MAX
#	define MAX(a, b)	(((a)>(b))?(a):(b))
#endif

extern void olibc_version (void);

extern bool addslashes (CChar * in, size_t inlen, char ** out, size_t * outlen);

extern void trim (char * str);
extern char * trim_r (char * str, bool should_free);

extern char ** split (CChar * src, int * oargc, CChar * delimiter);
extern char * join (CChar * glue, CChar ** sep);

extern Long64 str2long (CChar * src);
extern double str2double (CChar * src);
extern int char2int (CChar c);
extern bool is_strint (char c);

extern void setansi (FILE * stream, int color, bool noansi);

extern char * human_size (ULong64 size, bool sub, bool unit);
extern char * numberFormat (double num, int dec, char dec_point, char thousand_sep, bool print);

extern void strtolower (char * src);
extern void strtoupper (char * src);

extern ULong32 bin2hex_r (CChar * src, char ** dst);
extern char * bin2hex (CChar * src);
extern char * hex2bin (CChar * src);
extern Long32 bin2dec (CChar * src);
extern Long64 bin2long (CChar * src);
extern char * long2bin (Long64 dec, size_t * outlen);
extern char * dec2bin (CChar * src, size_t * outlen);

extern bool is_ksc5601 (int c1, int c2);
extern bool is_utf8 (UCChar * src);
extern char * charset_conv (CChar * src, CChar * from, CChar * to);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
