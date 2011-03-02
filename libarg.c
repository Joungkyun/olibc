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
 * @file	libarg.c
 * @brief	Command line argument API
 *
 * This file includes command line argument apis for easliy using
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-02 17:22:04 $
 * $Revision: 1.32 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libarg.c,v 1.32 2011-03-02 17:22:04 oops Exp $ */

/**
 * If this constants is not defined, declare extern global variables
 * in libarg.h
 */
#define LIBARG_SRC

#include <oc_common.h>
#include <libstring.h>
#include <libarg.h>

int _ogetopt_chk_int = -1; //!< o_getopt processing count
int _ogetopt_cmd_int = 0;  //!< Number of o_cmdarg array
//! String length of o_optarg variable. Use by o_getopt API
int o_optlen;
//! Value of current option. Use by o_getopt API
char o_optarg[ARGLENGTH];

/**
 * The o_cmdarg variable has command line arguments that
 * removed option arguments. This variable called by o_getopt
 * api and is must memory freed with @e ofree_array() function.
 */
char ** o_cmdarg = null;

/** @defgroup arg_internalfunc Argument API internal functions of olibc
 * @{
 */

/**
 * @brief	Check valid o_getopt long options
 * @param	option The input long option name
 * @param	options Array of valid o_getopt long options
 * @return	Number of valid long option array.
 * @retval	">=0" Number of array
 * @retval     -1  Invalid long option
 *
 * Check valid long option, and returns valid long option array number.
 *
 * This api is only internal. If you build with over gcc4,
 * you cannot access this api.
 */
static int longopt_chk (const char * option, const struct o_option * options) // {{{
{
	int	i = 0;

	while ( 1 ) {
		if ( options[i].value == 0 )
			break;

		if ( ! strcmp ( option, options[i].name ))
			return i;
		i++;
	}

	oc_error ("Unsupported option --%s\n\n", option);

	return -1;
} // }}}

/**
 * @brief	Check valid o_getopt short options
 * @param	option The input short option character
 * @param	options The valid option string
 * @return  integer
 * @retval	1 Valid option that has option value
 * @retval	0 Valid option that don't have option value
 * @retval	-1 Invalid option that don't have option value
 *
 * This api is only internal. If you build with over gcc4,
 * you cannot access this api.
 */
static int optvalue_chk (const char option, const char * options) // {{{
{
	int	len = strlen (options);
	int	i,
		val = -1;

	for ( i=0; i<len; i++ ) {
		if ( options[i] == option && options[i+1] == ':' ) {
			val = 1;
			break;
		} else if  ( options[i] == option ) {
			val = 0;
			break;
		}
	}

	if ( val == -1 )
		oc_error ("Unsupported option -%c\n\n", option);

	return val;
} // }}}

/**
 * @brief Check whether is exist white space or not in given string
 * @param	stream The input string
 * @param	length The length of input string
 * @return	bool
 * @retval	true Exsits only white space
 * @retval	false Don't exsit only white space
 *
 * This api is only internal. If you build with over gcc4,
 * you cannot access this api.
 */
static bool only_whitespace (const char * stream, int length) // {{{
{
	int	i,
		len;

	len = length ? length : strlen (stream);

	for ( i = 0; i < len; i++ )
		if ( ! isspace (stream[i]) )
			return false;

	return true;
} // }}}

/**
 * @brief	Preserve white space in the quoted string
 * @param	stream The input string
 * @return	The character pointer of preserved string
 * @exception DEALLOCATE
 *   When occurs internal error, convert_quoted_blank() returns null.
 *   If the return character pointer is not null, the caller should
 *   deallocate this buffer using @e free()
 *
 * convert_quoted_blank() function replaced white space in quoted string
 * to prserved from Internal Field Separator.
 *
 * This api is only internal. If you build with over gcc4,
 * you cannot access this api.
 */
