/* $Id: libstring.c,v 1.7 2003-09-19 10:52:31 oops Exp $ */
#include <common.h>
#include <libstring.h>

#include <idna.h>
#include <punycode.h>
#include <stringprep.h>

#ifdef EANBLE_NLS
#include <langinfo.h>
#endif

void memlocate_chk (char *str);
char * decode_race (char *domain, char *charset, int debug);
char * encode_race (char *domain, char *charset, int debug);

void memlocate_chk (char *str) {
	if (str == NULL) {
		fprintf (stderr, "Error: Failed memory location\n");
		exit (FAILURE);
	}
}

/* this function must memory free */
char * addslashes(char *str, int should_free) {
	/* maximum string length, worst case situation */
	char *new_str;
	char *source, *target;
	char *end;
	int length, new_length;

	length = strlen (str);

	if (!str) {
		new_length = 0;
		new_str = malloc (sizeof (char) * (length + 1) );
		memlocate_chk (new_str);

		strcpy (new_str, str);

		if (should_free) {
			free (str);
		}
		return new_str;
	}

	new_str = (char *) malloc((length?length:(length=strlen(str)))*2+1);
	memlocate_chk (new_str);

	source = str;
	end = source + length;
	target = new_str;

	while (source < end) {
		switch (*source) {
			case '\0':
				*target++ = '\\';
				*target++ = '0';
				break;
			case '\'':
			case '\"':
			case '\\':
				*target++ = '\\';
				/* break is missing *intentionally* */
			default:
				*target++ = *source;
				break;
		}

		source++;
	}

	*target = 0;
	new_length = target - new_str;
	if (should_free) {
		free(str);
	}
	new_str = (char *) realloc(new_str, new_length+1);
	return new_str;
}

/* trim is remove space charactor on behind and forward on string.
 * warn !! this function modified original variable !!
 */ 
void trim (char *str) {
	int len = strlen (str);
	int start = 0, end = 0, i = 0;
	
	for ( i=0; i<len; i++ ) {
		if ( ! isspace (str[i]) ) {
			start = i;
			break;
		}
	}

	for ( i=len-1; i>0; i-- ) {
		if ( ! isspace (str[i]) ) {
			end = i;
			break;
		}
	}

	if ( end < start ) {
		fprintf (stderr, "ERROR: end point is smaller than start point on trim function\n");
		exit (FAILURE);
	}

	if ( start == end ) {
		start = 0;
		end = len;
	}

	memset ( str + end + 1, '\0', 1);
	memmove ( str, str + start, end - start + 1);
	memset ( str + end - start +1, '\0', 1);
}

/* trim is remove space charactor on behind and forward on string.
 * this function must free
 */ 
char * trim_r (char *str, int should_free) {
	int len = strlen (str);
	int start = 0, end = 0, i = 0;
	char *ret;
	
	for ( i=0; i<len; i++ ) {
		if ( ! isspace (str[i]) ) {
			start = i;
			break;
		}
	}

	for ( i=len-1; i>0; i-- ) {
		if ( ! isspace (str[i]) ) {
			end = i;
			break;
		}
	}

	if ( end < start ) {
		fprintf (stderr, "ERROR: end point is smaller than start point on trim function\n");
		exit (FAILURE);
	}

	if ( start == end ) {
		start = 0;
		end = len;
	}

	ret = malloc (sizeof(char) * (len +	32));
	memlocate_chk (ret);

	memset (ret, '\0', sizeof(ret));
	memcpy (ret, str + start, end - start + 1);

	if ( should_free ) free (str);

	return ret;
}

long long str2long (char *s) {
	int len, i = 0, minus = 0, bufno = 0;
	long long x = 1, res = 0;
	char *buf;

	/* removed blank charactor */
	buf = trim_r (s, 0);
	len = strlen (buf);

	/* minus value check */
	if ( buf[0] == '-' ) minus = 1;

	for ( i = len - 1; i > -1; i-- ) {
		if ( (bufno = char2int (buf[i])) > 0 ) {
			res += bufno * x;
			x *= 10;
		}
	}

	if (minus) res *= -1;
	free (buf);

	return res;
}

