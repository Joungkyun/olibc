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
 * @file	oc_common.h
 * @brief	Common build environments for olibc
 *
 * This file includes common header files and declare
 * proto type of internal apis.
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-04-05 06:09:58 $
 * $Revision: 1.28 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: oc_common.h,v 1.28 2011-04-05 06:09:58 oops Exp $ */

#ifndef OC_COMMON_H
#define OC_COMMON_H

#ifdef HAVE_CONFIG_H
#include <olibc-config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#ifdef HAVE_LIBINTL_H
#include <libintl.h>
#endif

#if defined _WIN32 || defined __CYGWIN__
#	include "win95nt.h"
#	ifdef OLIBC_EXPORTS
#		define OLIBC_API __declspec(dllexport)
#	else
#		define OLIBC_API __declspec(dllimport)
#	endif
#else
#	if defined(__GNUC__) && __GNUC__ >= 4
#		define OLIBC_API __attribute__ ((visibility("default")))
#	else
#		define OLIBC_API
#	endif
#endif

#include <limits.h>
#include <errno.h>
#include <olibc/oc_type.h>

#ifdef HAVE_ICONV_H
	#define HAVE_ICONV 1
	#ifndef ICONV_CONST
		#ifdef HAVE_LIBICONV
			#define ICONV_CONST const
		#else
			#define ICONV_CONST
		#endif
	#endif
#endif

#if ! defined(__x86_64__) && ! defined(LLONG_MAX)
	/* Minimum and maximum values a `signed long long int' can hold.  */
	#define LLONG_MAX    9223372036854775807LL
	#define LLONG_MIN    (-LLONG_MAX - 1LL)

	/* Maximum value an `unsigned long long int' can hold.  (Minimum is 0.)  */
	#define ULLONG_MAX   18446744073709551615ULL
#endif


bool only_whitespace (CChar * stream, CInt length);
size_t get_charcount (CChar * str, size_t sl, CChar * del, size_t dl);
Bit64 devided64_high_low (Long64 v);
Long64 combined64_high_low (Bit64 v);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
