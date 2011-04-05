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
 * @file	libtime.c
 * @brief	Time API
 *
 * This file includes time apis for easliy using
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-04-05 06:09:58 $
 * $Revision: 1.15 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libtime.c,v 1.15 2011-04-05 06:09:58 oops Exp $ */
#include <oc_common.h>

#ifdef HAVE_LIBRT
	#include <time.h>
	#define oc_clock_gettime(x) clock_gettime (CLOCK_REALTIME, x)
	typedef struct timespec oc_time_t;
	#define DNSEC 1000000000
#elif HAVE_GETTIMEOFDAY
	#include <sys/time.h>
	#define oc_clock_gettime(x) gettimeofday (x, '\0')
	typedef struct timeval oc_time_t;
	#define DNSEC 1000000
#else
	#define oc_clock_gettime(x) EFAULT
	typedef long oc_time_t;
	#define DNSEC 1000000
#endif

#include <libtime.h>

/**
 * @brief Return current Unix timestamp with microseconds
 * @return	double decimal : current Unix timestamp with microseconds<br />
 *          If return value is 0, system can't support clock_gettime()
 *          or gettimeofday() funtcion.
 *
 * microtime() returns the current Unix timestamp with microseconds.
 * This function is only available on operating systems that
 * support the gettimeofday() or clock_gettime() system call.
 *
 * If system enables to support system call gettimeofday both and
 * clock_gettime(), olibc use clock_gettime() system call.
 */
OLIBC_API
double microtime (void) // {{{
{
	oc_time_t	tp;
	long		* tp_1,
				sec = 0L;
	double		ret = 0,
				msec = 0.0;

	tp_1 = (long *) &tp;

	if ( oc_clock_gettime ((oc_time_t *) &tp) == 0) {
		sec = *tp_1;
		msec = (*(tp_1 + 1) / (double) DNSEC);

		if (msec >= 1.0) msec -= (long) msec;
		ret = sec + msec;
	}
	OC_DEBUG ("STRING: %lu.%lu\n", *tp_1, *(tp_1 + 1));
	OC_DEBUG ("FLOAT : %.09f\n", ret);

	return ret;
} // }}}

/**
 * @example microtime.c
 *   The example for microtime() api
 */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