long double str2double (char *s) {
	int len, i = 0, dotlen = 0;
	int minus = 0, bufno = 0;
	long double res = 0;
	long long l = 0, x = 1;
	float f = 0, y = 0.1;
	char *buf, *dot;

	buf = trim_r (s, 0);
	len = strlen (buf);

	dot = (char *) strchr (buf, '.');

	if ( buf[0] == '-' ) minus = 1;

	if ( dot == NULL ) {
		for ( i = len - 1; i > -1; i-- ) {
			if ( ! check_int (buf[i]) ) continue;
			else {
				l += char2int (buf[i]) * x;
				x *= 10;
			}
		}
	} else {
		dotlen = strlen (dot);

		if ( dotlen > 7 ) {
			fprintf (stderr, "ERROR: float length is too long. max 6");
			exit (FAILURE);
		}

		for ( i = len - dotlen + 1; i < len; i++ ) {
			if ( (bufno = char2int (buf[i])) > 0 ) {
				f += bufno * y;
				y /= 10;
			}
		}

		len = len - dotlen;
		for ( i = len; i > -1; i-- ) {
			if ( (bufno = char2int (buf[i])) > 0 ) {
				l += bufno * x;
				x *= 10;
			}
		}
	}

	res = l + f;
	if (minus) res *= -1;
	free (buf);

	return res;
}

int char2int (char c) {
	if (c > 47 && c < 58)
		return c - 48;

	return -1;
}

int check_int (char c) {
	if ( c < 48 || c > 57 ) return 0;
	return 1;
}

void setansi (FILE *stream, int color, int noansi) {
	int ansi = 0;

	switch ( color ) {
		case GRAY :
			ansi = 30;
			break;
		case BGRAY :
			ansi = 40;
			break;
		case RED :
			ansi = 31;
			break;
		case BRED :
			ansi = 41;
			break;
		case GREEN :
			ansi = 32;
			break;
		case BGREEN :
			ansi = 42;
			break;
		case YELLOW :
			ansi = 33;
			break;
		case BYELLOW :
			ansi = 43;
			break;
		case BLUE :
			ansi = 34;
			break;
		case BBLUE :
			ansi = 44;
			break;
		case MAGENTA :
			ansi = 35;
			break;
		case BMAGENTA :
			ansi = 45;
			break;
		case CYAN :
			ansi = 36;
			break;
		case BCYAN :
			ansi = 46;
			break;
		case WHITE :
			ansi = 37;
			break;
		case BWHITE :
			ansi = 47;
			break;
		default :
			ansi = 0;
	}

	if ( ! noansi ) {
		if ( color == ENDANSI )
			fprintf (stream, "[7;0m");
		else
			fprintf (stream, "[1;%dm", ansi);
	}
}

char * human_size (double size, int sub, int unit) {
	float res;
	unsigned char sunit[6], ssunit, re_unit[3];
	char * BYTE_C, bytes[1024];
	static char result[256];

	memset (result, '\0', sizeof(result));
	memset (sunit, '\0', sizeof(sunit));
	memset (re_unit, '\0', sizeof(re_unit));

	if (unit != 1) {
		strcpy (sunit, "Bytes");
		ssunit = 'B';
	} else {
		strcpy (sunit, "Bits");
		ssunit = 'b';
	}

	BYTE_C = (char *) numberFormat (size, 0, '.', ',', 0);
	strcpy (bytes, BYTE_C);
	free(BYTE_C);

	if (size < 1024) {
		sprintf (result, "%s %s", bytes, sunit);
	} else {
		if (size < 1048576 ) {
			res = (float) size/1024;
			memset (re_unit, 'K', 1);
		} else if (size < 1073741827) {
			res = (float) size/1048576;
			memset (re_unit, 'M', 1);
		} else if (size < 1099511627776) {
			res = (float) size/1073741827;
			memset (re_unit, 'G', 1);
		} else {
			res = (float) size/1099511627776;
			memset (re_unit, 'T', 1);
		}
		memset (re_unit + 1, ssunit, 1);

		if (sub)
			sprintf(result, "%.2f %s (%s %s)", res, re_unit, bytes, sunit);
		else
			sprintf(result, "%.2f %s", res, re_unit);
	}

	return result;
}

/* follows PHP license 2.02
 * This function must free */
