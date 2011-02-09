/** 
 * @file	libstring.c
 * @brief	String API
 */

/* $Id: libstring.c,v 1.26 2011-02-09 15:18:37 oops Exp $ */
#include <oc_common.h>
#include <libstring.h>

char * decode_race (char *domain, char *charset, int debug);
char * encode_race (char *domain, char *charset, int debug);

/*
 * This function must memory free
 * This is binary safe.
 */
size_t addslashes_ex (unsigned char * in, size_t inlen, unsigned char * out) // {{{
{
	/* maximum string length, worst case situation */
	unsigned char *source, *target;
	unsigned char *end;
	size_t outlen;

	if ( in == NULL || inlen < 1 )
		return 0;

	source = in;
	target = out;
	end = source + inlen;

	while (source < end) {
		switch (*source) {
			case 0x00: // Null byte (\0)
				*target++ = 0x5c; // back slash
				*target++ = 0x30; // '0'
				break;
			case 0x27: // single qoute
			case 0x22: // double qoute
			case 0x5c: // back slash
				*target++ = 0x5c;
				// break is missing *intentionally*
			default:
				*target++ = *source;
		}

		source++;
	}

	*target = 0;
	outlen = target - out;

	return outlen;
} // }}}

/**
 * @brief	print olibc version
 * @return	string
 */
OLIBC_API
void olibc_version (void) // {{{
{
	printf ("%s\n", PACKAGE_VERSION);
} // }}}

/**
 * @brief	remove white space on behind and forward on strings.
 * @param	str given strings
 * @return	void
 *
 * trim is remove white space on behind and forward on string.
 * warn !! this function modified original variable !!
 */ 
OLIBC_API
void trim (char *str) // {{{
{
	int len = strlen (str);
	int start = 0, end = 0, i = 0;

	// get end
	OC_DEBUG ("%s\n", "Get end point");
	for ( i=len-1; i>0; i-- ) {
		OC_DEBUG("\t%4d : 0x%x\n", i, str[i]);
		if ( ! isspace (str[i]) ) {
			end = i;
			break;
		}
	}

	if ( end == 0 ) {
		memset (str, 0, 1);
		return;
	}

	// get start
	OC_DEBUG ("%s\n", "Get start point");
	for ( i=0; i<len; i++ ) {
		OC_DEBUG("\t%4d : 0x%x\n", i, str[i]);
		if ( ! isspace (str[i]) ) {
			start = i;
			break;
		}
	}

	if ( end < start ) {
		oc_error ("%s\n", "end point is smaller than start point on trim function");
		exit (FAILURE);
	}

	OC_DEBUG ("start(%d) : end(%d)\n", start, end);
	memmove (str, str + start, end - start + 1);
	memset (str + (end - start + 1), 0, 1);
} // }}}

/**
 * @brief	remove white space on behind and forward on strings.
 * @param[in]	str given strings
 * @param[in]	should_free 0 or 1. set 1, free memory of str argument.
 * @return		point of result
 *
 * trim is remove white space on behind and forward on string.
 * Return point of this function is must freed.
 */ 
OLIBC_API
char * trim_r (char *str, int should_free) // {{{
{
	int len;
	char *buf;

	if ( str == NULL )
		return NULL;

	if ( (len = strlen (str)) == 0 ) {
		if ( should_free )
			ofree (str);
		return NULL;
	}

	oc_strdup (buf, str, len);
	if ( should_free )
		ofree (str);

	trim (buf);

	return buf;
} // }}}

/**
 * @brief	Quote string with slashes
 * @param	in given string for qouting
 * @param	should_free hould_free 0 or 1. set 1, free memory of in argument.
 * @return	point of result
 * @seealso	addslashes_r
 *
 * Returns a string with backslashes before characters that need
 * to be quoted in database queries etc. These characters are single
 * quote ('), double quote ("), backslash (\).
 *
 * This is not binary safe. If you want to binary safe, use addslashes_r.
 */
OLIBC_API
char * addslashes (char * in, int should_free) // {{{
{
	size_t outlen, len;
	unsigned char * out;

	if ( in == NULL )
		return NULL;

	len = strlen (in);
	oc_malloc_r (out, sizeof (char) * (len * 2 + 1), NULL);
	outlen = addslashes_ex ((unsigned char *) in, len, out);

	if ( should_free )
		ofree (in);

	if ( ! outlen ) {
		ofree (out);
		return NULL;
	}

	/*
	// if you want to save memory
	if ( outlen < (len * 2) )
		oc_realloc_r (out, outlen + 1, NULL);
		out = (char *) realloc(out, outlen + 1);
	*/

	return (char *) out;
} // }}}

/**
 * @brief	Quote string with slashes
 * @param	in given binary data for qouting
 * @param	inlen length of given binary data
 * @param	out converted binary data
 * @param	should_free 0 or 1. set 1, free memory of in argument.
 * @return	length of converted data
 * @seealso	addslashes
 *
 * convert binary data with * backslashes before characters that need
 * to be quoted in database queries etc. These characters are single
 * quote ('), double quote ("), backslash (\) and Null byte (\0).
 *
 * This is binary safe.
 */
