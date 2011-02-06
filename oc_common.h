/* $Id: oc_common.h,v 1.2 2011-02-06 14:57:34 oops Exp $ */
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

#define ofree (v) \
	if ( v != NULL ) free (v)

#define oc_error(fmt,...) \
	fprintf (stderr, "OC ERROR: " fmt, __VA_ARGS__)
#endif

#define oc_debug(fmt,...) \
	fprintf (stderr, \
			"%s:%d %s -> " fmt, \
			__FILE__, __LINE__, __func__, __VA_ARGS__ \
	)

/*
 * v -> allocated variable
 * size -> allocated size
 * ret -> if failed allocat, return value
 */
#define oc_malloc(v, size, ret) \
	v = malloc (size); \
	if ( v == NULL ) { \
		oc_error ("memory allocation failed\n"); \
		return ret; \
	} \

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
