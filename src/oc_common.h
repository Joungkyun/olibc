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
 * $Date: 2011-04-13 09:36:16 $
 * $Revision: 1.3 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: oc_common.h,v 1.3 2011-04-13 09:36:16 oops Exp $ */

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

#ifdef ENABLE_NLS
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#ifdef HAVE_LIBINTL_H
#include <libintl.h>
#endif
#endif

#if defined _WIN32 || defined __CYGWIN__
#	include "win95nt.h"
#	ifdef OLIBC_EXPORTS
#		define OLIBC_API __declspec(dllexport)
#	else
#		define OLIBC_API __declspec(dllimport)
#	endif
#else
#	ifdef HAVE_VISIBILITY
#		define OLIBC_API __attribute__ ((visibility("default")))
#	else
#		define OLIBC_API
#	endif
#endif

#include <limits.h>
#include <errno.h>
#include <olibc/oc_type.h>

#if ! defined(__x86_64__) && ! defined(LLONG_MAX)
	/* Minimum and maximum values a `signed long long int' can hold.  */
	#define LLONG_MAX    9223372036854775807LL
	#define LLONG_MIN    (-LLONG_MAX - 1LL)

	/* Maximum value an `unsigned long long int' can hold.  (Minimum is 0.)  */
	#define ULLONG_MAX   18446744073709551615ULL
#endif

#ifdef HAVE_LSTAT_EMPTY_STRING_BUG
#define safe_lstat(x, y) lstat (x ? x : "/123", y)
#else
#define safe_lstat(x, y) lstat (x, y)
#endif

/*
 * Definition about memory api
 */

#define oc_error_debug(...) \
	{ \
		fprintf (stderr, "DEBUG: %s(%s:%d): ", __func__, __FILE__, __LINE__); \
		fprintf (stderr, __VA_ARGS__); \
	};

#ifdef __OCDEBUG__
	#define OC_DEBUG(...) oc_error_debug (__VA_ARGS__)
	#define oc_error(...) oc_error_debug ( __VA_ARGS__)
#else
	#define OC_DEBUG(...)
	#define oc_error(...) \
		{ \
			fprintf (stderr, "OC ERROR: "); \
			fprintf (stderr, __VA_ARGS__); \
		}
#endif

#ifdef __OCMEMDEBUG__
	#define OC_MEM_DEBUG(...) oc_error_debug (__VA_ARGS__)
#else
	#define OC_MEM_DEBUG(...)
#endif


#define oc_malloc(v, size) \
	{ \
		OC_MEM_DEBUG("Memory allocation\n"); \
		v = malloc (size); \
	}
	
#define oc_realloc(v, size) \
	{ \
		void * ptr_oc_ival; \
		OC_MEM_DEBUG("Memory reallocation\n"); \
		if ( (ptr_oc_ival = realloc (v, size)) == NULL ) { \
			ofree (v); \
			v = NULL; \
		} else { \
			v = ptr_oc_ival; \
		} \
	}

#define oc_strdup(v, val, size) \
	{ \
		OC_DEBUG ("strdup add size : %d + %d\n", size, (size < 4) ? 4 : 1); \
		oc_malloc(v, size + ((size < 4) ? 4 : 1)); \
		if ( v != NULL ) { \
			memcpy (v, val, size); \
			memset (v + size, 0, 1); \
		} \
	}

#define ofree(v) \
	{ \
		OC_MEM_DEBUG ("Memory free%s\n", v == NULL ? ": NULL" : ""); \
		if ( v != NULL ) { \
			free (v); \
			v = NULL; \
		} \
	}

#define OC_DEF_EXIT    0
#define OC_DEF_RETURN  1
#define OC_DEF_MALLOC  0
#define OC_DEF_REALLOC 1

#define oc_malloc_originate(type, v, size, ret, result) \
	{ \
		if ( type == OC_DEF_REALLOC ) { oc_realloc (v, size); } \
		else { oc_malloc (v, size); } \
		if ( v == NULL ) { \
			oc_error ("%s: memory %sallocation failed\n", __func__, type ? "re" : ""); \
			if ( result == OC_DEF_RETURN ) return ret; \
			exit (1); \
		} \
		if ( type == OC_DEF_MALLOC ) { memset (v, 0, size); } \
	}

/*
 * v -> allocated variable
 * size -> allocated size
 * ret -> if failed allocat, return value
 */
#define oc_malloc_r(v, size, ret) \
	oc_malloc_originate (OC_DEF_MALLOC, v, size, ret, OC_DEF_RETURN)

#define oc_realloc_r(v, size, ret) \
	oc_malloc_originate (OC_DEF_REALLOC, v, size, ret, OC_DEF_RETURN)

#define oc_malloc_die(v, size) \
	oc_malloc_originate (OC_DEF_MALLOC, v, size, ret, OC_DEF_EXIT)

#define oc_realloc_die(v, size) \
	oc_malloc_originate (OC_DEF_REALLOC, v, size, ret, OC_DEF_EXIT)

#define oc_strdup_originate(v, data, ret, result) \
	{ \
		oc_strdup (v, data, strlen (data)); \
		if ( v == NULL ) { \
			if ( result == OC_DEF_RETURN ) \
				return ret; \
			exit (1); \
		} \
	}

#define oc_strdup_r(v, data, ret) \
		 oc_strdup_originate (v, data, ret, OC_DEF_RETURN)

#define oc_strdup_e(v, data, ret) \
		 oc_strdup_originate (v, data, ret, OC_DEF_EXIT)


#define oc_safe_cpy(dst, src, size) \
	{ \
		UInt cpylen_oc_ival; \
		cpylen_oc_ival = strlen (src); \
		cpylen_oc_ival = (cpylen_oc_ival > (size-1)) ? size - 1 : cpylen_oc_ival; \
		memmove (dst, src, cpylen_oc_ival); \
		memset (dst + cpylen_oc_ival, 0, 1); \
	}

#define oc_safe_bcpy(dst, src, srclen, size) \
	{ \
		size = (srclen > (size-1)) ? size - 1 : srclen; \
		memmove (dst, src, size); \
		memset (dst + size, 0, 1); \
	}

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