static char * convert_quoted_blank (const char * stream) // {{{
{
	char	* ret;
	int		len,
			newlen,
			white,
			i,
			size,
			squote = 0,
			dquote = 0,
			openquote = 0;

	newlen = 0;
	len = strlen (stream);
	white = get_whitespace (stream);
	size = sizeof (char) * ((white * 8) + len + 1);

	oc_malloc_r (ret, size, null);
	memset  (ret, 0, size);

	for ( i=0; i<len; i++ ) {
		if ( openquote == 0 && stream[i] == '\'' ) {
			if ( ! squote && stream[i-1] != '\\' ) {
				openquote = 1;
				dquote = 1;
				continue;
			}
		} else if ( openquote == 0 && stream[i] == '"' ) {
			if ( ! dquote && stream[i-1] != '\\' ) {
				openquote = 1;
				squote = 1;
				continue;
			}
		} else if ( openquote == 1 && stream[i] == '"' ) {
			if ( !dquote && squote && stream[i-1] != '\\' ) {
				openquote = 0;
				squote = 0;
				continue;
			}
		} else if ( openquote == 1 && stream[i] == '\'' ) {
			if ( dquote && ! squote && stream[i-1] != '\\' ) {
				openquote = 0;
				dquote = 0;
				continue;
			}
		}

		if ( openquote == 1 && isspace (stream[i]) ) {
			memcpy (ret + newlen, "__OABC__", 8);
			newlen += 8;
		} else {
			if ( stream[i] == '\\' ) {
				if ( stream[i+1] == '\\' ) i++;
				else continue;
			}

			memset (ret + newlen, stream[i], 1);
			newlen++;
		}
	}

	return ret;
} // }}}

/**
 * @brief	revoke replaced white space
 * @param	stream The input string
 * @return	The character pointer of revoked string
 * @exception DEALLOCATE
 *   When occurs internal error, convert_unquoted_blank() returns null.
 *   If the return character pointer is not null, the caller should
 *   deallocate this buffer using @e free()
 *
 * unconvert_quoted_blank() function revoked replaced white space by
 * convert_quoted_blank() function
 *
 * This api is only internal. If you build with over gcc4,
 * you cannot access this api.
 */
static char * unconvert_quoted_blank (const char * stream) // {{{
{
	char * ret;
	int i,
		j,
		len,
		size;

	if ( ! stream || ! strlen (stream) )
		return null;

	len = stream ? strlen (stream) : 0;
	size = sizeof (char) * (len + 1);

	oc_malloc_r (ret, size, null);
	memset  (ret, 0, size);

	for (i = 0, j = 0; i <len; i++, j++ ) {
		if ( ! strncmp ("__OABC__", stream + i, 8) ) {
			memset (ret + j, ' ', 1);
			i += 7;
		} else {
			memset (ret + j, stream[i], 1);
		}
	}

	return ret;
} // }}}

/** @} */ // end of arg_internalfunc group


/**
 * @brief	Parse command-line options
 * @param	oargc Number of argv array
 * @param	oargv Input the argv array
 * @param	opt  The string containing the legitimate option characters.<br />
 *               Same as 3th argument of getopt. See also 'man 3 getopt'
 * @param	longopt The pointer to the first element of an array of struct
 *               o_option declared in <olibc/libarg.h>.<br />
 *               This is same as 4th argument of getopt_long. See also
 *               'man 3 getopt_long'<br /><br />
 *               If you don't want to use longopt, you can set NULL.
 * @retval	character The option character
 * @retval	-1 Close parsing
 * @sa getopt(3) getopt_long(3) ofree_array argv_make
 *
 * The o_getopt() function parses the command-line arguments similarly
 * getopt(1). Its arguents argc and argv are the argument count and array
 * as passed to the main() function on program invocation.
 *
 * After parsing, o_cmdarg global array variable has rest arguments
 * in the order except the option arguments.
 *
 * Before call o_getopt(), you need to initialize follow 2 variables.
 *
 * @code
 * _ogetopt_cmd_int = 0; // Number of o_cmdarg array
 * _ogetopt_chk_int = -1; // o_getopt processing count.
 * @endcode
 *
 * After using variables of o_getopt, you must be freed memory of o_cmdarg
 * variable with ofree_array() function.
 *
 * If option has values, this value is saved on o_optarg that size is 1024
 * byte and o_optlen is allocated length of o_optarg variable.
 */
