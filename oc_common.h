/** 
 * @file	oc_common.h
 * @brief	Common build environments for olibc
 *
 * This file includes common header files and declare
 * proto type of internal apis.
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-29 10:08:14 $
 * $Revision: 1.21.2.2 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: oc_common.h,v 1.21.2.2 2011-03-29 10:08:14 oops Exp $ */

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
	#define HAVE_ICONV 1 // for libidn
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

UInt get_charcount (CChar * str, CChar * del);
Bit64 devided64_high_low (ULong64 v);
ULong64 combined64_high_low (Bit64 v);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
