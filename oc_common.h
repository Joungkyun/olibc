/* $Id: oc_common.h,v 1.8 2011-02-09 15:57:11 oops Exp $ */
#ifndef OC_COMMON_H
#define OC_COMMON_H

#ifdef HAVE_CONFIG_H
#include <olibc-config.h>
#else
#define SUCCESS		0
#define FAILURE		1
#define LSUCCESS	1
#define LFAILURE	0
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

int get_charcount (char *str, char *del);

#define oc_error_debug(fmt,...) \
	fprintf (stderr, \
			"%s(%s:%d): " fmt, \
			__func__, __FILE__, __LINE__, __VA_ARGS__ \
	)

#ifdef __OCDEBUG__
#	define OC_DEBUG(fmt,...) \
		fprintf (stderr, \
				 "DEBUG: %s(%s:%d): " fmt, \
				 __func__, __FILE__, __LINE__, __VA_ARGS__ \
		);
#	define oc_error(fmt,...) \
			oc_error_debug(fmt, __VA_ARGS__)
#else
#	define OC_DEBUG(fmt,...)
#	define oc_error(fmt,...) \
			fprintf (stderr, "OC ERROR: " fmt, __VA_ARGS__)
#endif

#ifdef __OCMEMDEBUG__
#	define OC_MEM_DEBUG(fmt,...) \
		fprintf (stderr, \
				 "DEBUG: %s(%s:%d): " fmt, \
				 __func__, __FILE__, __LINE__, __VA_ARGS__ \
		);
#else
#	define OC_MEM_DEBUG(fmt,...)
#endif

#define ofree(v) \
	{ \
		OC_MEM_DEBUG ("%s%s\n", "Memory free", v == NULL ? ": NULL" : ""); \
		if ( v != NULL ) { \
			free (v); \
		} \
	}

#define oc_malloc(v, size) \
	{ \
		OC_MEM_DEBUG("%s\n", "Memory allocation"); \
		v = malloc (size); \
	}
	
#define oc_realloc(v, size) \
	{ \
		void * ptr; \
		OC_MEM_DEBUG("%s\n", "Memory reallocation"); \
		if ( (ptr = realloc (v, size)) == NULL ) { \
			ofree (v); \
			v = NULL; \
		} else { \
			v = ptr; \
		} \
	}

#define oc_strdup(v, val, size) \
	{ \
		oc_malloc(v, size + 1); \
		if ( v != NULL ) { \
			memcpy (v, val, size); \
			memset (v + size, 0, 1); \
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
		oc_strdup (v, data, strlen (ret)); \
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
#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
