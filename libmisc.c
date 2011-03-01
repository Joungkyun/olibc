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
 * @file	libmisc.c
 * @brief	Internal AIPs for olibc
 *
 * This file includes internal apis of olibc
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-01 17:42:26 $
 * $Revision: 1.10 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libmisc.c,v 1.10 2011-03-01 17:42:26 oops Exp $ */
#include <oc_common.h>

UInt get_charcount (CChar * str, CChar * del)
{
	UInt no, i, j, len, dlen;

	if ( str == null || del == null )
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