char * numberFormat (double d, int dec, char dec_point, char thousand_sep, int print) {
	 char *tmpbuf, *resbuf;
	 char *src, *tgt;  /* source, target */
	 int tmplen, reslen = 0;
	 int count = 0;
	 int is_negative = 0;

	 if (d < 0) {
		 is_negative = 1;
		 d = -d;
	 }
	 dec = MAX(0, dec);
	 tmpbuf = (char *) malloc (1024);
	 memlocate_chk (tmpbuf);

	 tmplen = sprintf (tmpbuf, "%.*f", dec, d);
	 if ( !isdigit ((int) tmpbuf[0]) ) {
		 return tmpbuf;
	 }

	 if ( dec )
		 reslen = dec + 1 + (tmplen - dec - 1) + ((thousand_sep) ? (tmplen - 1 - dec - 1) / 3 : 0);
	 else
		 reslen = tmplen + ((thousand_sep) ? (tmplen - 1) / 3 : 0);

	 if (is_negative)
		 reslen++;

	 resbuf = (char *) malloc (reslen + 1);
	 memlocate_chk (resbuf);

	 src = tmpbuf + tmplen - 1;
	 tgt = resbuf + reslen;
	 *tgt-- = 0;

	 if ( dec ) {
		 while (isdigit ((int) *src)) {
			 *tgt-- = *src--;
		 }

		 *tgt-- = dec_point;  /* copy that dot */
		 src--;
	 }

	 while( src >= tmpbuf ) {
		 *tgt-- = *src--;
		 if (thousand_sep && (++count % 3) == 0 && src >= tmpbuf) {
			 *tgt-- = thousand_sep;
		 }
	 }
	 if ( is_negative ) {
		 *tgt-- = '-';
	 }

	 free(tmpbuf);

	 if (print) {
		 printf ("%s", resbuf);
		 free (resbuf);
		 return NULL;
	 } else {
		 return resbuf;
	 }
}

void strtolower (char *str) {
	int i = 0;
	int len = strlen (str);

	for ( i = 0; i < len; i++ )
		memset (str + i, tolower (str[i]), 1);
}

void strtoupper (char *str) {
	int i = 0;
	int len = strlen (str);

	for ( i = 0; i < len; i++ )
		memset (str + i, toupper (str[i]), 1);
}

char * hex2bin (char *str) {
	char *data;
	int len = strlen (str), i, j;

	data = malloc (sizeof (char) * (len + 4));
	memlocate_chk (data);

	memset (data, '\0', sizeof (data));

	for ( i = 0, j = 0; i < len; i++ ) {
		memcpy (data + j, _hex2bin (str[i]), 4);
		j += 4;
	}

	return data;
}

char * _hex2bin (char c) {
	static char h2b[5];

	memset (h2b, '\0', sizeof (h2b));

	if((c >= 0x61 && c <= 0x7a) || (c >= 0x41 && c <= 0x5a)) {
		switch (c) {
			case 'a' : memcpy (h2b, "1010", 4); break;
			case 'b' : memcpy (h2b, "1011", 4); break;
			case 'c' : memcpy (h2b, "1100", 4); break;
			case 'd' : memcpy (h2b, "1101", 4); break;
			case 'e' : memcpy (h2b, "1110", 4); break;
			case 'f' : memcpy (h2b, "1111", 4); break;
			case 'A' : memcpy (h2b, "1010", 4); break;
			case 'B' : memcpy (h2b, "1011", 4); break;
			case 'C' : memcpy (h2b, "1100", 4); break;
			case 'D' : memcpy (h2b, "1101", 4); break;
			case 'E' : memcpy (h2b, "1110", 4); break;
			case 'F' : memcpy (h2b, "1111", 4); break;
		}
	} else {
		switch (c) {
			case '0' : memcpy (h2b, "0000", 4); break;
			case '1' : memcpy (h2b, "0001", 4); break;
			case '2' : memcpy (h2b, "0010", 4); break;
			case '3' : memcpy (h2b, "0011", 4); break;
			case '4' : memcpy (h2b, "0100", 4); break;
			case '5' : memcpy (h2b, "0101", 4); break;
			case '6' : memcpy (h2b, "0110", 4); break;
			case '7' : memcpy (h2b, "0111", 4); break;
			case '8' : memcpy (h2b, "1000", 4); break;
			case '9' : memcpy (h2b, "1001", 4); break;
		}
	}

	return h2b;
}

int bin2dec (char *src) {
	int i, ret = 0;
	char var[2];

	for(i=0 ; i<8 ; i++) {
		sprintf(var, "%c", src[i]);
		ret += atoi(var) << (7 - i);
	}

	return ret;
}

#ifdef HAVE_ICONV_H
char * convert_racecode (char * domain, int mode, int debug) {
	static char conv[512];
	char charset[32];
	int len = strlen (domain);

	if ( len > 256 )
		return domain;

	memset (charset, '\0', sizeof (charset));
	memset (conv, '\0', sizeof (conv));

#ifdef HAVE_LIBIDN
	strcpy (charset, stringprep_locale_charset ());
#else
	strcpy (charset, "EUC-KR");
#endif

	if ( ! mode )
		strcpy (conv, encode_race (domain, charset, debug));
	else
		strcpy (conv, decode_race (domain, charset, debug));

	return conv;
}
#else
char * convert_racecode (char * domain, int mode, int debug) {
	fprintf (stderr, "ERROR: olibc compiled without iconv library\n");
	exit (FAILURE);
}
#endif

