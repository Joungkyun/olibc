/* $Id: libidn.c,v 1.6 2011-02-18 09:50:21 oops Exp $ */
#include <oc_common.h>
#include <libidn.h>

#include <idna.h>
#include <punycode.h>
#include <stringprep.h>

#ifdef EANBLE_NLS
#include <langinfo.h>
#endif

char * decode_race (char *domain, char *charset, int debug);
char * encode_race (char *domain, char *charset, int debug);
int permit_extension (char *tail);
void strtolower (char *str);

#ifdef HAVE_ICONV_H
char * convert_racecode (char * domain, int mode, int debug) {
	const char delimiters[] = ".";
	static char retconv[1024];
	char conv[512];
	char charset[32];
	char chkname[512];
	char *token, *btoken;
	int len = strlen (domain);

	if ( len > 512 )
		return domain;

	memset (retconv, 0, 1024);
	memset (chkname, 0, 32);
	memset (charset, 0, 32);
	memset (conv, 0, 512);

#ifdef HAVE_LIBIDN
	strcpy (charset, stringprep_locale_charset ());
#else
	strcpy (charset, "EUC-KR");
#endif

	/* convert to low case */
	strtolower ( domain );

	token = strtok_r ( domain, delimiters, &btoken );

	if ( token == NULL ) {
		char tmpval[512] = { 0, };

		if ( ! mode )
			strcpy (tmpval, encode_race (chkname, charset, debug));
		else
			strcpy (tmpval, decode_race (chkname, charset, debug));

		sprintf ( retconv, "%s.", tmpval );
	} else {
		while ( token != NULL ) {
			char tmpval[512] = { 0, };
			memset (conv, 0, 512);

			if ( ! mode )
				strcpy (tmpval, encode_race (token, charset, debug));
			else
				strcpy (tmpval, decode_race (token, charset, debug));

			sprintf ( conv, "%s.", tmpval );
			strcat ( retconv, conv );
			token = strtok_r ( NULL, delimiters, &btoken );
		}
	}

	memset ( retconv + strlen(retconv) - 1, 0, 1);

	return retconv;
}
#else
char * convert_racecode (char * domain, int mode, int debug) {
	fprintf (stderr, "ERROR: olibc compiled without iconv library\n");
	return domain;
}
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
UInt convert_punycode_r (CChar * src, UChar ** dst, bool mode) // {{{
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
		p = stringprep_locale_to_utf8 (*dst);
		if ( p == NULL ) {
			oc_error ("%s: could not convert from %s to UTF-8.\n",
				 	 *dst, stringprep_locale_charset ());

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

		r = stringprep_utf8_to_locale (p);
		ofree (p);
		if ( !r ) {
			oc_error ("%s: could not convert from UTF-8 to %s.\n",
					 *dst, stringprep_locale_charset ());
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
char * convert_punycode (char * domain, int mode, int debug) // {{{
{
	static char conv[512] = { 0, };
	UChar * dst;
	UInt len;

	mode = mode ? true : false;
	len = convert_punycode_r (domain, &dst, mode);
	if ( len == 0 || dst == NULL )
		return conv;

	oc_safe_cpy (conv, dst, 512);
	ofree (dst);
	return conv;
} // }}}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
