/** 
 * @file	libtime.c
 * @brief	Time API
 *
 * This file includes time apis for easliy using
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-02-21 20:03:05 $
 * $Revision: 1.7 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libtime.c,v 1.7 2011-02-21 20:03:05 oops Exp $ */
#include <oc_common.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include <libtime.h>

/**
 * @brife Return current Unix timestamp with microseconds
 * @param	void
 * @return	double decimal : current Unix timestamp with microseconds
 *
 * microtime() returns the current Unix timestamp with microseconds.
 * This function is only available on operating systems that
 * support the gettimeofday() system call.
 */
OLIBC_API
double microtime (void) // {{{
{
#ifdef HAVE_SYS_TIME_H
	struct timeval tp;
	long sec = 0L;
	double ret = 0;
	double msec = 0.0;

	if ( gettimeofday ((struct timeval *) &tp, '\0') == 0) {
		msec = (double) (tp.tv_usec / 1000000.00);
		sec = tp.tv_sec;

		if (msec >= 1.0) msec -= (long) msec;
		ret = sec + msec;
	}

	return ret;
#else
	oc_error ("olibc compiled without sys/time.h\n");
	return 0;
#endif
} // }}}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
