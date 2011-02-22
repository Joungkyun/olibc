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
 *     * Neither the name of the JoungKyun.Kim nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
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
 */

/** 
 * @file	libtime.c
 * @brief	Time API
 *
 * This file includes time apis for easliy using
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-02-22 19:11:09 $
 * $Revision: 1.10 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libtime.c,v 1.10 2011-02-22 19:11:09 oops Exp $ */
#include <oc_common.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include <libtime.h>

/**
 * @brief Return current Unix timestamp with microseconds
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

/**
 * @example microtime.c
 *   microtime() test file
 */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