OLIBC_API
int o_getopt (int oargc, char ** oargv, const char * opt, const struct o_option * longopt) // {{{
{
	char	** opt_t;
	char	* x;
	char	* longopt_sep;
	char	longopt_sep_arg[ARGLENGTH];
	int		ret,
			cint,
			arglen,
			optargs_chk,
			chklong,
			longno;

retry:
	/* init argument string length */
	o_optlen = 0;
	ret = 0;
	chklong = 0;

	if ( oargc < 2 ) return -1;

	if ( longopt != null )
	   	chklong = 1;

	if ( _ogetopt_chk_int == 0 )
		return -1;
	else
		_ogetopt_chk_int = ( _ogetopt_chk_int < 0 ) ? oargc - 1 : _ogetopt_chk_int;

	cint = oargc - _ogetopt_chk_int;
	opt_t = oargv + cint;

	arglen = strlen (*opt_t);
	oc_malloc_die (x, sizeof (char) * (arglen + 1));
	strcpy(x, *opt_t);

	if ( chklong && ! strncmp ( "--", x, 2) ) {
		memset (longopt_sep_arg, 0, ARGLENGTH);

		longopt_sep = strchr (x, '=');
		if ( longopt_sep != null ) {
			strncpy (longopt_sep_arg, longopt_sep + 1, ARGLENGTH);
			*longopt_sep = 0;
		}
		
		longno = longopt_chk (x + 2, longopt);

		if ( longno == -1 ) {
			ret = -2;
			memset (o_optarg, 0, ARGLENGTH);
		} else {

			if ( longopt[longno].required == required_arguments ) {
				memset (o_optarg, 0, ARGLENGTH);

				if ( longopt_sep != null ) {
					/* if argument with equer (=) */
					strncpy (o_optarg, longopt_sep_arg, ARGLENGTH);
				} else {
					/* elseif argument with white character */
					opt_t++;
	
					if ( *opt_t == null || ! strncmp ("-", *opt_t, 1) ) {
						oc_error ("No value of %s optoin\n\n", x);
					} else {
						strncpy (o_optarg, *opt_t, ARGLENGTH);
						_ogetopt_chk_int--;
					}
				}
			} else {
				memset (o_optarg, 0, ARGLENGTH);
			}

			ret = longopt[longno].value;
		}
	} else if ( x[0] == '-' ) {

		if ( arglen > 2 )
			oc_error ("Unsupported option %s\n\n", x);

		optargs_chk = optvalue_chk (x[1], opt);
		memset (o_optarg, 0, ARGLENGTH);

		if ( optargs_chk == 1 ) {
			opt_t++;

			if ( *opt_t == null || ! strncmp ("-", *opt_t, 1) ) {
				oc_error ("No value of -%c optoin\n\n", x[1]);
			} else {
				strncpy (o_optarg, *opt_t, ARGLENGTH);
				_ogetopt_chk_int--;
			}
		}
		ret = x[1];
	} else {
		ret = 0;
		memset (o_optarg, 0, ARGLENGTH);

		/* increased command line number argument */
		_ogetopt_cmd_int++;

		if ( _ogetopt_cmd_int == 1 ) {
			oc_malloc_die (o_cmdarg, sizeof (char *) * (_ogetopt_cmd_int + 1));
		} else {
			oc_realloc_die (o_cmdarg, sizeof (char *) * (_ogetopt_cmd_int + 1));
		}

		o_cmdarg[_ogetopt_cmd_int] = null;

		oc_malloc_die (o_cmdarg[_ogetopt_cmd_int - 1], sizeof (char) * (strlen (x) + 1));
		strcpy (o_cmdarg[_ogetopt_cmd_int - 1], x);
	}

	ofree (x);

	_ogetopt_chk_int--;
	o_optlen = strlen (o_optarg);

	if ( ! ret )
		goto retry;

	return ret;
} // }}}

