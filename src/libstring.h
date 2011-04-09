/*
 *                       Written by JoungKyun.Kim
 *            Copyright (c) 2011 JoungKyun.Kim <http://oops.org>
 *
 * -----------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of JoungKyun.Kim nor the url of oops.org
 *       nor the names of their contributors may be used to endorse or
 *       promote products derived from this software without specific prior
 *       written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * -----------------------------------------------------------------------------
 *  This file is part of olibc.
 */

/** 
 * @file	libstring.h
 * @brief	String API header file
 *
 * This file includes proto type of string apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-04-09 16:03:05 $
 * $Revision: 1.2 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libstring.h,v 1.2 2011-04-09 16:03:05 oops Exp $ */
#ifndef LIBSTRING_H
#define LIBSTRING_H

#include <olibc/oc_type.h>

# ifdef __cplusplus
extern "C"
{
# endif

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

extern char * bin2hex (CChar * src, size_t * outlen);
extern char * hex2bin (CChar * src, size_t * outlen);
extern Long32 bin2dec (CChar * src);
extern Long64 bin2long (CChar * src);
extern char * long2bin (Long64 dec, size_t * outlen);
extern char * dec2bin (CChar * src, size_t * outlen);

extern bool is_ksc5601 (int c1, int c2);
extern bool is_utf8 (UCChar * src);
extern char * charset_conv (CChar * src, CChar * from, CChar * to);

# ifdef __cplusplus
}
# endif
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
