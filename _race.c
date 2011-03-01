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
 * @file	_race.c
 * @brief	Row-Based ASCII Compatible Encoding(RACE) API for IDN
 *
 * This file includes proto type of RACE code apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-01 17:35:56 $
 * $Revision: 1.15 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: _race.c,v 1.15 2011-03-01 17:35:56 oops Exp $ */
#include <oc_common.h>
#include <_race.h>

#ifdef HAVE_ICONV_H
#include <iconv.h>

#ifdef EANBLE_NLS
#include <langinfo.h>
#endif

#define DEBUGARG 0

char ansist[] = "[1;34m";
char ansien[] = "[7;0m";
int  unbuflen = 0;
char *extension[] = { "com", "net", "org", "info", "biz", "name" };

char * decode_race (char * domain, char * charset, int debug) // {{{
{
	static char	redomain[1024];
	char		name[BUFSIZ],
				dedomain[1024],
				backupstr[1024],
				uncompress[1024];
	int			i = 0,
				len = 0;

	memset (redomain, 0, 1024);
	len = strlen (domain);

	/* convert to low case */
	strtolower (domain);

	/* if don't start 'bq--', name is not race code */
	if ( strncmp (domain, RacePrefix, 4) ) {
		strcpy (redomain, domain);
		return redomain;
	}

	memset (name, 0, BUFSIZ);
	strcpy (name, domain + 4);

	for ( i=len - 1; i>0; i-- ) {
		if ( name[i] & 0x80 ) {
			return domain;
		}
	}   

	if ( debug ) {
		fprintf (stderr, "\n");
		fprintf (stderr, "%s[ INPUT Date Check ]%s\n", ansist, ansien);
		fprintf (stderr, "%s-------------------------------------------%s\n", ansist, ansien);
		fprintf (stderr, "Input String     :     %s \n", domain);
		fprintf (stderr, "Convert Target   :     %s \n", name);
		fprintf (stderr, "\n");
	}

	/* decoding base 32 */
	memset (backupstr, 0, 1024);
	strcpy (backupstr, race_base32_decode (name));

	/* uncompress unicode hex */
	memset (uncompress, 0, 1024);
	race_uncompress (uncompress, backupstr, 1024);

	/* convert utf-16be to euc-kr */
	string_convert (dedomain, uncompress, "UTF-16BE", charset, debug);
	sprintf (redomain, "%s", dedomain);

	return redomain;
} // }}}

char * encode_race (char * domain, char * charset, int debug) // {{{
{
	static char	endomain[1024];
	char		name[BUFSIZ],
				utf16str[1024],
				backupstr[1024],
				compress[1024];
	int			i = 0,
				yes = 0,
				len = 0,
				comlen = 0;

	memset (endomain, 0, 1024);
	len = strlen (domain);

	/* convert to low case */
	strtolower (domain);

	memset (name, 0, BUFSIZ);
	strcpy (name, domain);

	for ( i=len - 1; i>0; i-- ) {
		if ( name[i] & 0x80 ) {
			yes = 1;
			break;
		}
	}

	if ( debug ) {
		fprintf (stderr, "\n");
		fprintf (stderr, "%s[ INPUT Data Check ]%s\n", ansist, ansien);
		fprintf (stderr, "%s-------------------------------------------%s\n", ansist, ansien);
		fprintf (stderr, "Input STring     :     %s \n", domain);
		fprintf (stderr, "Convert Target   :     %s \n", name);
		fprintf (stderr, "\n");
	}

	/* if singlebyte domain, don't convert */
	if ( ! yes ) {
		return domain;
	}

	memset (utf16str, 0, 1024);
	memset (backupstr, 0, 1024);

	strcpy (backupstr, name);
	len = utf16_length (backupstr);

	/* convert euc-kr to utf-16be */
	string_convert (utf16str, backupstr, charset, "UTF-16BE", debug);

	/* compress unicode */
	memset (compress, 0, 1024);
	strcpy (compress, race_compress (utf16str, len * 2));

	comlen = strlen (compress);

	if ( ! comlen ) {
		fprintf (stderr, "ERROR: Race code compress failed\n");
		return "";
	} else if ( comlen > 72 ) {
		fprintf (stderr, "ERROR: Compress string is bigger than 72 characters\n");
		return "";
	}

	memset (endomain, 0, 1024);
	sprintf (endomain, "%s%s", RacePrefix, race_base32_encode (compress));

	return endomain;
} // }}}