/**
 * @brief	Make string array with the input string using white space delimiters.
 * @param[in]	stream	The input string
 * @param[out]	oargc	Number of string arrays
 * @return	Pointer of string array
 * @sa ofree_array
 * @exception DEALLOCATE
 *   When occurs internal error, argv_make() returns null.
 *   If the return string array pointer is not null, the caller should
 *   deallocate this buffer using @e ofree_array()
 *
 * The argv_make() function make string array with white space delimiters
 * from the input string. The last array element has NULL terminator.
 *
 * The white space in the quoted string is preserved.
 */
OLIBC_API
char ** argv_make (CChar * stream, int * oargc) // {{{
{
	CChar	delimiters[] = " \n\t";
	char	** oargv;
	char	** sep,
			** sep_t;

	char	* tmp,
			* tmp_t,
			* tmparg;
	int		i,
			white;

	if ( stream == null ) return null;
	if ( strlen (stream) < 1 ) return null;

	if ( (tmp = convert_quoted_blank (stream)) == null )
		return null;

	white = get_whitespace (tmp);

	oc_malloc (oargv, (sizeof (char *) * (white + 2)));
	if ( oargv ==  null) {
		ofree (tmp);
		return null;
	}

	oc_malloc (sep, sizeof (char *) * (white + 2));
	if ( sep == null ) {
		ofree (tmp);
		ofree (oargv);
		return null;
	}
	sep_t = sep;
	tmp_t = tmp;

	*oargc = i = 0;
	while ( (*sep = strsep (&tmp_t, delimiters) ) != null ) {
		if ( **sep != 0 ) {
			if ( (tmparg = unconvert_quoted_blank (*sep)) == null ) {
				oargv[i] = null;
				ofree_array (oargv);
				ofree (tmp);
				ofree (sep_t);
				return null;
			}

			oc_strdup (oargv[i], tmparg, strlen (tmparg));
			if ( oargv[i] == null ) {
				ofree_array (oargv);
				ofree (tmparg);
				ofree (tmp);
				ofree (sep_t);
				return null;
			}
			ofree (tmparg);

			sep++;
			i++;
		}
	}

	ofree (sep_t);
	ofree (tmp);

	oargv[i] = null;
	*oargc = i;

	return oargv;
} // }}}

/**
 * @brief	Split a string by string
 * @param[in]	src The input string.
 * @param[out]	oargc Number of return arraies
 * @param[in]	delimiter The boundary string.
 * @return		The string array
 * @sa	ofree_array
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
	int		delno,
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

	if ( len < 1 || dlen < 1 ) {
		ofree (buf);
		return null;
	}

	delno = get_charcount (buf, delimiter);
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
 * @brief	free memory that allocated by argv_make() function
 * @param	argv_array Return array pointer of argv_make() function
 * @return	void
 * @sa	argv_make
 *
 * The ofree_array is freed memory that allocated by argv_make()
 * function.
 */
OLIBC_API
void ofree_array (char ** argv_array) // {{{
{
	int	i = 0;

	if ( argv_array != null ) {
		while ( argv_array[i] != null ) {
			ofree (argv_array[i]);
			i++;
		}
		ofree (argv_array);
	}
} // }}}

/* return number of white space */
/**
 * @brief	Get number of white space
 * @param	src The input string
 * @return	Number of white space
 */
OLIBC_API
int get_whitespace (CChar * src) // {{{
{
	int	no,
		i,
		len;

	no = 0;
	len = strlen (src);

	for ( i = 0; i < len; i++ ) {
		if ( isspace (src[i]) ) no++;
	}

	return no;
} // }}}

/**
 * @example argvMake.c
 *   The example for argv_make() api
 * @example getWhitespace.c
 *   The example for get_whitespace() api
 * @example oGetopt.c
 *   The example for o_getopt() api
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
