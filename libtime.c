/* $Id: libtime.c,v 1.6 2011-02-13 17:34:30 oops Exp $ */
#include <oc_common.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include <libtime.h>

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