int permit_extension (char * tail) // {{{
{
	int	i = 0;

	for ( i=0; i<3; i++) {
		if ( ! strncmp (extension[i], tail, 3) ) {
			return 1;
		}
	}

	return 0;
} // }}}

int string_convert (char * dest, char * src, char * from, char * to, int debug) // {{{
{
	iconv_t	cd;
	char	* inbuf_p = src,
			* outbuf_p = dest,
			buf[9];

	size_t	il,
			ol;
	int		err,
			len;

    if ( strcmp (from, "UTF-16BE") ) len = strlen (src);
	else len = unbuflen * 2;

	il = len;
	ol = len * 3;

	if (src == NULL) {
		fprintf (stderr, "ERROR: input data is nothing\n");
		return 1;
	}

	if ( ! strcmp (to, from) ) {
		fprintf (stderr, "ERROR: original character set sames convert character set\n");
		strcpy (outbuf_p, src);
		return 1;
	}

	cd = iconv_open (to, from);

	if ( cd == (iconv_t) -1 ) {
		fprintf (stderr, "ERROR: Can't not open iconv point\n");
		strcpy (outbuf_p, src);
		return 1;
	}

	err = iconv ( cd, &inbuf_p, &il, &outbuf_p, &ol );

	if (err == (size_t) -1) {
		switch (errno) {
			case EINVAL:
				fprintf (stderr, "Incomplete text, do not report an error\n");
				return 1;
				break;
			case E2BIG:
				fprintf (stderr, "There is not sufficient room at *outbuf\n");
				strcpy (outbuf_p, src);
				return 1;
				break;
			case EILSEQ:
				fprintf (stderr, "An invalid multibyte sequence has been encountered in the input\n");
				strcpy (outbuf_p, src);
				return 1;
				break;
			default:
				fprintf (stderr, "ERROR: encoding error with iconv\n");
				strcpy (outbuf_p, src);
				return 1;
		}
	}
	iconv_close (cd);

	if ( debug ) {
		int i, utflen = 0;

		fprintf (stderr, "%sstring_convert() debug%s\n", ansist, ansien);
		fprintf (stderr, "%s-------------------------------------------%s\n", ansist, ansien);
		fprintf (stderr, "Convert Method   :     %s from %s\n", to, from);
		fprintf (stderr, "Convert String   :     %s\nHex Data         :     ", src);
		if ( strcmp ( from, "UTF-16BE") ) utflen = utf16_length (src);
		else utflen = unbuflen;

		for (i = 0; i < utflen; i++) {
			sprintf (buf, "%02X", dest[i] & 0x000000ff);
			memset (buf + 2, 0, 1);
				fprintf (stderr, "%s", buf);

			if ( (i % 2) == 1 ) fprintf (stderr, " ");
		}
		fprintf (stderr, "\n");
		fprintf (stderr, "Convert Length   :     %d\n", len);
		fprintf (stderr, "INPUT Remaining  :     %d\n", il);
		fprintf (stderr, "OUTPUT Remaining :     %d\n", ol);
		fprintf (stderr, "\n");
	}

	return 0;
} // }}}

