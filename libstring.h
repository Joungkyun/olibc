/** 
 * @file	libstring.h
 * @brief	String API header file
 *
 * This file includes proto type of string apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-29 18:18:35 $
 * $Revision: 1.29.2.4 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libstring.h,v 1.29.2.4 2011-03-29 18:18:35 oops Exp $ */
#ifndef LIBSTRING_H
#define LIBSTRING_H

#include <olibc/oc_type.h>

#define OC_ENDANSI	0   //!< declare ansi end
#define OC_GRAY		30  //!< declare ansi color gray
#define OC_BGRAY	40  //!< declare ansi color bold gray
#define OC_RED		31  //!< declare ansi color red
#define OC_BRED		41  //!< declare ansi color bold red
#define OC_GREEN	32  //!< declare ansi color green
#define OC_BGREEN	42  //!< declare ansi color bold green
#define OC_YELLOW	33  //!< declare ansi color yellow
#define OC_BYELLOW	43  //!< declare ansi color bold yellow
#define OC_BLUE		34  //!< declare ansi color blue
#define OC_BBLUE	44  //!< declare ansi color bold blue
#define OC_MAGENTA	35  //!< declare ansi color magenta
#define OC_BMAGENTA	45  //!< declare ansi color bold magenta
#define OC_CYAN		36  //!< declare ansi color cyan
#define OC_BCYAN	46  //!< declare ansi color bold cyan
#define OC_WHITE	37  //!< declare ansi color white
#define OC_BWHITE	47  //!< declare ansi color bold white

#ifndef MAX
#	define MAX(a, b)	(((a)>(b))?(a):(b))
#endif

void olibc_version (void);

char * addslashes (char * in, bool should_free);
bool addslashes_r (CChar * in, size_t inlen, char ** out, size_t * outlen);

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
