/** 
 * @file	oc_common.h
 * @brief	Common build environments for olibc
 *
 * This file includes common header files and declare
 * proto type of internal apis.
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-28 07:45:30 $
 * $Revision: 1.25 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: oc_common.h,v 1.25 2011-03-28 07:45:30 oops Exp $ */

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

bool only_whitespace (CChar * stream, CInt length);
UInt get_charcount (CChar * str, size_t sl, CChar * del, size_t dl);
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
