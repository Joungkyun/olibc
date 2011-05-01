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
 * $Date$
 * $Revision$
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id$ */
#include <oc_common.h>

/** @defgroup global_internalfunc Global internal functions of olibc
 * @{
 */

/**
 * @brief	Get number of delimiters in the input string
 * @param	str The input string
 * @param	sl	The length of input string
 * @param	del	The input delimiters
 * @param	dl	The length of delimiters string
 * @return	Number of delimiters in the input string
 */
size_t get_charcount (CChar * str, size_t sl, CChar * del, size_t dl) // {{{
{
	size_t no,
		   i,
		   j;

	if ( str == null || del == null )
		return 0;

	no = 0;

	for ( i=0; i<sl; i++ ) {
		for ( j=0; j<dl; j++ ) {
			if ( str[i] == del[j] ) {
				if ( i>0 && str[i-1] == '\\' )
					continue;

				no++;
			}
		}
	}

	return no;
} // }}}

/**
 * @brief	Devided 64bit to high and low bit.
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
 * @brief	Combined to 64bit integer with 32bit high and low bit.
 * @param	v struct of 23bit high and low bit
 * @return	64bit interger
 */
ULong64 combined64_high_low (Bit64 v) // {{{
{
	if ( v.high == 0 )
		return (ULong64) v.low;

	return ((ULong64) v.high << 32) | v.low;
} // }}}

/** @} */ // end of global_internalfunc

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
