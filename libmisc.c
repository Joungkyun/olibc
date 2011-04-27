/* $Id: libmisc.c,v 1.2 2003-11-06 18:04:50 oops Exp $ */
#include <common.h>

void ofree (void *s) {
	if ( s != NULL ) free (s);
}

int get_charcount (char *str, char *del) {
	int no, i, j, len, dlen;

	no = 0;
	len = strlen (str);
	dlen = strlen (del);

	for ( i = 0; i < len; i++ ) {
		for ( j = 0; j < dlen; j++ ) {
			if ( str[i] == del[j] ) {
				if ( str[i-1] == '\\' )
					continue;

				no++;
			}
		}
	}

	return no;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
