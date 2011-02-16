/* $Id: libidn.c,v 1.5 2011-02-16 10:46:28 oops Exp $ */
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

char * convert_punycode (char * domain, int mode, int debug) {
#ifdef HAVE_LIBIDN
#ifdef HAVE_ICONV_H
	static char conv[512] = { 0, };
	int dlen, rc;
	char *p, *r;
	uint32_t *q;
	char *z;

	if ( domain == NULL )
		return conv;

	dlen = strlen (domain);

	if ( dlen > 511 )
		strncpy (conv, domain, 511);
	else
		strcpy (conv, domain);

	if ( dlen > 256 )
		return conv;

	if ( conv[dlen - 1] == '\n' )
		conv[dlen -1] = 0;

	if ( ! mode ) {
		//p = stringprep_locale_to_utf8 (conv);
		p = charset_conv (domain, "EUC-KR", "UTF-8");
		if ( p == NULL ) {
			fprintf (stderr, "ERROR: %s: could not convert from %s to UTF-8.\n",
				 	 conv, charset_conv());

			return conv;
		}

		q = stringprep_utf8_to_ucs4 (p, -1, NULL);
		ofree (p);

		z = charset_conv (p, "UTF-8", "UCS-4BE");

		if ( !q ) {
			fprintf (stderr, "ERROR: %s: could not convert from UCS-4 to UTF-8.\n", conv);
			return conv;
		}

		if ( debug ) {
			size_t i;
			for ( i = 0; q[i]; i++ ) {
				fprintf (stderr, "input[%d] = U+%04x\n", i, q[i] & 0xFFFF);
				fprintf (stderr, "input[%d] = U+%04x\n", i, z[i] & 0xFFFF);
			}
		}

		rc = idna_to_ascii_4z (q, &r, 0);
		ofree (q);

		if ( rc != IDNA_SUCCESS ) {
			fprintf (stderr, "ERROR: %s: idna_to_ascii_from_locale() failed with error %d.\n",
					 conv, rc);
			return conv;
		}
	} else {
		p = stringprep_locale_to_utf8 (conv);
		if ( !p ) {
			fprintf (stderr, "ERROR: %s: could not convert from %s to UTF-8.\n",
					 conv, stringprep_locale_charset ());
			return conv;
		}

		q = stringprep_utf8_to_ucs4 (p, -1, NULL);
		if ( !q ) {
			ofree (p);
			fprintf (stderr, "ERROR: %s: could not convert from UCS-4 to UTF-8.\n", conv);
			return conv;
		}

		rc = idna_to_unicode_8z4z (p, &q, 0|0);
		ofree (p);

		if (rc != IDNA_SUCCESS) {
			free (q);
			fprintf (stderr,
				"ERROR: %s: idna_to_unicode_locale_from_locale() "
				"failed with error %d.\n",
				 conv, rc
			 );
			return conv;
		}

		if ( debug ) {
			size_t i;
			for (i = 0; q[i]; i++)
				fprintf (stderr, "output[%d] = U+%04x\n", i, q[i] & 0xFFFF);
		}

		p = stringprep_ucs4_to_utf8 (q, -1, NULL, NULL);
		if ( !p ) {
			ofree (q);
			fprintf (stderr, "ERROR: %s: could not convert from UCS-4 to UTF-8.\n", conv);
			return conv;
		}

		r = stringprep_utf8_to_locale (p);
		ofree (p);
		if ( !r ) {
			fprintf (stderr, "ERROR: %s: could not convert from UTF-8 to %s.\n",
					 conv, stringprep_locale_charset ());
			return conv;
		}

	}

	if ( strlen (r) > 510 ) {
		ofree (r);
		return conv;
	}

	memset (conv, 0, 512);
	memmove (conv, r, strlen (r));
	ofree (r);

	return conv;
#else
	fprintf (stderr, "ERROR: olibc compiles without iconv library\n");
	return domain;
#endif
#else
	fprintf (stderr, "ERROR: olibc compiles without libidn\n");
	return domain;
#endif
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
