/* $Id: common.h,v 1.5 2003-11-06 18:04:50 oops Exp $ */
#ifndef COMMON_H
#define COMMON_H

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

void ofree (void *s);
int get_charcount (char *str, char *del);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
