/** 
 * @file	libstring.h
 * @brief	String API header file
 *
 * This file includes proto type of string apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-29 11:06:01 $
 * $Revision: 1.29.2.2 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libstring.h,v 1.29.2.2 2011-03-29 11:06:01 oops Exp $ */
#ifndef LIBSTRING_H
#define LIBSTRING_H

#include <olibc/oc_type.h>

#define ENDANSI		0	//!< declare ansi end
#define GRAY		1	//!< declare ansi color gray
#define BGRAY		2	//!< declare ansi color bold gray
#define RED			3	//!< declare ansi color red
#define BRED		4	//!< declare ansi color bold red
#define GREEN		5	//!< declare ansi color green
#define BGREEN		6	//!< declare ansi color bold green
#define YELLOW		7	//!< declare ansi color yellow
#define BYELLOW		8	//!< declare ansi color bold yellow
#define BLUE		9	//!< declare ansi color blue
#define BBLUE		10	//!< declare ansi color bold blue
#define MAGENTA		11	//!< declare ansi color magenta
#define BMAGENTA	12	//!< declare ansi color bold magenta
#define CYAN		13	//!< declare ansi color cyan
#define BCYAN		14	//!< declare ansi color bold cyan
#define WHITE		15	//!< declare ansi color white
#define BWHITE		16	//!< declare ansi color bold white

#ifndef MAX
#	define MAX(a, b)	(((a)>(b))?(a):(b))
#endif

void olibc_version (void);

char * addslashes (char * in, bool should_free);
bool addslashes_r (UChar * in, size_t inlen, UChar ** out, size_t * outlen);

void trim (char * str);
char * trim_r (char * str, bool should_free);

Long64 str2long (CChar * src);
long double str2double (CChar * src);
int char2int (CChar c);
int check_int (char c);

void setansi (FILE * stream, int color, bool noansi);

char * human_size (double size, int sub, int unit);
char * human_size_r (ULong64 size, bool sub, bool unit);
char * numberFormat (double num, int dec, char dec_point, char thousand_sep, bool print);

void strtolower (char * src);
void strtoupper (char * src);

ULong32 bin2hex_r (CChar * src, char ** dst);
char * bin2hex (CChar * src);
char * hex2bin (CChar * src);
char * _hex2bin (CChar c);
Long32 bin2dec (CChar * src);
Long64 bin2long (CChar * src);
UInt long2bin (Long64 dec, char ** dst);
UInt dec2bin (CChar * src, char ** dst);

bool is_ksc5601 (int c1, int c2);
bool is_utf8 (UCChar * src);
char * charset_conv (CChar * src, CChar * from, CChar * to);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