void race_uncompress (char * ret, char * src, int retsize) // {{{
{
	char	tmp[9],
			buf[1024];
	int		i = 0,
			len = 0,
			buflen = 0,
			retlen = 0;

	memset (tmp, 0, sizeof (tmp));
	memset (ret, 0, retlen);
	memset (buf, 0, sizeof (buf));

	len = strlen (src);
	for ( i=0, buflen = 0; i<len; i+=8, buflen++ ) {
		strncpy (tmp, src + i, 8);
		if ( strlen(tmp) < 8 ) continue;

		memset (buf + buflen, bin2dec (tmp), 1);
	}

	if ( buf[0] == 0xffffffd8 ) {
		memcpy (ret, buf + 1, buflen);
	} else {
		i = 1;
		len = buflen;
		for ( i=1; i<len; i++ ) {
			if ( buf[i] == 0xffffffff ) {
				if ( (i+1) == len ) break;

				if ( buf[i+1] == 0xffffff99 ) {
					memset (ret + retlen, buf[0] & 0x000000ff, 1);
					memset (ret + retlen + 1, 0xff, 1);
				} else {
					memset (ret + retlen + 1, 0x00, 1);
					memset (ret + retlen, buf[i+1] & 0x000000ff, 1);
				}
				i++;
			} else {
				memset (ret + retlen, buf[0] & 0x000000ff, 1);
				memset (ret + retlen + 1, buf[i] & 0x000000ff, 1);
			}
			retlen += 2;
		}
	}

#if DEBUGARG
	fprintf (stderr, "%srace_uncompress() debug%s\n", ansist, ansien);
	fprintf (stderr, "%s-------------------------------------------%s\n", ansist, ansien);
	fprintf (stderr, "Origianl Source  :     %s\n", src);
	fprintf (stderr, "Hexa Convert     :     %s\n", bin2hex (src));
	fprintf (stderr, "Convert Data     :     %s\n", buf);
	fprintf (stderr, "Return Value     :     %s\n\n", ret);
#endif

	unbuflen = buflen - 1;
} // }}}

char * race_compress (char * src, int len) // {{{
{
	static char	outputstr[1024];
	char		buf[BUFSIZ];
	int			i = 0,
				simple = 0,
				outlen = 0;
#if DEBUGARG
	int			j = 0;
#endif

	memset (buf, 0, BUFSIZ);
	memset (outputstr, 0, 1024);

#if DEBUGARG
	/* for debugging code */
	fprintf (stderr, "%srace_compress() debug%s\n", ansist, ansien);
	fprintf (stderr, "%s-------------------------------------------%s\n", ansist, ansien);

	for ( i=0; i<len; i++ ) {
		//fprintf (stderr, "%d, %02x\n", i, src[i] << 24);
		fprintf (stderr, "%d, %02x\n", i, src[i] & 0x000000ff);
		sprintf (buf + j, "%02x", src[i] & 0x000000ff);
		j += 2;
	}   
	fprintf (stderr, "HEX CODE         : %s\n", buf);
#endif

	if ( ! race_check_same (src, len) ) {
		simple = race_check_simple (src, len);

		if ( simple != 0 )
			sprintf (outputstr, "%02x", simple & 0x000000ff);
		else {
			sprintf (outputstr, "%02x", 0xd8);
			outlen = strlen (outputstr);
			for ( i=0; i<len; i++ ) {
				if ( src[i] == 0 && src[i+1] == 0 )
					break;

				sprintf (outputstr + outlen, "%02x", src[i] & 0x000000ff); 
				outlen = strlen (outputstr);
			}

#if DEBUGARG
			fprintf (stderr, "Return Value     : %s\n\n", outputstr);
#endif
			return outputstr;
		}
	} else {
		sprintf (outputstr, "%02x", src[0] & 0x000000ff);
	}
	outlen = strlen (outputstr);

	if ( src[0] >= 0xffffffd8 && src[0] <= 0xffffffdc ) {
#if DEBUGARG
			fprintf (stderr, "Return Value     : NULL\n\n");
#endif
		return "";
	}

	for ( i=0; i<len; i+=2 ) {
		if ( src[i] == 0xffffff00 && src[i+1] == 0xffffff99 ) {
#if DEBUGARG
			fprintf (stderr, "Return Value     : NULL\n\n");
#endif
			return "";
		}

		if ( src[i] == src[0] && src[i+1] != 0xffffffff) {
			sprintf (outputstr + outlen, "%02x", src[i+1] & 0x000000ff);
			outlen = strlen (outputstr);
		} else if ( src[i] == src[0] && src[i+1] == 0xffffffff) {
			sprintf (outputstr + outlen, "%02x%02x", src[i+1] & 0x000000ff, 0x99);
			outlen = strlen (outputstr);
		} else {
			sprintf (outputstr + outlen, "%02x%02x", 0xff, src[i+1] & 0x000000ff);
			outlen = strlen (outputstr);
		}
	}

#if DEBUGARG
	fprintf (stderr, "Return Value     : %s\n\n", outputstr);
#endif

	return outputstr;
} // }}}

