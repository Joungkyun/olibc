/* $Id: libmisc.c,v 1.1 2003-09-27 09:15:37 oops Exp $ */
#include <common.h>

void ofree (void *s) {
	if ( s != NULL ) free (s);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
