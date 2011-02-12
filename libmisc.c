/* $Id: libmisc.c,v 1.7 2011-02-12 17:43:43 oops Exp $ */
#include <oc_common.h>

UInt get_charcount (CChar * str, CChar * del)
{
	UInt no, i, j, len, dlen;

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

/**
 * @brief	devided 64bit to high and low bit.
 * @param	v 64bit ingeger
 * @return	Bit64 struct
 */
Bit64 devided64_high_low (ULong64 v) // {{{
{
	Bit64 r;

	if ( v <= ULONG_MAX ) {
		r.high = 0;
		r.low = (ULong32) v;
		return r;
	}

	r.high = v >> 32;
	r.low  = v & 0xffffffffULL;

	return r;
} // }}}

/**
 * @brief	combined to 64bit integer with 32bit high and low bit.
 * @param	v struct of 23bit high and low bit
 * @return	64bit iterger
 */
ULong64 combined64_high_low (Bit64 v) // {{{
{
	if ( v.high == 0 )
		return (ULong64) v.low;

	return ((ULong64) v.high << 32) | v.low;
} // }}}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
