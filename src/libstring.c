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
 *     * Neither the name of JoungKyun.Kim nor the url of oops.org
 *       nor the names of their contributors may be used to endorse or
 *       promote products derived from this software without specific prior
 *       written permission.
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
 *  This file is part of olibc.
 */

/** 
 * @file	libstring.c
 * @brief	String API
 *
 * This file includes string apis for a convenient string handling.
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date$
 * $Revision$
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */

/* $Id$ */
#include <oc_common.h>
#include <libstring.h>
#include <libarg.h>

#ifdef HAVE_ICONV_H
#include <iconv.h>
#endif

/** @defgroup string_internalfunc String API internal functions of olibc
 * @{
 */

/**
 * @brief	convert 4 or 8 byte decimal string to binary
 * @param[in]	src	The input decimal string
 * @param[out]	dst	Converted binary string
 * @param[in]	complete Bool. Set true filled right blank to 0.
 * @return	The length of dst variable that has converted binary string
 */
static UInt byte2bin (Long64 src, char ** dst, bool complete) // {{{
{
	ULong64	mask;
	Long64	m;
	UInt	len;

	if ( src > INT_MAX || src < INT_MIN )
#ifdef __x86_64__
		mask = LONG_MAX + 1UL;
#else
		mask = LLONG_MAX + 1ULL;
#endif
	else
		mask = INT_MAX + 1UL;

	oc_malloc_r (*dst, sizeof (char) * 65, 0);

	OC_DEBUG ("Buf No: %d\n", src);

	len = 0;
	while ( mask > 0 ) {
		m = (Long64) src & mask;

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
 * @brief	convert binary string to hex character
 * @param	src 4byte binary string
 * @return	hexadecimal character [0-9a-f?]
 *
 * The _bin2hex() function convert 4byte binary string to
 * hexcical character.
 */
static char _bin2hex (CChar * src) // {{{
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
 * @brief	check utf8 rest byte except 1st byte 
 * @param	s The input string thst start 2th byte
 * @param	byte The type of byte
 */
static bool utf8_underbit_check (UCChar * s, UInt byte) // {{{
{
	UInt	i;
	for ( i=0; i<byte; i++ ) {
		if ( *(s + i) >> 6 != 0x02 )
			return false;
	}

	return true;
} // }}}

/**
 * @brief	Predict length of converted string per character set
 * @param	charset	The input character set
 * @param	srclen	The length of source string.
 */
static size_t charset_conv_outplen (CChar * charset, size_t srclen) // {{{
{
	if ( ! strncmp (charset, "utf", 3) || ! strncmp (charset, "UTF", 3) )
		return srclen * 2;
	else if ( ! strncmp (charset, "ucs", 3) || ! strncmp (charset, "UCS", 3) )
		return srclen * 2;

	return srclen;
} // }}}

/**
 * @brief	Convert hexadecimal character to binary string
 * @param	c	hexadecimal character for converting
 * @return	binary string
 * @sa	hex2bin bin2hex
 * @retval	'....' out of range
 */
static char * _hex2bin (CChar c) // {{{
{
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

/** @} */ // end of string_internalfunc group

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
 * @brief	Remove white space on behind and forward on the string.
 * @param	str The input string
 * @return	void
 * @sa	trim_r
 *
 * The trim() is remove white space on behind and forward on the input string.
 *
 * @warning
 * The trim() function modified original variable !!
 */ 
OLIBC_API
void trim (char * str) // {{{
{
	int	len,
		start = 0,
		end = 0,
		i = 0;

	if ( str == null )
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
 * @brief	Remove white space on behind and forward on strings.
 * @param[in]	str The input string
 * @param[in]	should_free bool<br />
 *              Set true, Should be freed memory of str argument.
 * @return		Pointer of result string
 * @sa trim
 * @exception DEALLOCATE
 *   When occurs internal error, trim_r() returns null.
 *   If the return string array pointer is not null, the caller should
 *   deallocate this buffer using @e free()
 *
 * The trim_r() is remove white space on behind and forward on the input string.
 */ 
OLIBC_API
char * trim_r (char * str, bool should_free) // {{{
{
	char	* buf;
	int		len;

	if ( str == null )
		return null;

	if ( (len = strlen (str)) == 0 ) {
		if ( should_free )
			ofree (str);
		return null;
	}

	oc_strdup (buf, str, len);
	if ( should_free )
		ofree (str);

	trim (buf);

	return buf;
} // }}}

/**
 * @brief	Quote string with slashes
 * @param[in]	in The input binary data for qouting
 * @param[in]	inlen length of given binary data
 * @param[out]	out converted binary data
 * @param[out]	outlen length of converted data
 * @retval	true Success
 * @retval	false Failure
 * @exception DEALLOCATE
 *   When occurs internal error, 2th argument @e out of addslashes() has
 *   null value. If the @e out argument has not null, the caller should deallocate
 *   this buffer using @e free()
 *
 * convert binary data with backslashes before characters that need
 * to be quoted in database queries etc. These characters are single
 * quote ('), double quote ("), backslash (\) and Null byte (\\0).
 *
 * The null byte is convert to string '\\0'.
 *
 * If @e outlen argument is set null, @e addslashes() api don't count
 * the length of return value.
 *
 * This is binary safe.
 */
OLIBC_API
bool addslashes (CChar * in, size_t inlen, char ** out, size_t * outlen) // {{{
{
	/* maximum string length, worst case situation */
	char	* source,
			* target,
			* end;
	size_t	c;

	if ( in == null || inlen < 1 )
		return false;

	c = get_charcount (in, inlen, "'\"\\\0", 4);
	oc_malloc_r (*out, sizeof (char) * (inlen + c) + 1, false);

	source = (char *) in;
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
	if ( outlen != null )
		*outlen = target - (*out);

	return true;
} // }}}

/**
 * @brief	convert to 64bit integer from string
 * @param[in]	src The input numeric string
 * @return	64bit long value
 *
 * The str2long() function conveted string of numeric type to 64bit integer.
 * If system support strtoll, the str2long() function use strtoll.
 */
OLIBC_API
Long64 str2long (CChar * src) // {{{
{
#ifdef HAVE_STRTOLL
#ifdef __x86_64__
	OC_DEBUG ("USE strtol\n");
	return strtol (src, null, 10);
#else
	OC_DEBUG ("USE strtoll\n");
	return strtoll (src, null, 10);
#endif
#else
	int		bufno = 0,
			i;
	UInt	len;
	bool	minus = false;
	Long64	x = 1,
			res = 0;
	char	* buf;

	/* removed blank character */
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
 * @brief	convert to double from string
 * @param[in]	src numeric string of double type
 * @return	double value
 *
 * The str2double() function conveted string of double numeric type
 * to integer that casted double.
 */
OLIBC_API
double str2double (CChar * src) { // {{{
	char	* dot,
			* decimal_t,
			* fraction_t;
	ULong64	decimal,
			fraction;
	float	fraction_f;
	bool	minus = false;
	double	buf;

	if ( src == null )
		return 0;

	if ( strlen (src) == 0 )
		return 0;

	if ( src[0] == '-' )
		minus = true;

	oc_strdup_r (decimal_t, src + minus, 0);

	if ( (dot = (char *) strrchr (decimal_t, '.')) == null ) {
		oc_strdup (fraction_t, "", 0);
	} else {
		oc_strdup (fraction_t, dot + 1, strlen (dot) - 1);
		*dot = 0;
	}

	if ( fraction_t == null ) {
		ofree (decimal_t);
		return 0;
	}

	OC_DEBUG ("Decimal String  => %s\n", decimal_t);
	OC_DEBUG ("Fraction String => %s\n", fraction_t);

	decimal = str2long (decimal_t);
	fraction = str2long (fraction_t);

	ofree (decimal_t);
	ofree (fraction_t);

#ifdef __x86_64__
	OC_DEBUG ("Decimal Integer  => %ld\n", decimal);
	OC_DEBUG ("Fraction Integer => %ld\n", fraction);
#else
	OC_DEBUG ("Decimal Integer  => %lld\n", decimal);
	OC_DEBUG ("Fraction Integer => %lld\n", fraction);
#endif

	fraction_f = fraction;

	while ( fraction_f > 1 ) {
		fraction_f /= 10;
		//OC_DEBUG ("----> %f\n", fraction_f);
	}

	OC_DEBUG ("Fraction floating => %f\n", fraction_f);

	buf = (double) decimal + fraction_f;
	if ( minus )
		buf *= -1;

	return buf;
} // }}}

/**
 * @brief	convert to integer from char
 * @param	c	The input numeric character (character 0 - 9)
 * @return	integer
 * @code
 *   The input character is out of range between ascii 48 and 57,
 *   the char2int() function returns -1.
 * @endcode
 * @retval	-1 Failure
 * @sa	is_strint str2double str2long
 *
 * The char2int() api convert to integer a character that is
 * ascii between 48 and 57. This api is similar the atoi() api.
 */
OLIBC_API
int char2int (CChar c) // {{{
{
	if (c > 47 && c < 58)
		return c - 48;

	return -1;
} // }}}

/**
 * @brief	check the ascii value of a character whether is between 48 and 57
 * @param	c A character for checking
 * @return	bool
 *
 * The is_strint() function check whether a given character has value
 * between 48 and 57.
 */
OLIBC_API
bool is_strint (char c) // {{{
{
	if ( c < 48 || c > 57 ) return false;
	return true;
} // }}}

/**
 * @brief	Set ansi color
 * @param	stream stdin, stdout or stderr, and so on.
 * @param	color olibc color constants
 * @param	noansi bool. Set true, this function don't operate.
 * @return	void
 *
 * Print magic character of ansi color on the file stream. If you want to stop
 * ansi mode, recall this function with ENDANSI constant.
 *
 * Color constants:
 * @code
 *     OC_ENDANSI   0
 *     OC_GRAY      30
 *     OC_BGRAY     40
 *     OC_RED       31
 *     OC_BRED      41
 *     OC_GREEN     32
 *     OC_BGREEN    42
 *     OC_YELLOW    33
 *     OC_BYELLOW   43
 *     OC_BLUE      34
 *     OC_BBLUE     44
 *     OC_MAGENTA   35
 *     OC_BMAGENTA  45
 *     OC_CYAN      36
 *     OC_BCYAN     46
 *     OC_WHITE     37
 *     OC_BWHITE    47
 * @endcode
 */
OLIBC_API
void setansi (FILE * stream, int color, bool noansi) // {{{
{
	if ( ! noansi ) {
		if ( color == 0 || (color > 29 && color < 38) || (color > 39 && color < 48) )
			fprintf (stream, "[1;%dm", color);
	}
} // }}}

/**
 * @brief	convert number to human readable
 * @param	size The input 64bit integer
 * @param	sub Bool. Set true, return with original size.<br />
 * 			For example, 'convert size (orignal size)'
 * @param	unit Bool. Set true, caculation with Byte, else with Bit.
 * @return	formatted string by human read.
 * @sa	human_size
 * @exception DEALLOCATE
 *   When occurs internal error, human_size() returns null.
 *   If the return string array pointer is not null, the caller should
 *   deallocate this buffer using @e free()
 *
 * The human_size() funtion convert to unit for human readable.
 * The return value support decimal porint under 2 digits.
 *
 * This api don't support negative.
 */
OLIBC_API
char * human_size (ULong64 size, bool sub, bool unit) // {{{
{
	char	units[] = { 0, 'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y' },
			singular[2] = { 0, };
	char	* buf;
	ULong64	osize = size,
			frac = 1;
	UInt	i = 0,
			dvd = 1024;

	oc_safe_cpy (singular, (size > 2) ? "s" : "", 2);
	oc_malloc_r (buf, sizeof (char) * 64, null);

	if ( ! unit )
		dvd = 1000;

	while ( size > dvd ) {
		frac = size;

		if ( dvd == 1024 )
			size >>= 10;
		else
			size /= 1000;

#ifdef __x86_64__
		OC_DEBUG ("INT  PART: %lu / %d = %lu\n", frac, dvd, size);
#else
		OC_DEBUG ("INT  PART: %llu / %d = %llu\n", frac, dvd, size);
#endif
		i++;
	}

	// get rest and under decimal point with integer (2digits)
	if ( dvd == 1000 )
		frac = ((frac + (~(size * 1000) + 1)) * 100) / 1000;
	else
		frac = (frac & 0x03ff) * 100 >> 10;

#ifdef __x86_64__
	OC_DEBUG ("FRAC PART: %ld\n", frac);
#else
	OC_DEBUG ("FRAC PART: %lld\n", frac);
#endif

	if ( sub ) {
		char	* BYTE_C;
		BYTE_C = (char *) numberFormat (osize, 0, '.', ',', 0);
		sprintf (
			buf,
#ifdef __x86_64__
			"%lu.%lu %c%c (%s B%s%s)",
#else
			"%llu.%llu %c%c (%s B%s%s)",
#endif
			size, frac, units[i], unit ? 'b' : 'B',
			BYTE_C, unit ? "it" : "yte", singular
		);
		ofree (BYTE_C);
	} else {
		sprintf (
			buf,
#ifdef __x86_64__
			"%lu.%lu %c%c",
#else
			"%llu.%llu %c%c",
#endif
			size, frac, units[i],
			unit ? 'b' : 'B'
		);
	}

	return buf;
} // }}}

/**
 * @brief	Format a number with grouped thousands
 * @param	num The number being formatted.
 * @param	dec Sets the number of decimal points.
 * @param	dec_point Sets the separator for the decimal point.
 * @param	thousand_sep Sets the thousands separator.
 * @param	print Bool. if set true, returns null.
 * @sa	http://php.net/manual/en/function.number-format.php
 * @return	formatted string
 * @exception DEALLOCATE 
 *   When occurs internal error, numberFormat() returns null.
 *   If the return string array pointer is not null, the caller should
 *   deallocate this buffer using @e free()
 *
 * @e Num will be formatted with @e dec decimals, @e dec_point instead
 * of a dot (".") before the decimals and @e thousands_sep instead of
 * a comma (",") between every group of thousands.
 *
 * This function is written with number_format of PHP (http://php.net).
 */
OLIBC_API
char * numberFormat (double num, int dec, char dec_point, char thousand_sep, bool print) // {{{
{
	char	* tmpbuf,
			* resbuf,
			* src,    // source
			* tgt;    // target
	int		tmplen,
			reslen = 0,
			count = 0,
			is_negative = 0;

	if ( num < 0 ) {
		is_negative = 1;
		num = -num;
	}
	dec = MAX (0, dec);
	oc_malloc_r (tmpbuf, 1024, null);

	tmplen = sprintf (tmpbuf, "%.*f", dec, num);
	if ( ! isdigit ((int) tmpbuf[0]) )
		return tmpbuf;

	if ( dec )
		reslen = dec + 1 + (tmplen - dec - 1) + ((thousand_sep) ? (tmplen - 1 - dec - 1) / 3 : 0);
	else
		reslen = tmplen + ((thousand_sep) ? (tmplen - 1) / 3 : 0);

	if (is_negative)
		reslen++;

	oc_malloc (resbuf, reslen + 1);
	if ( resbuf == null ) {
		ofree (tmpbuf);
		return null;
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
		return null;
	}
	return resbuf;
} // }}}

/*
 * Case sensitive
 */

/**
 * @brief	Make a string lowercase
 * @param	str The input string
 * @return	void
 *
 * @warning
 *   The strtoupper() function changes input variable (@e src)!
 *
 * The strtolower() function converts @e str with all alphabetic
 * characters converted to lowercase.
 */
OLIBC_API
void strtolower (char * str) // {{{
{
	int	len;

	if ( str == null )
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
 * @brief	Make a string uppercase
 * @param	str The input string
 * @return	void
 *
 * @warning
 *   The strtoupper() function changes input variable (@e src)!
 *
 * The strtoupper() function converts @e str with alphabetic
 * characters to uppercase.
 */
OLIBC_API
void strtoupper (char * str) // {{{
{
	int	len;

	if ( str == null )
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
 * @brief	Convert binary data into hexadecimal representation
 * @param[in]	src The input binary string
 * @param[out]	outlen The length of return string
 * @return	The pointer of converted hexadecimal string.
 * @sa	hex2bin
 * @exception DEALLOCATE
 *   When occurs internal error, return null.
 *   If the return value is string array pointer, the caller should
 *   deallocate this buffer using @e free()
 *
 * The bin2hex() function converts binary string to hexadecimal
 * string. The binary string that is out of range is converted
 * '?' character.
 *
 * If @e outlen argument is set null, @e bin2hex() api don't count
 * the length of return value.
 */
OLIBC_API
char * bin2hex (CChar * src, size_t * outlen) // {{{
{
	int		i,
			j,
			len;
	char	buf[5],
			* dst;

	if ( outlen != null )
		*outlen = 0;

	if ( src == null )
		return null;

	if ( (len = strlen (src)) < 4 )
		return null;

	oc_malloc_r (dst, sizeof (char) * (len / 4 + 1), null);

	i = j = 0;
	for ( ; i<len; i+=4, j++ ) {
		memset (buf, 0, 5);
		strncpy (buf, src + i, 4);
		memset (dst + j, _bin2hex (buf), 1);
	}

	if ( outlen != null )
		*outlen = j;

	return dst;
} // }}}

/**
 * @brief	Convert hexadecimal string to binary string
 * @param	src hexadecimal string for converting
 * @param[out]	outlen The length of return string
 * @return	binary string
 * @sa	bin2hex
 * @exception DEALLOCATE
 *   When occurs internal error, hex2bin() returns null.
 *   If the return string array pointer is not null, the caller should
 *   deallocate this buffer using @e free()
 *
 * The @e hex2bin() api convert hexadecimal string to binary string.
 * If @e outlen argument is set null, @e hex2bin() api don't count
 * the length of return value.
 */
OLIBC_API
char * hex2bin (CChar * src, size_t * outlen) // {{{
{
	char	* data;
	UInt	len,
			alloc,
			i,
			j;

	if ( outlen != null )
		*outlen = 0;

	if ( src == null )
		return null;

	if ( strlen (src) == 0 )
		return null;

	len = strlen (src);
	alloc = (len * 4) + 1;

	// if failed memory allocate, return NULL
	oc_malloc_r (data, sizeof (char) * alloc, null);
	memset (data, 0, sizeof (char) * alloc);

	for ( i=0, j=0; i<len; i++ ) {
		memcpy (data + j, _hex2bin (src[i]), 4);
		j += 4;
	}

	if ( outlen != null )
		*outlen = j;

	return data;
} // }}}

/*
 * Decimal <-> Binary
 */

/**
 * @brief	Convert binary string to 32bit integer
 * @param	src	The input binary string
 * @sa	dec2bin bin2long long2bin
 * @return	32bit integer
 */
OLIBC_API
Long32 bin2dec (CChar * src) // {{{
{
	Long32	ret = 0;
	UInt	len, i;
	char	var;

	len = strlen (src);

	for ( i=0 ; i<len ; i++ ) {
		var = (src[i] == 0x30) ? 0 : 1;
		ret += var << (len - 1 - i);
	}

	return ret;
} // }}}

/**
 * @brief	Convert binary string to 64bit integer
 * @param	src	The input binary string
 * @sa	long2bin bin2dec dec2bin
 * @return	64bit integer
 * @warning
 *    The bin2long() function caculate 64bit integer with high and
 *    low bit on 32bit system
 */
OLIBC_API
Long64 bin2long (CChar * src) // {{{
{
#ifdef __x86_64__
	Long64  ret = 0;
	UInt    len, i;
	char    var;

	len = strlen (src);

	for ( i=0 ; i<len ; i++ ) {
		var = (src[i] == 0x30) ? 0 : 1;
		if ( len <= 32 )
			ret += ((Long32) var) << (len - 1 - i);
		else
			ret += ((Long64) var) << (len - 1 - i);
	}

	return ret;
#else
	Long64	ret = 0;
	UInt	var,
			len;
	char	high[33] = { 0, },
			low[33]  = { 0, };
	char	* buf;
	Bit64	v = { 0, 0 };
	bool	over32 = false,
			over32_high = false;

	if ( (len = strlen (src)) > 32 ) {
		UInt	hlen = len - 32;
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
	OC_DEBUG ("BUF: %s (length: %d)\n", buf, strlen (buf));
	var = bin2dec (buf);

	if ( over32 ) {
		if ( over32_high == false ) {
			over32_high = true;

			v.high = var;
			buf = low;
			goto b2l_low;
		}

		v.low = (ULong32) var;
		OC_DEBUG ("HIGH: %d\n", v.high);
		OC_DEBUG ("LOW : %d\n", v.low);
		ret = combined64_high_low (v);
	}

	return over32 ? ret : (Long32) var;
#endif
} // }}}

/**
 * @brief	Convert 64bit integer to binary string
 * @param[in]	dec The input 64bit ingeter
 * @param[out]	outlen The length of return string
 * @return	The pointer of binary string
 * @sa	dec2bin bin2long bin2dec
 * @exception DEALLOCATE
 *   When occurs internal error, the return value of long2bin() has
 *   null. If the return value is not null, the caller should deallocate
 *   this buffer using @e free()
 *
 * @warning
 *    On 32bit system, The long2bin() function caculate 64bit integer
 *    with high and low bit.
 *
 * The long2bin() function convert signed 64bit integer to binary string.
 *
 * If @e outlen argument is set null, @e long2bin() api don't count
 * the length of return value.
 */
OLIBC_API
char * long2bin (Long64 dec, size_t * outlen) // {{{
{
#ifdef __x86_64__
	char	* dst;
	size_t	len = 0;

	if ( outlen != null )
		*outlen = 0;

	if ( (len = byte2bin (dec, &dst, false)) == 0 )
		return null;

	if ( outlen != null )
		*outlen = len;

	return dst;
#else
	Long32	tmp;
	size_t	buflen,
			len = 0;
	Bit64	v;
	char	* buf,
			* dst;
	bool	over32 = false,
			full_print = false;

	if ( outlen != null )
		*outlen = 0;

	if ( ! dec ) {
		oc_strdup (dst, "0", 1);
		if ( outlen != null ) {
			*outlen = 1;
			if ( dst == null )
				*outlen = 0;
		}
		return dst;
	}

	if ( dec > INT_MAX || dec < INT_MIN )
		over32 = true;

	len = buflen = 0;
	oc_malloc_r (dst, sizeof (char) * 65, 0);

	v = devided64_high_low (dec);
	tmp = over32 ? v.high : v.low;

lowbit:
	if ( (buflen = byte2bin ((Long64) tmp, &buf, full_print)) == 0 ) {
		ofree (dst);
		if ( outlen != null )
			*outlen = 0;
		return null;
	}

	OC_DEBUG ("res -> %s (%d)\n", buf, buflen);
	OC_DEBUG ("res -> %s (%d)\n", dst, len);

	memcpy (dst + len, buf, buflen);
	len += buflen;
	ofree (buf);

	if ( over32 == true ) {
		over32 = false;
		full_print = true;
		tmp = v.low;
		goto lowbit;
	}

	if ( outlen != null )
		*outlen = len;

	return dst;
#endif
} // }}}

/**
 * @brief	Convert decimal string to binary
 * @param[in]	src The input deciaml string
 * @param[out]	outlen The length of return string
 * @return	The pointer of binary string
 * @sa	long2bin bin2dec bin2long
 * @exception DEALLOCATE
 *   When occurs internal error, the return value of dec2bin() has
 *   null. If the return value is not null, the caller should deallocate
 *   this buffer using @e free()
 *
 * The dec2bin() function convert decimal string to binary string.
 * This supports signed 64bit integer.
 *
 * If @e outlen argument is set null, @e decbin() api don't count
 * the length of return value.
 */
OLIBC_API
char * dec2bin (CChar * src, size_t * outlen) // {{{
{
	Long64	dec;

	if ( outlen != null )
		*outlen = 0;

	if ( src == null )
		return NULL;

	if ( strlen (src) == 0 )
		return NULL;

#ifdef HAVE_STRTOLL
	OC_DEBUG ("USE strtoll\n");
#ifdef __x86_64__
	dec = strtol (src, null, 10);
#else
	dec = strtoll (src, null, 10);
#endif
#else
	dec = (Long64) str2long (src);
#endif

	return long2bin (dec, outlen);
} // }}}

/**
 * @brief	Check 2byte whether is ksc5601 or not.
 * @param	c1 The first byte of hangul
 * @param	c2 The second byte of hangul
 * @return	bool
 */
OLIBC_API
bool is_ksc5601 (int c1, int c2) // {{{
{
	int c = (c1 << 8) | c2;
	OC_DEBUG ("0x%x : 0x%x => 0x%x, %c%c\n", c1, c2, c, c1, c2);

	if ( ! (c1 & 0x80) )
		return false;

	if ( ((c1 >= 0xa1 && c1 <= 0xa4) || (c1 >= 0xa8 && c1 <= 0xa9) ||
		  (c1 >= 0xb0 && c1 <= 0xc8) || (c1 >= 0xca && c1 <= 0xfd))
		 && (c2 >= 0xa1 && c2 <= 0xfe) )
		return true;

	if ( c >= 0xa1a2 && c <= 0xa1fe )
		return true;
	if ( c >= 0xa5a1 && c <= 0xa5f8 )
		return true;
	if ( c >= 0xa6a1 && c <= 0xa6e4 )
		return true;
	if ( c >= 0xa7a1 && c <= 0xa7ef )
		return true;
	if ( c >= 0xaaa1 && c <= 0xaaf3 ) // hirakana
		return true;
	if ( c >= 0xaba1 && c <= 0xabf6 ) // katakana
		return true;
	if ( c >= 0xaca1 && c <= 0xacf1 )
		return true;

	return false;
} // }}}

/**
 * @brief		Check whether is utf8 or not
 * @param		src The string for checking
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

	if ( src == null )
		return false;

	len = strlen ((CChar *) src);
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
			/* 2th check is true and previous characters is same current characters, retry */
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

/**
 * @brief	Convert character set
 * @param	src The input source string
 * @param	from The input source character set
 * @param	to The destination character set
 * @return	The pointer of converted string
 * @exception DEALLOCATE
 *   When occurs internal error, charset_conv() returns null.
 *   If the return string array pointer is not null, the caller should
 *   deallocate this buffer using @e free()
 *
 * The charset_conv() function is convert string to @e to from @e from
 */
OLIBC_API
char * charset_conv (CChar *src, CChar * from, CChar * to) // {{{
{
#ifdef HAVE_ICONV
	iconv_t	cd;
	ICONV_CONST char	* ibuf_t;
	char	* ibuf,
			* obuf,
			* obuf_t;
	size_t	err,
			ilen,
			olen,
			olen_t;

	if ( src == null || from == null || to == null )
		return null;

	if ( strcmp (from, to) == 0 ) {
		oc_strdup_r (obuf, src, null);
		return obuf;
	}

	ilen = strlen (src) + 1;
	oc_strdup_r (ibuf, src, null);
	ibuf_t = ibuf;

	olen = olen_t = charset_conv_outplen (to, ilen);
	oc_malloc (obuf, sizeof (char) * olen);
	if ( obuf == null ) {
		ofree (ibuf);
		return null;
	}
	obuf_t = obuf;

	cd = iconv_open (to, from);
	if ( cd == (iconv_t) -1 ) {
		oc_error ("Can not open iconv descriptor\n");
		return null;
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
		obuf = null;
	} else
		*obuf_t = 0;

skip_error:

	ofree (ibuf);
	iconv_close (cd);

	return obuf;
#else
	oc_error ("Don't support iconv in current build\n");
	return null;
#endif
} // }}}

/**
 * @brief	Split a string by string
 * @param[in]	src The input string.
 * @param[out]	oargc Number of return arraies
 * @param[in]	delimiter The boundary string.
 * @return		The string array
 * @sa	join ofree_array
 * @exception DEALLOCATE
 *   When occurs internal error, split() returns null.
 *   If the return string array pointer is not null, the caller should
 *   deallocate this buffer using @e ofree_array()
 *
 * Returns an array of strings, each of which is a substring of string
 * formed by splitting it on boundaries formed by the string delimiter.
 */
OLIBC_API
char ** split (CChar * src, int * oargc, CChar * delimiter) // {{{
{
	char	** sep;
	char	* buf;
	size_t	delno,
			len,
			dlen,
			start,
			end,
			i,
			j,
			no;

	*oargc = 0;
	if ( src == null || delimiter == null )
		return null;

	/* removed white space of front and end string */
	oc_strdup_r (buf, src, null);
	trim (buf);

	len = strlen (buf);
	dlen = strlen (delimiter);

	// todo: if dlen < 1, return whole src with 1 array.
	if ( len < 1 || dlen < 1 ) {
		ofree (buf);
		return null;
	}

	delno = get_charcount (buf, strlen (buf), delimiter, strlen (delimiter));
	delno++;
	oc_malloc_r (sep, sizeof (char *) * (delno + 1), null);

	start = 0;
	end = 0;
	no = 0;

	for ( i=0; i<len; i++ ) {
		for ( j=0; j<dlen; j++ ) {
			if ( buf[i] == delimiter[j] ) {
				if ( buf[i-1] != '\\' ) {
					end = i;
				}

				if ( start == end ) {
					end--;
					start++;
					break;
				}
			}

			if ( end > start ) {
				if ( only_whitespace (buf + start, end - start) ) {
					start = end + 1;
					break;
				}

				oc_strdup (sep[no], buf + start, end - start);
				if ( sep[no] == null ) {
					ofree (buf);
					ofree_array (sep);
					return null;
				}
				trim (sep[no]);
				OC_DEBUG ("ARRAY[%d] = %s\n", no, sep[no]);

				start = end + 1;
				no++;
				break;
			}
		}
	}

	if ( end != len && ! only_whitespace (buf+ start, 0) ) {
		oc_strdup (sep[no], buf + start, strlen (buf + start));
		trim (sep[no]);
		OC_DEBUG ("ARRAY[%d] = %s\n", no, sep[no]);
		no++;
	}

	ofree (buf);

	sep[no] = null;
	*oargc = no;

	return sep;
} // }}}

/**
 * @brief	Join array elements with a string
 * @param	glue delimeter of joined string.
 * @param	sep The array of strings to join.
 * @return		The pointer of joined string.
 * @sa	split
 * @exception DEALLOCATE
 *   When occurs internal error, join() returns null.
 *   If the return string array pointer is not null, the caller should
 *   deallocate this buffer using @e free()
 *
 * Join array elements with a glue string. If glue is set null,
 * join() api joined string with empty string.
 */
OLIBC_API
char * join (CChar * glue, CChar ** sep) // {{{
{
	ULong	alloc = 1024,
			plen = 0;
	int		glen;
	char	* buf;

	if ( glue == null ) {
		glue = "";
		glen = 0;
	} else
		glen = strlen (glue);

	oc_malloc_r (buf, sizeof (char) * alloc, null);

	while ( *sep != null ) {
		plen += strlen (*sep) + glen;
		if ( plen > (alloc - 1) ) {
			alloc = plen + 1024;
			oc_realloc_r (buf, sizeof (char) * alloc, null);
		}

		if ( strlen (buf) && glen )
			strcat (buf, glue);
		strcat (buf, *sep);
		sep++;
	}

	return buf;
} // }}}

/**
 * @brief	compare two strings that is reverse.
 * @param	s1 compared strings
 * @param	s2 compared strings
 * @param	compare length
 * @return		return an integer less than, equal to, or greater than zero.
 *				if s1 (or the first n bytes thereof) is found, respectively,
 *				to be less than, to match, or be greater than s2.
 *
 * This function is reverse strncmp.
 */
OLIBC_API
int strrncmp (CChar * s1, CChar * s2, int len) // {{{
{
	int s1len, s2len, clen;

	s1len = safe_strlen (a);
	s2len = safe_strlen (b);
	clen = (s1len > s2len) ? s2len : s1len;

	if ( len > clen )
		len = clen;

	s1len--; s2len--;
	for ( ; a[s1len--] == b[s2len--]; len-- ) {
		if ( s1len < 0 || s2len < 0 )
			break;

		if ( len == 0 )
			return 0;
	}

	s1len++; s2len++;

	return a[s1len] - b[s2len];
} // }}}

/**
 * @example trim.c
 *   The example for trim() and trim_r() api
 * @example addslashes.c
 *   The example for addslashes() api
 * @example str2long.c
 *   The example for str2long(), str2double(), char2int() and is_strint() api
 * @example setansi.c
 *   The example for setansi() api
 * @example humanSize.c
 *   The example for numberFormat(), human_size() api
 * @example caseSensitive.c
 *   The example for strtoupper() and strtolower() api
 * @example binhex.c
 *   The example for bin2hex() and hex2bin() api
 * @example bindec.c
 *   The example for dec2bin(), long2bin(), bin2dec() and bin2long() api
 * @example charset.c
 *   The example for is_ksc5601() and is_utf8() api
 * @example CharsetConv.c
 *   The example for charset_conv() api
 * @example split.c
 *   The exmaple for split() api
 */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
