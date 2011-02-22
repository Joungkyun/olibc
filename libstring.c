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
 * @file	libstring.c
 * @brief	String API
 *
 * This file includes string apis for a convenient string handling.
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-02-22 18:11:46 $
 * $Revision: 1.65 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */

/* $Id: libstring.c,v 1.65 2011-02-22 18:11:46 oops Exp $ */
#include <oc_common.h>
#include <libstring.h>

#ifdef HAVE_ICONV_H
#include <iconv.h>
#endif

/**
 * @brief	print olibc version
 * @return	void
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
void trim (char * str) // {{{
{
	int len;
	int start = 0, end = 0, i = 0;

	if ( str == NULL )
		return;

	len = strlen (str);
	if ( len < 1 )
		return;

	// get end
	for ( i=len-1; i>=0; i-- ) {
		OC_DEBUG("\t%4d : 0x%x\n", i, str[i]);
		if ( ! isspace (str[i]) ) {
			end = i;
			break;
		}
	}
	OC_DEBUG ("Get end point: %d\n", end);

	if ( end < 0 ) {
		memset (str, 0, 1);
		return;
	}

	// get start
	OC_DEBUG ("Get start point\n");
	for ( i=0; i<len; i++ ) {
		OC_DEBUG("\t%4d : 0x%x\n", i, str[i]);
		if ( ! isspace (str[i]) ) {
			start = i;
			break;
		}
	}
	OC_DEBUG ("Get start point: %d\n", start);

	// If case, exists only white space
	if ( ! start && ! end && isspace (str[start]) ) {
		memset (str, 0, 1);
		return;
	}

	if ( end < start ) {
		oc_error ("end point is smaller than start point on trim function\n");
		exit (1);
	}

	OC_DEBUG ("start(%d) : end(%d)\n", start, end);
	memmove (str, str + start, end - start + 1);
	memset (str + (end - start + 1), 0, 1);
} // }}}

/**
 * @brief	remove white space on behind and forward on strings.
 * @param[in]	str given strings
 * @param[in]	should_free bool / set true, free memory of str argument.
 * @return		point of result
 *
 * trim is remove white space on behind and forward on string.
 * Return point of this function is must freed.
 */ 
