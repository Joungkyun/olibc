/* $Id: libmisc.c,v 1.5 2011-02-10 09:58:37 oops Exp $ */
#include <oc_common.h>

uint get_charcount (char *str, char *del) {
	uint no, i, j, len, dlen;

	if ( str == NULL || del == NULL )
		return 0;

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