OLIBC_API
int addslashes_r (unsigned char * in, size_t inlen, unsigned char ** out, int should_free) // {{{
{
	size_t outlen;

	if ( in == NULL )
		return 0;

	oc_malloc (*out, sizeof (char) * (inlen * 2 + 1));
	if ( out == NULL )
		return 0;

	outlen = addslashes_ex (in, inlen, *out);

	if ( should_free )
		ofree (in);

	if ( ! outlen )
		ofree (*out);

	return outlen;
} // }}}

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
		bufno = char2int (buf[i]);

		if ( bufno == 0 ) {
			x *= 10;
			continue;
		}

		if ( bufno > 0 ) {
			res += bufno * x;
			x *= 10;
		}
	}

	if (minus) res *= -1;
	ofree (buf);

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
		if ( minus ) l = str2long (buf + 1);
		else l = str2long (buf);
	} else {
		dotlen = strlen (dot);

		if ( dotlen > 7 ) {
			fprintf (stderr, "ERROR: float length is too long. max 6");
			exit (FAILURE);
		}

		for ( i = len - dotlen + 1; i < len; i++ ) {
			bufno = char2int (buf[i]);

			if ( bufno == 0 ) {
				y /= 10;
				continue;
			}

			if ( bufno > 0 ) {
				f += bufno * y;
				y /= 10;
			}
		}

		len = len - dotlen;
		for ( i = len; i > -1; i-- ) {
			bufno = char2int (buf[i]);
			if ( bufno == 0 ) {
				x *= 10;
				continue;
			}

			if ( bufno > 0 ) {
				l += bufno * x;
				x *= 10;
			}
		}
	}

	res = l + f;
	if (minus) res *= -1;
	ofree (buf);

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

	memset (result, 0, 256);
	memset (sunit, 0, 6);
	memset (re_unit, 0, 3);

	if (unit != 1) {
		strcpy (sunit, "Bytes");
		ssunit = 'B';
	} else {
		strcpy (sunit, "Bits");
		ssunit = 'b';
	}

	BYTE_C = (char *) numberFormat (size, 0, '.', ',', 0);
	strcpy (bytes, BYTE_C);
	ofree(BYTE_C);

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
	 oc_malloc_r (tmpbuf, 1024, NULL);

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

	 oc_malloc (resbuf, reslen + 1);
	 if ( resbuf == NULL ) {
		 ofree (tmpbuf);
		 return NULL;
	 }

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

	 ofree(tmpbuf);

	 if (print) {
		 printf ("%s", resbuf);
		 ofree (resbuf);
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

char * bin2hex (char *str) {
	int i, j, len;
	char tmp[5];
	static char buf[1024] = { 0, };

	len = strlen (str);

	for ( i = 0, j = 0; i < len; i += 4, j++ ) {
		memset (tmp, 0, 5);
		strncpy ( tmp, str + i, 4 );
		if ( strlen (tmp) < 4 ) continue;

		memset ( buf + j, _bin2hex (tmp), 1);
	}

	return buf;
}

char _bin2hex (char *s) {
	if ( ! strcmp ( s, "0000" ) ) return '0';
	else if ( ! strcmp ( s, "0001" ) ) return '1';
	else if ( ! strcmp ( s, "0010" ) ) return '2';
	else if ( ! strcmp ( s, "0011" ) ) return '3';
	else if ( ! strcmp ( s, "0100" ) ) return '4';
	else if ( ! strcmp ( s, "0101" ) ) return '5';
	else if ( ! strcmp ( s, "0110" ) ) return '6';
	else if ( ! strcmp ( s, "0111" ) ) return '7';
	else if ( ! strcmp ( s, "1000" ) ) return '8';
	else if ( ! strcmp ( s, "1001" ) ) return '9';
	else if ( ! strcmp ( s, "1010" ) ) return 'a';
	else if ( ! strcmp ( s, "1011" ) ) return 'b';
	else if ( ! strcmp ( s, "1100" ) ) return 'c';
	else if ( ! strcmp ( s, "1101" ) ) return 'd';
	else if ( ! strcmp ( s, "1110" ) ) return 'e';
	else if ( ! strcmp ( s, "1111" ) ) return 'f';
	else return 0;
}

char * hex2bin (char *str) {
	char *data;
	int len = strlen (str), i, j;

	// if failed memory allocate, return NULL
	oc_malloc_r (data, sizeof (char) * (len + 4), NULL);
	memset (data, 0, sizeof (char) * (len + 4));

	for ( i = 0, j = 0; i < len; i++ ) {
		memcpy (data + j, _hex2bin (str[i]), 4);
		j += 4;
	}

	return data;
}

char * _hex2bin (char c) {
	static char h2b[5];

	memset (h2b, 0, 5);

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

int is_ksc5601 (unsigned c1, unsigned c2) {
	unsigned char *c = (unsigned char *) ((c1 << 8) | c2);
	//printf ("0x%x : 0x%x => 0x%x, %c%c\n", c1, c2, c, c);

	if ( ! (c1 & 0x80) )
		return FAILURE;

	if ( ((c1 >= 0xa1 && c1 <= 0xa4) || (c1 >= 0xa8 && c1 <= 0xa9) ||
		  (c1 >= 0xb0 && c1 <= 0xc8) || (c1 >= 0xca && c1 <= 0xfd))
		 && (c2 >= 0xa1 && c2 <= 0xfe) )
		return SUCCESS;

	if ( (int) c >= 0xa1a2 && (int) c <= 0xa1fe )
		return SUCCESS;
	if ( (int) c >= 0xa5a1 && (int) c <= 0xa5f8 )
		return SUCCESS;
	if ( (int) c >= 0xa6a1 && (int) c <= 0xa6e4 )
		return SUCCESS;
	if ( (int) c >= 0xa7a1 && (int) c <= 0xa7ef )
		return SUCCESS;
	if ( (int) c >= 0xaaa1 && (int) c <= 0xaaf3 ) // hirakana
		return SUCCESS;
	if ( (int) c >= 0xaba1 && (int) c <= 0xabf6 ) // katakana
		return SUCCESS;
	if ( (int) c >= 0xaca1 && (int) c <= 0xacf1 )
		return SUCCESS;

	return FAILURE;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
