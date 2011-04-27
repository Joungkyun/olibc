/* $Id: libstring.h,v 1.5 2004-02-03 08:44:05 oops Exp $ */
#ifndef LIBSTRING_H
#define LIBSTRING_H

#define ENDANSI		0
#define GRAY		1
#define BGRAY		2
#define RED			3
#define BRED		4
#define GREEN		5
#define BGREEN		6
#define YELLOW		7
#define BYELLOW		8
#define BLUE		9
#define BBLUE		10
#define MAGENTA		11
#define BMAGENTA	12
#define CYAN		13
#define BCYAN		14
#define WHITE		15
#define BWHITE		16

#define MAX(a, b)	(((a)>(b))?(a):(b))

void olibc_version (void);

/* addslashes follows PHP License 2.02 <http://www.php.net/license/2_02.txt>
 *
 * Returns a string with backslashes before characters that need to be quoted in
 * database queries etc. These characters are single quote ('), double quote ("),
 * backslash (\) and NUL (the NULL byte). 
 *
 * This function same as addslashes of PHP
 *
 * str         => original string
 * should_free => str memory free in addslashes
 *
 * return value must freed memory */
char * addslashes(char *str, int should_free);

/* trim follows BPL License v.1 <http://devel.oops.org/document/bpl>
 *
 * Strip whitespace from the beginning and end of a string
 * This function will convert original string. If you don't change
 * original string, use trim_r function.
 *
 * Warn, str variable must located memory.*/
void trim (char *str);

/* trim_r follows BPL License v.1 <http://devel.oops.org/document/bpl>
 *
 * return string that strip whitespace from the beginning and end
 * this function don't changed origianl string.
 *
 * str         =>original string
 * should_free => str memory free in function
 *
 * return value must freed memory */
char * trim_r (char *str, int should_free);

/* str2long follows BPL License v.1 <http://devel.oops.org/document/bpl>
 *
 * convert to long long type from string that composed int charactors
 *
 * this function valid until 19th */
long long str2long (char *s);

/* str2double follows BPL License v.1 <http://devel.oops.org/document/bpl>
 *
 * convert to long double type to string that compose int charactors and dot charactor
 *
 * this function valid unitil int 14th */
long double str2double (char *s);

/* char2int follows BPL License v.1 <http://devel.oops.org/document/bpl>
 *
 * return the int value that sames char charactor */
int char2int (char c);

/* check_int follows BPL License v.1 <http://devel.oops.org/document/bpl>
 *
 * check whether given charactor is int type or char type
 * if int type, return 1 nor return 0 */
int check_int (char c);

/* setansi follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * assign the color of file stream. after used this function, must recall
 * this function with ENDANSI
 *
 * stream => stdout / stderr / file streams etc
 * color  => assigned color
 *           ENDANSI     0
 *           GRAY        1
 *           BGRAY       2
 *           RED         3
 *           BRED        4
 *           GREEN       5
 *           BGREEN      6
 *           YELLOW      7
 *           BYELLOW     8
 *           BLUE        9
 *           BBLUE       10
 *           MAGENTA     11
 *           BMAGENTA    12
 *           CYAN        13
 *           BCYAN       14
 *           WHITE       15
 *           BWHITE      16
 * noansi => don't operate this function */
void setansi (FILE *stream, int color, int noansi);

/* human_size follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * returns auto convert with Byte/Bit unit with strings.
 *
 * size => original size
 * sub  => print with original size that grouped thousands
 * unit => 0 is bit unit and 1 is byte unit
 *
 * return values must allocated memory */
char * human_size (double size, int sub, int unit);

/* numberFormat follows PHP License 2.02 <http://www.php.net/license/2_02.txt>
 * return Format a number with grouped thousands with string
 *
 * d            => original number
 * dec point    => number ciphers
 * dec_point    => assigned character as point off
 * thousand_sep => assigned character as grouped thousand
 * print print  => convert value and return NULL
 *
 * return values must allocated memory */
char * numberFormat (double d, int dec, char dec_point, char thousand_sep, int print);

/* strtolower follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * convert string to low case
 *
 * this function convert original str */
void strtolower (char *str);

/* strtoupper follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * convert string to upper case
 *
 * this function convert original str */
void strtoupper (char *str);

/* bin2hex follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * convert binary type strings to hexadecimal type string
 *
 * this function convert original str */
char * bin2hex (char *str);

/* _bin2hex follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * convert binary type strings to hexadecimal charactor */
char _bin2hex (char *s);

/* hex2bin follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * convert binary type strings from hexadecimal type string
 *
 * this function convert original str */
char * hex2bin (char *str);

/* hex2bin follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * convert binary type strings from hexadecimal charactor */
char * _hex2bin (char c);

/* _bin2dec follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * convert to decimal number from binary type string */
int bin2dec (char *src);

/* convert_racecode follow BPL License v.1 <http://devel.oops.org/document/bpl>
 * encode and decode race code for multibyte domain (com/net/org)
 *
 * str   => original domain
 * mode  => 0 : encode to racecode
 *          1 : decode from racecode
 * debug => verbose mode */
char * convert_racecode (char *str, int mode, int debug);

/* convert_punycode follows GPL License v2
 * encode and decode punycode for local multibyte domain
 *
 * domain => original domain
 * mode   => 0 : encode to punycode
 *           1 : decode from punycode
 * debug => verbose mode */
char * convert_punycode (char * domain, int mode, int debug);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
