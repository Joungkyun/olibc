/*
 *  Copyright (C) 2011 JoungKyun.Kim
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

/** 
 * @file	libidn.c
 * @brief	IDN (Internationalized Doman Name) API
 *
 * This file includes idn apis for easliy using
 * <p>
 * IDN is used punycode that subscribed RFC4592, and this
 * punycode apis is linking libidn library. So, pubycode apis
 * of olibc is follows LGPL because libidn has LGPL license.
 * <p>
 * olibc has basically LBPL license, but the punycode
 * apis don't have LBPL. Thus, olibc support libogc.so and
 * libogc.a for LGPL punycode apis. Attention, liboc.so
 * and liboc.a don't include punycode apis.
 * <p>
 * If you want to dynamic link, use libolibc.so. The libolibc.so
 * has symbols of liboc and libogc. The libolibc.a don't support.
 *
 * @sa	http://www.gnu.org/software/libidn/
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-02-22 18:11:46 $
 * $Revision: 1.12 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libidn.c,v 1.12 2011-02-22 18:11:46 oops Exp $ */
#include <oc_common.h>
#include <libidn.h>
#include <libstring.h>

#include <idna.h>
#include <punycode.h>
#include <stringprep.h>

#ifdef EANBLE_NLS
#include <langinfo.h>
#endif

/*
 * replace code for stringprep_utf8_to_ucs4
// need memory free
UInt * toucs4 (CChar * s, CChar * from) // {{{
{
	UChar * p, * q, * buf;
	size_t l, i, j;
	UInt * z;
	UInt bsize = 8;

	l = i = j = 0;

	p = charset_conv (s, from, "UCS-4");
	if ( p == NULL )
		p = charset_conv (s, "UTF-8", "UCS-4");

	if ( p == NULL )
		return NULL;

	oc_malloc (z, sizeof (UInt) * (bsize + 1));
	if ( z == NULL ) {
		ofree (p);
		return NULL;
	}

	buf = p;
	while ( true ) {
		if ( i == 0 || (i & 0x03) == 0 ) { // ( !i || (i%4) == 0 )
			p += 2;
		}

		q = p + 1;
		if ( ! (*p & 0xff) && ! (*q & 0xff) )
			break;

		if ( l > 7 && (l & 0x07) == 0 ) {
			bsize += 8;
			oc_realloc (z, sizeof (UInt) * (bsize + 1));
			if ( z == NULL ) {
				ofree (p);
				return NULL;
			}
		}

		z[l] = ((*p << 8) | *q) & 0xffff;
		OC_DEBUG ("%4d. %04x | %04x =  U+%04x\n", i, *p << 8, *q, z[l]);
		l++;
		p += 2;
		i += 4;
	}
	z[l] = 0x00;
	ofree (buf);

	return z;
} // }}}
*/

/**
 * @brief	convert between punycode and international domain
 * @param	domain international domain for converting
 * @param	mode encode(set 0) or decode(set 1)
 * @param	debug set 1, print debug messages
 * @return	return length of dst argument
 *
 * dst argument is must freed with free()
 */
OLIBC_API
UInt convert_punycode_r (CChar * src, UChar ** dst, bool mode, CChar * charset) // {{{
{
#ifdef HAVE_LIBIDN
#ifdef HAVE_ICONV_H
	int dlen, rc;
	char *p, *r;
	uint32_t *q;

	*dst = NULL;

	if ( src == NULL || strlen (src) < 1 )
		return 0;

	dlen = strlen (src);
	oc_strdup_r (*dst, src, 0);

	// remove after newline
	{
		char * newline;
		if ( (newline = strchr (*dst, '\n')) != NULL )
			*newline = 0;
	}

	if ( ! mode ) {
		//
		// Encoding mode
		//
		if ( charset == NULL ) {
			p = stringprep_locale_to_utf8 (*dst);
			if ( p == NULL )
				oc_error ("%s: could not convert from %s to UTF-8.\n",
					 	 *dst, stringprep_locale_charset ());
		} else {
			p = charset_conv (*dst, charset, "UTF-8");
			stringprep_locale_charset_cache = "UTF-8";
		}

		if ( p == NULL ) {
			ofree (*dst);
			return 0;
		}

		q = stringprep_utf8_to_ucs4 (p, -1, NULL);
		ofree (p);

		if ( !q ) {
			oc_error ("%s: could not convert to UCS-4 from UTF-8.\n", *dst);
			ofree (*dst);
			return 0;
		}

#ifdef __OCDEBUG__
		{
			size_t i;
			for ( i = 0; q[i]; i++ )
				OC_DEBUG ("input[%d] = U+%04x\n", i, q[i] & 0xffff);
		}
#endif

		rc = idna_to_ascii_4z (q, &r, 0);
		ofree (q);

		if ( rc != IDNA_SUCCESS ) {
			oc_error ("%s: idna_to_ascii_from_locale() failed with error %d.\n",
					 *dst, rc);
			ofree (*dst);
			return 0;
		}
	} else {
		//
		// Decoding mode
		//
		p = stringprep_locale_to_utf8 (*dst);
		if ( !p ) {
			oc_error ("%s: could not convert from %s to UTF-8.\n",
					 *dst, stringprep_locale_charset ());
			ofree (*dst);
			return 0;
		}

		rc = idna_to_unicode_8z4z (p, &q, 0|0);
		ofree (p);

		if (rc != IDNA_SUCCESS) {
			free (q);
			oc_error (
				"%s: idna_to_unicode_locale_from_locale() "
				"failed with error %d.\n",
				 *dst, rc
			 );
			ofree (*dst);
			return 0;
		}

#ifdef __OCDEBUG__
		{
			size_t i;
			for (i = 0; q[i]; i++)
				OC_DEBUG ("output[%d] = U+%04x\n", i, q[i] & 0xffff);
		}
#endif

		p = stringprep_ucs4_to_utf8 (q, -1, NULL, NULL);
		ofree (q);
		if ( !p ) {
			oc_error ("%s: could not convert from UCS-4 to UTF-8.\n", *dst);
			ofree (*dst);
			return 0;
		}

		if ( charset == NULL ) {
			r = stringprep_utf8_to_locale (p);
			if ( !r )
				oc_error ("%s: could not convert from UTF-8 to %s.\n",
						 *dst, stringprep_locale_charset ());
		} else
			r = charset_conv (p, "UTF-8", charset);

		ofree (p);

		if ( ! r ) {
			ofree (*dst);
			return 0;
		}
	}

	ofree (*dst);

	dlen = strlen (r);
	oc_strdup (*dst, r, dlen);
	ofree (r);
	if ( *dst == NULL )
		return 0;

	return dlen;
#else
	oc_error ("olibc was compiled without iconv library\n");
	return domain;
#endif
#else
	oc_error ("olibc was compiled without libidn\n");
	return domain;
#endif
} // }}}

/**
 * @brief	convert between punycode and international domain
 * @param	domain international domain for converting
 * @param	mode encode(set 0) or decode(set 1)
 * @param	debug deprecated (no action)
 * @return	converted string (static memory)
 *
 * No thread safe.
 */
OLIBC_API
char * convert_punycode (char * domain, int mode, int debug) // {{{
{
	static char conv[512] = { 0, };
	UChar * dst;
	UInt len;

	mode = mode ? true : false;
	len = convert_punycode_r (domain, &dst, mode, NULL);
	if ( len == 0 || dst == NULL )
		return conv;

	oc_safe_cpy (conv, dst, 512);
	ofree (dst);
	return conv;
} // }}}

/**
 * @example punycode.c
 *   convert_punycode() and convert_punycode_r() test file
 */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
