#include <common.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include <libtime.h>

#ifdef HAVE_SYS_TIME_H
double microtime (void) {
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
}
#else
double microtime (void) {
	fprintf (stderr, "ERROR: olibc compiled without sys/time.h\n");
}
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