int utf16_length (char * src) // {{{
{
	static int	ulen = 0;
	int			i = 0;

	for ( i=0; i<strlen(src); i++ ) {
		if ( src[i] & 0x80 ) {
			i++;
		}

		ulen += 1;
	}

	return ulen;
} // }}}

int race_check_same (const char * src, int len) // {{{
{
	static int	same = 1;
	int			i = 0;

	for ( i=0; i<len-1; i+=2 ) {
		if ((src[i] & 0x000000ff) != (src[i+2] & 0x000000ff)) {
			same = 0;
			break;
		}
	}

	return same;
} // }}}

int race_check_simple (const char * src, int len) // {{{
{
	static int	secondval = 0;
	int			i = 0;

	for ( i=0; i<len-1; i+=2) {
		if ( secondval == 0 && src[i] != 0x0000)
			secondval = src[i];
		else if ( src[i] != secondval && src[i] != 0x0000 )
			return 0;
	}

	return secondval;
} // }}}

char * race_base32_encode (char * src) // {{{
{
	static char	ret[1024];
	char		buf[1024],
				buf1[6];
	int			i = 0,
				j = 0,
				slen = 0,
				buflen = 0,
				buf1len = 0,
				retlen = 0;

	memset (ret, 0, 1024);
	memset (buf, 0, 1024);
	memset (buf1, 0, 6);

	slen = strlen (src);

#if DEBUGARG
	fprintf (stderr, "%srace_base32_encode() debug%s\n", ansist, ansien);
	fprintf (stderr, "%s-------------------------------------------%s\n", ansist, ansien);
	fprintf (stderr, "Origianl Source  : %s\n", src);
#endif

	for ( i=0; i<slen; i++ ) {
		strcpy ( buf + buflen, _hex2bin (src[i]));
		buflen = strlen (buf);
	}

#if DEBUGARG
	fprintf (stderr, "Convert BIN data : %s\n", buf);
	fprintf (stderr, "Convert BIN len  : %d\n", strlen(buf));
#endif

	for ( i=0; i<buflen; i+=5 ) {
		strncpy ( buf1, buf + i, 5);

		buf1len = strlen (buf1);
		if ( buf1len < 5 ) {
			slen = 5 - buf1len;

			for ( j=0; j<slen; j++ ) {
				strncpy (buf1 + buf1len, "0", 1);
				buf1len = strlen (buf1);
			}
		}

		memset (ret + retlen, en_base32 (buf1), 1);
		retlen = strlen (ret);
	}

#if DEBUGARG
	fprintf (stderr, "Return Value     : %s\n\n", ret);
#endif

	return ret;
} // }}}

char * race_base32_decode (char * src) // {{{
{
	static char	ret[1024];
	int			i = 0,
				len = 0,
				retlen = 0,
				setlen = 0;

	memset (ret, 0, 1024);

	len = strlen (src);

	for ( i=0; i<len; i++ ) {
		strcpy (ret + retlen, de_base32 (src[i]));
		retlen = strlen (ret);
	}

	setlen = len * 5;
	if ( setlen % 8 ) {
		setlen = setlen - ( setlen % 8 );
		memset ( ret + setlen, 0, 1 );
	}

#if DEBUGARG
	fprintf (stderr, "%srace_base32_decode() debug%s\n", ansist, ansien);
	fprintf (stderr, "%s-------------------------------------------%s\n", ansist, ansien);
	fprintf (stderr, "Origianl Source  :     '%s'\n", src);
	fprintf (stderr, "Orig Source LEN  :     '%d'\n", len);
	fprintf (stderr, "Return Value     :     %s\n", ret);
	fprintf (stderr, "Return Value Len :     %d\n\n", strlen(ret));
#endif

	return ret;
} // }}}