char * convert_punycode (char * domain, int mode, int debug) {
#ifdef HAVE_LIBIDN
#ifdef HAVE_ICONV_H
	static char conv[512];
	int dlen = strlen (domain), rc;
	char *p, *r;
	uint32_t *q;

	if ( dlen > 256 )
		return domain;

	if ( domain[dlen - 1] == '\n' )
		domain[dlen -1] = 0;

	if ( ! mode ) {
		p = stringprep_locale_to_utf8 (domain);
		if ( !p ) {
			fprintf (stderr, "ERROR: %s: could not convert from %s to UTF-8.\n",
				 	 domain, stringprep_locale_charset ());

			exit (FAILURE);
		}

		q = stringprep_utf8_to_ucs4 (p, -1, NULL);
		if ( !q ) {
			fprintf (stderr, "ERROR: %s: could not convert from UCS-4 to UTF-8.\n", domain);
			exit (FAILURE);
		}

		if ( debug ) {
			size_t i;
			for ( i = 0; q[i]; i++ )
				fprintf (stderr, "ERROR: input[%d] = U+%04x\n", i, q[i] & 0xFFFF);
		}

		rc = idna_to_ascii_4z (q, &r, 0);
		free (q);

		if ( rc != IDNA_SUCCESS ) {
			fprintf (stderr, "ERROR: %s: idna_to_ascii_from_locale() failed with error %d.\n",
					 domain, rc);
			exit (FAILURE);
		}
	} else {
		p = stringprep_locale_to_utf8 (domain);
		if ( !p ) {
			fprintf (stderr, "ERROR: %s: could not convert from %s to UTF-8.\n",
					 domain, stringprep_locale_charset ());
			exit (FAILURE);
		}

		q = stringprep_utf8_to_ucs4 (p, -1, NULL);
		if ( !q ) {
			free (p);
			fprintf (stderr, "ERROR: %s: could not convert from UCS-4 to UTF-8.\n", domain);
			exit (FAILURE);
		}

		rc = idna_to_unicode_8z4z (p, &q, 0|0);
		free (p);

		if (rc != IDNA_SUCCESS) {
			fprintf (stderr, "ERROR: %s: idna_to_unicode_locale_from_locale() failed with error %d.\n",
					 domain, rc);
			exit (FAILURE);
		}

		if ( debug ) {
			size_t i;
			for (i = 0; q[i]; i++)
				fprintf (stderr, "output[%d] = U+%04x\n", i, q[i] & 0xFFFF);
		}

		p = stringprep_ucs4_to_utf8 (q, -1, NULL, NULL);
		if ( !p ) {
			free (q);
			fprintf (stderr, "ERROR: %s: could not convert from UCS-4 to UTF-8.\n", domain);
			exit (FAILURE);
		}

		r = stringprep_utf8_to_locale (p);
		free (p);
		if ( !r ) {
			fprintf (stderr, "ERROR: %s: could not convert from UTF-8 to %s.\n",
					 domain, stringprep_locale_charset ());
			exit (FAILURE);
		}

	}

	if ( strlen (r) > 510 )
		return domain;

	memset ( conv, 0, sizeof (conv) );
	memmove ( conv, r, strlen (r));
	free (r);

	return conv;
#else
	fprintf (stderr, "ERROR: olibc compiles without iconv library\n");
	exit (FAILURE);
#endif
#else
	fprintf (stderr, "ERROR: olibc compiles without libidn\n");
	exit (FAILURE);
#endif
}

char * fileread (char * path) {
	FILE *fp;
	size_t fsize = 0, len = 0, length = 0;
	char tmp[FILEBUF] = { 0, }, *text;
	struct stat f;

	if ((fp = fopen(path, "rb")) == NULL) {
		fprintf(stderr, "ERROR: Can't open %s in read mode\n", path);
		exit (FAILURE);
	}

	stat (path, &f);
	if ( f.st_size < 1 )
		return NULL;

	/* initialize tmp variavle */
	text = malloc (sizeof (char) * (f.st_size + 1));

	/* if failed memory allocation */
	if ( text == NULL )
		return NULL;

	while ( (length = fread (tmp, sizeof (char), FILEBUF, fp)) > 0 ) {
		memmove (text + len, tmp, length);
		len += length;
		memset (tmp, 0, sizeof (tmp));
	}
	memset (tmp + len, 0, 1);

	fclose (fp);

	return text;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