OLIBC_API
char * trim_r (char * str, bool should_free) // {{{
{
	int len;
	char * buf;

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
 * @param	in given binary data for qouting
 * @param	inlen length of given binary data
 * @param	out converted binary data
 * @param	outlen length of converted data
 * @return	length of converted data
 * @sa		addslashes
 *
 * convert binary data with * backslashes before characters that need
 * to be quoted in database queries etc. These characters are single
 * quote ('), double quote ("), backslash (\) and Null byte (\0).
 *
 * This is binary safe.
 */
OLIBC_API
int addslashes_r (UChar * in, size_t inlen, UChar ** out, size_t * outlen) // {{{
{
	/* maximum string length, worst case situation */
	UChar * source, * target;
	UChar * end;

	if ( in == NULL || inlen < 1 )
		return 0;

	oc_malloc_r (*out, sizeof (char) * (inlen * 2 + 1), 0);

	source = in;
	target = *out;
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
	*outlen = target - (*out);
	/*
	// if you want to save memory
	if ( *outlen < (inlen * 2) )
		oc_realloc_r (*out, sizeof (char) * ((*outlen) + 1), NULL);
	*/

	return 1;
} // }}}

/**
 * @brief	Quote string with slashes
 * @param	in given string for qouting
 * @param	should_free bool / set true, free memory of in argument.
 * @return	point of result
 * @sa		addslashes_r
 *
 * Returns a string with backslashes before characters that need
 * to be quoted in database queries etc. These characters are single
 * quote ('), double quote ("), backslash (\).
 *
 * This is not binary safe. If you want to binary safe, use addslashes_r.
 */
OLIBC_API
char * addslashes (char * in, bool should_free) // {{{
{
	size_t outlen;
	UChar * out;

	if ( in == NULL )
		return NULL;

	if ( addslashes_r ((UChar *) in, strlen (in), &out, &outlen) == 0 )
		return NULL;

	if ( should_free )
		ofree (in);

	return (char *) out;
} // }}}

/**
 * @brief	convert to 64bit integer from string
 * @param[in]	src numeric string
 * @return	64bit long value
 */
OLIBC_API
Long64 str2long (CChar * src) // {{{
{
#ifdef HAVE_STRTOULL
	OC_DEBUG ("USE strtoll\n");
	return strtoll (src, NULL, 10);
#else
	int bufno = 0, i;
	UInt len;
	bool minus = false;
	Long64 x = 1, res = 0;
	char * buf;

	/* removed blank charactor */
	oc_strdup_r (buf, src, 0);
	trim (buf);
	len = strlen (buf);

	OC_DEBUG ("INPUT Trimed String: %s (%d byte)\n", buf, len);

	/* minus value check */
	if ( buf[0] == '-' ) minus = true;

	len--;
	for ( i=len; i>-1; i-- ) {
		bufno = char2int (buf[i]);
		OC_DEBUG ("    %2d: %d\n", i, bufno);

		if ( bufno == 0 ) {
			x *= 10;
			//x = (x << 3) + (x << 1);
			continue;
		}

		if ( bufno > 0 ) {
			res += (bufno * x);
			x *= 10;
		}
		OC_DEBUG ("    => Current : %lld   \n", res);
	}

	if (minus) res *= -1;
	ofree (buf);

	return res;
#endif
} // }}}

/**
 * @brief	convert type casting to double from string
 * @param[in]	src numeric string
 * @return	double value
 */
OLIBC_API
long double str2double (CChar * src) { // {{{
	char * dot;
	char * decimal_t, * fraction_t;
	ULong64 decimal, fraction;
	float fraction_f;
	bool minus = false;
	long double buf;

	if ( src == NULL )
		return 0;

	if ( strlen (src) == 0 )
		return 0;

	if ( src[0] == '-' )
		minus = true;

	oc_strdup_r (decimal_t, src + minus, 0);

	if ( (dot = (char *) strrchr (decimal_t, '.')) == NULL ) {
		oc_strdup (fraction_t, "", 0);
	} else {
		oc_strdup (fraction_t, dot + 1, strlen (dot) - 1);
		*dot = 0;
	}

	if ( fraction_t == NULL ) {
		ofree (decimal_t);
		return 0;
	}

	OC_DEBUG ("Decimal String  => %s\n", decimal_t);
	OC_DEBUG ("Fraction String => %s\n", fraction_t);

	decimal = str2long (decimal_t);
	fraction = str2long (fraction_t);

	ofree (decimal_t);
	ofree (fraction_t);

	OC_DEBUG ("Decimal Integer  => %lld\n", decimal);
	OC_DEBUG ("Fraction Integer => %lld\n", fraction);

	fraction_f = fraction;

	while ( fraction_f > 1 ) {
		fraction_f /= 10;
		//OC_DEBUG ("----> %f\n", fraction_f);
	}

	OC_DEBUG ("Fraction floating => %f\n", fraction_f);

	buf = (long double) decimal + fraction_f;
	if ( minus )
		buf *= -1;

	return buf;
} // }}}

/**
 * @brief	convert casting type to int from char
 * @param	c	numeric charactor (character 0 - 9)
 * @return	int value (On fail, return -1)
 *
 * If given string is our of range between ascii 48 and 57,
 * return -1.
 */
OLIBC_API
int char2int (CChar c) // {{{
{
	if (c > 47 && c < 58)
		return c - 48;

	return -1;
} // }}}

/**
 * @brief	check a ascii value of a charactor whether is between 48 and 57
 * @param	c a charactor for checking
 * @return	bool
 *
 * The check_int() function check whether a given charactor has value
 * between 48 and 57.
 */
OLIBC_API
int check_int (char c) // {{{
{
	if ( c < 48 || c > 57 ) return 0;
	return 1;
} // }}}

/**
 * @brief	Set ansi color
 * @param	stream stdin, stdout or stderr, and so on.
 * @param	color color constants
 * @param	noansi bool value. If not 0, this function don't opperate.
 * @return	void
 *
 * Set ansi color of printed string on a file stream. If you want to stop
 * ansi mode, recall this function with ENDANSI constant.
 *
 * Color constants:
 *     ENDANSI   0
 *     GRAY      1
 *     BGRAY     2
 *     RED       3
 *     BRED      4
 *     GREEN     5
 *     BGREEN    6
 *     YELLOW    7
 *     BYELLOW   8
 *     BLUE      9
 *     BBLUE    10
 *     MAGENTA  11
 *     BMAGENTA 12
 *     CYAN     13
 *     BCYAN    14
 *     WHITE    15
 *     BWHITE   16
 */
OLIBC_API
void setansi (FILE * stream, int color, bool noansi) // {{{
{
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
} // }}}

/**
 * @brief	convert number to human readable
 * @param	size double number
 * @param	sub bool. set true, return with original size. ex, 'convert size (orignal size)'
 * @param	unit bool. set true, caculation with Byte, else with Bit.
 * @return	formatted string by human read.
 *
 * The human_size_r() funtion convert to unit for human readable.
 * The return value support decimal porint under 2 digits.
 *
 * The return value is must freed.
 *
 * This function is thread safe.
 */
OLIBC_API
char * human_size_r (ULong64 size, bool sub, bool unit) // {{{
{
	char units[] = { 0, 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y' };
	UInt i = 0, dvd = 1024;
	ULong64 osize = size, frac = 1;
	char * buf;
	char singular[2] = { 0, };

	oc_safe_cpy (singular, (size > 2) ? "s" : "", 2);
	oc_malloc_r (buf, sizeof (char) * 64, NULL);
	memset (buf, 0, 64);

	if ( ! unit )
		dvd = 1000;

	while ( size > dvd ) {
		frac = size;

		if ( dvd == 1024 )
			size >>= 10;
		else
			size /= 1000;

		OC_DEBUG ("INT  PART: %llu / %d = %lld\n", frac, dvd, size);
		i++;
	}

	// get rest and under decimal point with integer (2digits)
	if ( dvd == 1000 )
		frac = ((frac + (~(size * 1000) + 1)) * 100) / 1000;
	else
		frac = (frac & 0x03ff) * 100 >> 10;

	OC_DEBUG ("FRAC PART: %lld\n", frac);

	if ( sub ) {
		char * BYTE_C;
		BYTE_C = (char *) numberFormat (osize, 0, '.', ',', 0);
		sprintf (
			buf, "%llu.%llu %c%c (%s B%s%s)",
			size, frac, units[i], unit ? 'b' : 'B',
			BYTE_C, unit ? "it" : "yte", singular
		);
		ofree (BYTE_C);
	} else
		sprintf (buf, "%llu.%llu %c%c", size, frac, units[i], unit ? 'b' : 'B');

	return buf;
} // }}}

/**
 * @brief	convert number to human readable
 * @param	size double number
 * @param	sub bool. set true, return with original size. ex, 'convert size (orignal size)'
 * @param	unit bool. set true, caculation with Byte, else with Bit.
 * @return	formatted string by human read.
 *
 * The human_size_r() funtion convert to unit for human readable.
 * The return value support decimal porint under 2 digits.
 *
 * This function is not thread safe, so use human_size_r function for
 * thread safe.
 */
OLIBC_API
char * human_size (double size, int sub, int unit) // {{{
{
	static char buf[1024] = { 0, };
	char * tmp;

	tmp = human_size_r (size, sub, unit);
	oc_safe_cpy (buf, tmp, 1024);
	ofree (tmp);

	return buf;
} // }}}

/**
 * @brief	Format a number with grouped thousands
 * @param	d The number being formatted.
 * @param	dec Sets the number of decimal points.
 * @param	dec_point Sets the separator for the decimal point.
 * @param	thousand_sep Sets the thousands separator.
 * @param	print bool. if set true, don't free return value.
 * @return	formatted string
 *
 * This function is formatted a number with grouped thousands.
 * See also http://php.net/manual/en/function.number-format.php
 *
 * This function is written with PHP (http://php.net).
 */
OLIBC_API
char * numberFormat (double d, int dec, char dec_point, char thousand_sep, bool print) // {{{
{
	char * tmpbuf, * resbuf;
	char * src, * tgt;  /* source, target */
	int tmplen, reslen = 0;
	int count = 0;
	int is_negative = 0;

	if (d < 0) {
		is_negative = 1;
		d = -d;
	}
	dec = MAX (0, dec);
	oc_malloc_r (tmpbuf, 1024, NULL);

	tmplen = sprintf (tmpbuf, "%.*f", dec, d);
	if ( ! isdigit ((int) tmpbuf[0]) )
		return tmpbuf;

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
	}
	return resbuf;
} // }}}

/*
 * Case sensitive
 */

/**
 * @brief	convert strings to lower case
 * @param	str strings
 * @return	void
 *
 * The strtolower() function converts strings to lower case.
 * The given argument of strtolower is changed.
 */
OLIBC_API
void strtolower (char * str) // {{{
{
	int len;

	if ( str == NULL )
		return;

	if ( (len = strlen (str)) < 1 )
		return;

	while ( *str != 0 ) {
		if ( *str >= 0x41 && *str <= 0x5a )
			*str += 32;
		str++;
	}
} // }}}

/**
 * @brief	convert strings to upper case
 * @param	str strings
 * @return	void
 *
 * The strtoupper() function converts strings to upper case.
 * The given argument of strtoupper is changed.
 */
OLIBC_API
void strtoupper (char * str) // {{{
{
	int len;

	if ( str == NULL )
		return;

	if ( (len = strlen (str)) < 1 )
		return;

	while ( *str != 0 ) {
		if ( *str >= 0x61 && *str <= 0x7a )
			*str += -32;
		str++;
	}
} // }}}

/*
 * Heximal <-> Binary
 */

/**
 * @brief	convert binary string to hex charactor
 * @param	src 4byte binary string
 * @return	hexical charactor [0-9a-f?]
 *
 * The _bin2hex() function convert 4byte binary string to
 * hexcical charactor.
 */
char _bin2hex (CChar * src) // {{{
{
	if ( ! strcmp ( src, "0000" ) ) return '0';
	else if ( ! strcmp (src, "0001") ) return '1';
	else if ( ! strcmp (src, "0010") ) return '2';
	else if ( ! strcmp (src, "0011") ) return '3';
	else if ( ! strcmp (src, "0100") ) return '4';
	else if ( ! strcmp (src, "0101") ) return '5';
	else if ( ! strcmp (src, "0110") ) return '6';
	else if ( ! strcmp (src, "0111") ) return '7';
	else if ( ! strcmp (src, "1000") ) return '8';
	else if ( ! strcmp (src, "1001") ) return '9';
	else if ( ! strcmp (src, "1010") ) return 'a';
	else if ( ! strcmp (src, "1011") ) return 'b';
	else if ( ! strcmp (src, "1100") ) return 'c';
	else if ( ! strcmp (src, "1101") ) return 'd';
	else if ( ! strcmp (src, "1110") ) return 'e';
	else if ( ! strcmp (src, "1111") ) return 'f';
	else return '?';
} // }}}

/**
 * @brief	convert binary strings to hex strings
 * @param[in]	src binary string
 * @param[out]	dst hexical string
 * @return	length of dst
 *
 * The bin2hex_r() function converts binary string to hexical
 * string.
 *
 * This function is thread safe, and must free return value.
 */
OLIBC_API
ULong32 bin2hex_r (CChar * src, char ** dst) // {{{
{
	int i, j, len;
	char buf[5];

	if ( src == NULL )
		return 0;

	if ( (len = strlen (src)) < 4 )
		return 0;

	oc_malloc_r (*dst, sizeof (char) * (len / 4 + 1), 0);

	i = j = 0;
	for ( ; i<len; i+=4, j++ ) {
		memset (buf, 0, 5);
		strncpy (buf, src + i, 4);
		memset (*dst + j, _bin2hex (buf), 1);
	}

	return j;
} // }}}

/**
 * @brief	convert binary strings to hex strings
 * @param	src binary string
 * @return	static string
 *
 * The bin2hex() function converts binary string to hexical
 * string.
 *
 * The return value used static memory, so this function is
 * not thread safe. If you want thread safe, use bin2hex_r.
 *
 * The argument length of bin2hex() is smaller than (1024 * 4).
 */
OLIBC_API
char * bin2hex (CChar * src) // {{{
{
	static char dst[1024] = { 0, };
	ULong32 len;
	char * buf;

	len = bin2hex_r (src, &buf);

	if ( ! len )
		return dst;

	strncpy (dst, buf, len);
	ofree (buf);
	return dst;
} // }}}

/**
 * @brief	convert hexical charactor to binary string
 * @param	c	hexical charactor for converting
 * @return	binary string or '...' on out of range.
 */
OLIBC_API
char * _hex2bin (CChar c) { // {{{
	if((c >= 0x61 && c <= 0x7a) || (c >= 0x41 && c <= 0x5a)) {
		switch (c) {
			case 'a' : return "1010"; break;
			case 'b' : return "1011"; break;
			case 'c' : return "1100"; break;
			case 'd' : return "1101"; break;
			case 'e' : return "1110"; break;
			case 'f' : return "1111"; break;
			case 'A' : return "1010"; break;
			case 'B' : return "1011"; break;
			case 'C' : return "1100"; break;
			case 'D' : return "1101"; break;
			case 'E' : return "1110"; break;
			case 'F' : return "1111"; break;
		}
	} else {
		switch (c) {
			case '0' : return "0000"; break;
			case '1' : return "0001"; break;
			case '2' : return "0010"; break;
			case '3' : return "0011"; break;
			case '4' : return "0100"; break;
			case '5' : return "0101"; break;
			case '6' : return "0110"; break;
			case '7' : return "0111"; break;
			case '8' : return "1000"; break;
			case '9' : return "1001"; break;
		}
	}

	return "....";
} // }}}

/**
 * @brief	convert hexical string to binary string
 * @param	src hexical string for converting
 * @return	binary string or NULL
 */
OLIBC_API
char * hex2bin (CChar * src) // {{{
{
	char * data;
	UInt len, alloc, i, j;

	if ( src == NULL )
		return NULL;

	if ( strlen (src) == 0 )
		return NULL;

	len = strlen (src);
	alloc = (len * 4) + 1;

	// if failed memory allocate, return NULL
	oc_malloc_r (data, sizeof (char) * alloc, NULL);
	memset (data, 0, sizeof (char) * alloc);

	for ( i=0, j=0; i<len; i++ ) {
		memcpy (data + j, _hex2bin (src[i]), 4);
		j += 4;
	}

	return data;
} // }}}

/*
 * Decimal <-> Binary
 */

/**
 * @brief	convert binary string to 32bit integer
 * @param	src	binary string
 * @return	32bit integer
 */
OLIBC_API
UInt bin2dec (CChar * src) // {{{
{
	UInt len, i, ret = 0;
	char var;

	len = strlen (src);

	for ( i=0 ; i<len ; i++ ) {
		var = (src[i] == 0x30) ? 0 : 1;
		ret += var << (len - 1 - i);
	}

	return ret;
} // }}}

/**
 * @brief	convert binary string to 64bit integer
 * @param	src	binary string
 * @return	64bit integer
 */
OLIBC_API
ULong64 bin2long (CChar * src) // {{{
{
	ULong64 ret = 0;
	UInt var, len;
	char high[33] = { 0, };
	char low[33]  = { 0, };
	char * buf;
	Bit64 v = { 0, 0 };
	bool over32 = false, over32_high = false;

	if ( (len = strlen (src)) > 32 ) {
		UInt hlen = len - 32;
		over32 = true;
		memmove (high, src, hlen);
		memmove (low,  src + hlen, 32);

		OC_DEBUG ("SRC : %32s (length: %d)\n", src, len);
		OC_DEBUG ("HIGH: %32s (length: %d)\n", high, hlen);
		OC_DEBUG ("LOW : %32s (length: %d)\n", low, strlen (low));

		buf = high;
		len -= 32;
	} else {
		buf = (char *) src;
	}

b2l_low:
	var = bin2dec (buf);

	if ( over32 ) {
		if ( over32_high == false ) {
			over32_high = true;

			v.high = (ULong32) var;
			buf = low;
			goto b2l_low;
		}

		v.low = (ULong32) var;
		OC_DEBUG ("HIGH: %lu\n", v.high);
		OC_DEBUG ("LOW : %lu\n", v.low);
		ret = combined64_high_low (v);
	}

	return ret;
} // }}}


/* dec to binary template fucntion */
UInt Forebyte2bin (Long32 src, char ** dst, bool complete) // {{{
{
	ULong32 mask = 2147483648UL;
	Long32 m;
	UInt len;

	oc_malloc_r (*dst, sizeof (char) * 33, 0);
	memset (*dst, 0, 32);

	OC_DEBUG ("Buf No: %ld\n", src);

	len = 0;
	while ( mask > 0 ) {
		m = (Long32) src & (Long32) mask;

		if ( m ) {
			memset ((*dst) + len, 49, 1);
			len++;
		} else {
			if ( complete || len ) {
				memset ((*dst) + len, 48, 1);
				len++;
			}
		}

		mask = mask >> 1;
	}
	memset ((*dst) + len, 0, 0);

	return len;
} // }}}

/**
 * @brief	convert 64bit integer to binary string
 * @param[in]	dec 64bit ingeter
 * @param[out]	dst binary string
 * @return	length of binary string
 *
 * The long2bin() function convert 64bit integer to
 * binary string.
 *
 * If return value is not 0, you must free dst.
 */
OLIBC_API
UInt long2bin (Long64 dec, char ** dst) // {{{
{
	ULong64 tmp;
	UInt len, buflen;
	Bit64 v;
	char * buf;
	bool over32 = false, full_print = false;

	if ( ! dec )
		return 0;

	if ( dec > 4294967295UL ) // 4byte ULONG_MAX
		over32 = true;

	len = buflen = 0;
	oc_malloc_r (*dst, sizeof (char) * 65, 0);

	v = devided64_high_low (dec);

	tmp = over32 ? v.high : v.low;

lowbit:
	if ( (buflen = Forebyte2bin (tmp, &buf, full_print)) == 0 )
		return 0;

	OC_DEBUG ("res -> %s (%d)\n", buf, buflen);

	memcpy (*dst + len, buf, buflen);
	len += buflen;
	ofree (buf);

	if ( over32 == true ) {
		over32 = false;
		full_print = true;
		tmp = (Long64) v.low;
		goto lowbit;
	}

	return len;
} // }}}

/**
 * @brief	convert decimal string to binary
 * @param[in]	src deciaml string
 * @param[out]	dst binary string
 * @return	length of binary string
 *
 * The dec2bin() function convert decimal string to
 * binary string. This supports 64bit.
 *
 * If return value is not 0, you must free dst.
 */
OLIBC_API
UInt dec2bin (CChar * src, char ** dst) // {{{
{
	Long64 dec;
	if ( src == NULL )
		return 0;

	if ( strlen (src) == 0 )
		return 0;

#ifdef HAVE_STRTOULL
	dec = strtoll (src, NULL, 10);
#else
	dec = (Long64) str2long (src);
#endif

	return long2bin (dec, dst);
} // }}}

/**
 * @brief		check 2byte whether is ksc5601 or not.
 * @param[in]	c1 first byte of hangul
 * @param[in]	c2 second byte of hangul
 * @return		bool
 */
OLIBC_API
bool is_ksc5601 (UInt c1, UInt c2) // {{{
{
	UChar * c = (UChar *) ((c1 << 8) | c2);
	OC_DEBUG ("0x%x : 0x%x => 0x%x, %c%c\n", c1, c2, (int) c, (int) c1, (int) c2);

	if ( ! (c1 & 0x80) )
		return false;

	if ( ((c1 >= 0xa1 && c1 <= 0xa4) || (c1 >= 0xa8 && c1 <= 0xa9) ||
		  (c1 >= 0xb0 && c1 <= 0xc8) || (c1 >= 0xca && c1 <= 0xfd))
		 && (c2 >= 0xa1 && c2 <= 0xfe) )
		return true;

	if ( (int) c >= 0xa1a2 && (int) c <= 0xa1fe )
		return true;
	if ( (int) c >= 0xa5a1 && (int) c <= 0xa5f8 )
		return true;
	if ( (int) c >= 0xa6a1 && (int) c <= 0xa6e4 )
		return true;
	if ( (int) c >= 0xa7a1 && (int) c <= 0xa7ef )
		return true;
	if ( (int) c >= 0xaaa1 && (int) c <= 0xaaf3 ) // hirakana
		return true;
	if ( (int) c >= 0xaba1 && (int) c <= 0xabf6 ) // katakana
		return true;
	if ( (int) c >= 0xaca1 && (int) c <= 0xacf1 )
		return true;

	return false;
} // }}}

bool utf8_underbit_check (UCChar * s, UInt byte)
{
	UInt i;
	for ( i=0; i<byte; i++ ) {
		if ( *(s + i) >> 6 != 0x02 )
			return false;
	}

	return true;
}

/**
 * @brief		check whether is utf8 or not
 * @param		src strings for checking
 * @return		bool
 */
OLIBC_API
bool is_utf8 (UCChar * src) // {{{
{
	size_t len, byte2;
	UCChar * s = src;
	UChar byte2_check = 0;

	len = byte2 = 0;
	byte2_check = false;

	if ( src == NULL )
		return false;

	len = strlen (src);
	if ( len == 0 )
		return true;

	// check utf8 bom code
	if ( s[0] == 0xef && s[1] == 0xbb && s[2] == 0xbf )
		return true;

again:
	while ( *s && ! (*s & 0x80) )
		s++;

	if ( ! *s ) {
		if ( byte2 ) return byte2;
		return false;
	}

	// binary of utf8 first byte is must start 11
	if ( (*s >> 6) != 0x03 )
		return false;

	// 2byte utf8 check
	// EUC-KR "Jeong" has 2byte utf-8 format. (11000001 10100100)
	// so, check next character
	if ( (*s >> 5 ) == 0x06 ) {
		byte2 = utf8_underbit_check (s + 1, 1);

		if ( ! byte2_check && byte2 == true ) {
			byte2_check = *s;
			s += 2;
			goto again;
		} else if ( byte2_check && byte2 == true && byte2_check == *s ) {
			/* 2th check is true and previous charactors is same current charactors, retry */
			byte2_check = *s;
			s += 2;
			goto again;
		}

		return byte2;
	}

	// 3byte utf8 check
	if ( (*s >> 4 ) == 0x0e )
		return utf8_underbit_check (s + 1, 2);

	// 4byte utf8 check
	if ( (*s >> 3 ) == 0x1e )
		return utf8_underbit_check (s + 1, 3);

	// 5byte utf8 check
	if ( (*s >> 2 ) == 0x3e )
		return utf8_underbit_check (s + 1, 4);

	// 6byte utf8 check
	if ( (*s >> 1 ) == 0x7e )
		return utf8_underbit_check (s + 1, 5);

	return false;
} // }}}

size_t charset_conv_outplen (CChar * charset, size_t srclen)
{
	if ( ! strncmp (charset, "utf", 3) || ! strncmp (charset, "UTF", 3) )
		return srclen * 2;
	else if ( ! strncmp (charset, "ucs", 3) || ! strncmp (charset, "UCS", 3) )
		return srclen * 2;

	return srclen;
}

/**
 * @brief	convert character set
 * @param	src source string
 * @param	from source character set
 * @param	to destination charactor set
 * @return	string point of converted string
 *
 * The charset_conv function is convert string to to_charset
 * from from_charset.
 *
 * If the return value is not NULL, you must freed memory of
 * return point with free() function
 */
OLIBC_API
char * charset_conv (CChar *src, CChar * from, CChar * to) // {{{
{
#ifdef HAVE_ICONV_H
	iconv_t cd;
	char * ibuf, * ibuf_t;
	char * obuf, * obuf_t;
	size_t err;

	size_t ilen, olen, olen_t;

	if ( src == NULL || from == NULL || to == NULL )
		return NULL;

	if ( strcmp (from, to) == 0 ) {
		oc_strdup_r (obuf, src, NULL);
		return obuf;
	}

	ilen = strlen (src) + 1;
	oc_strdup_r (ibuf, src, NULL);
	ibuf_t = ibuf;

	olen = olen_t = charset_conv_outplen (to, ilen);
	oc_malloc (obuf, sizeof (char) * olen);
	if ( obuf == NULL ) {
		ofree (ibuf);
		return NULL;
	}
	obuf_t = obuf;

	cd = iconv_open (to, from);
	if ( cd == (iconv_t) -1 ) {
		oc_error ("Can not open iconv descriptor\n");
		return NULL;
	}

	OC_DEBUG ("INPUT     : %s\n", from);
	OC_DEBUG ("OUTPUT    : %s\n", to);
conv_retry:
	OC_DEBUG ("SRC string: %s\n", ibuf);
	OC_DEBUG ("DEST Size : %u\n", olen_t);
	err = iconv (cd, &ibuf_t, &ilen, &obuf_t, &olen);

	OC_DEBUG ("RET  SIZE : %d\n", err);
	if ( err == (size_t) -1 ) {
		switch (errno) {
			case EINVAL:
				//oc_error ("Incomplete text, do not report an error\n");
				goto skip_error;
				break;
			case E2BIG:
				OC_DEBUG ("CONV Retry: E2BIG There is not sufficient room at *obuf_t\n");
				olen *= 2;
				olen_t = olen;
				oc_realloc (obuf, sizeof (char) * olen);

				{
					size_t used = obuf_t - obuf;
					obuf_t = obuf + used;
					olen += ((~used) + 1);
				}

				//oc_error ("There is not sufficient room at *obuf_t\n");
				goto conv_retry;
				break;
			case EILSEQ:
				oc_error ("An invalid multibyte sequence has been encountered in the input\n");
				break;
			default:
				oc_error ("ERROR: encoding error with iconv\n");
		}

		ofree (obuf);
		obuf = NULL;
	} else
		*obuf_t = 0;

skip_error:

	ofree (ibuf);
	iconv_close (cd);

	return obuf;
#else
	oc_error ("Don't support iconv in current build\n");
	return NULL;
#endif
} // }}}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