char en_base32 (char * src) // {{{
{
	if ( ! strcmp (src, "00000") ) return 'a';
	else if ( ! strcmp (src, "00001") ) return 'b';
	else if ( ! strcmp (src, "00010") ) return 'c';
	else if ( ! strcmp (src, "00011") ) return 'd';
	else if ( ! strcmp (src, "00100") ) return 'e';
	else if ( ! strcmp (src, "00101") ) return 'f';
	else if ( ! strcmp (src, "00110") ) return 'g';
	else if ( ! strcmp (src, "00111") ) return 'h';
	else if ( ! strcmp (src, "01000") ) return 'i';
	else if ( ! strcmp (src, "01001") ) return 'j';
	else if ( ! strcmp (src, "01010") ) return 'k';
	else if ( ! strcmp (src, "01011") ) return 'l';
	else if ( ! strcmp (src, "01100") ) return 'm';
	else if ( ! strcmp (src, "01101") ) return 'n';
	else if ( ! strcmp (src, "01110") ) return 'o';
	else if ( ! strcmp (src, "01111") ) return 'p';
	else if ( ! strcmp (src, "10000") ) return 'q';
	else if ( ! strcmp (src, "10001") ) return 'r';
	else if ( ! strcmp (src, "10010") ) return 's';
	else if ( ! strcmp (src, "10011") ) return 't';
	else if ( ! strcmp (src, "10100") ) return 'u';
	else if ( ! strcmp (src, "10101") ) return 'v';
	else if ( ! strcmp (src, "10110") ) return 'w';
	else if ( ! strcmp (src, "10111") ) return 'x';
	else if ( ! strcmp (src, "11000") ) return 'y';
	else if ( ! strcmp (src, "11001") ) return 'z';
	else if ( ! strcmp (src, "11010") ) return '2';
	else if ( ! strcmp (src, "11011") ) return '3';
	else if ( ! strcmp (src, "11100") ) return '4';
	else if ( ! strcmp (src, "11101") ) return '5';
	else if ( ! strcmp (src, "11110") ) return '6';
	else if ( ! strcmp (src, "11111") ) return '7';

	return 0;
} // }}}

char * de_base32 (char src) // {{{
{
	if ( src == 'a' ) return "00000";
	else if ( src == 'b' ) return "00001";
	else if ( src == 'c' ) return "00010";
	else if ( src == 'd' ) return "00011";
	else if ( src == 'e' ) return "00100";
	else if ( src == 'f' ) return "00101";
	else if ( src == 'g' ) return "00110";
	else if ( src == 'h' ) return "00111";
	else if ( src == 'i' ) return "01000";
	else if ( src == 'j' ) return "01001";
	else if ( src == 'k' ) return "01010";
	else if ( src == 'l' ) return "01011";
	else if ( src == 'm' ) return "01100";
	else if ( src == 'n' ) return "01101";
	else if ( src == 'o' ) return "01110";
	else if ( src == 'p' ) return "01111";
	else if ( src == 'q' ) return "10000";
	else if ( src == 'r' ) return "10001";
	else if ( src == 's' ) return "10010";
	else if ( src == 't' ) return "10011";
	else if ( src == 'u' ) return "10100";
	else if ( src == 'v' ) return "10101";
	else if ( src == 'w' ) return "10110";
	else if ( src == 'x' ) return "10111";
	else if ( src == 'y' ) return "11000";
	else if ( src == 'z' ) return "11001";
	else if ( src == '2' ) return "11010";
	else if ( src == '3' ) return "11011";
	else if ( src == '4' ) return "11100";
	else if ( src == '5' ) return "11101";
	else if ( src == '6' ) return "11110";
	else if ( src == '7' ) return "11111";

	return "00000";
} // }}}

# else

char * encode_race (char * domain, char * charset, int debug)
{
	oc_error ("compiled without iconv\n");
	return domain;
}

char * decode_race (char * domain, char * charset, int debug)
{
	oc_error ("compiled without iconv\n");
	return domain;
}
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
