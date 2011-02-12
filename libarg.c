/* $Id: libarg.c,v 1.22 2011-02-12 17:43:43 oops Exp $ */
#define LIBARG_SRC

#include <oc_common.h>
#include <libarg.h>

int _ogetopt_chk_int = -1;
int _ogetopt_cmd_int = 0;
int o_optlen;

char o_optarg[ARGLENGTH];
char ** o_cmdarg = NULL;

void trim (char * str);

int longopt_chk (const char * option, const struct o_option * options) // {{{
{
	int i = 0;

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

int optvalue_chk (const char option, const char * options) // {{{
{
	int len = strlen (options);
	int i, val = -1;

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

bool only_whitespace (const char * stream, int length) // {{{
{
	int i, len;

	len = length ? length : strlen (stream);

	for ( i = 0; i < len; i++ )
		if ( ! isspace (stream[i]) )
			return false;

	return true;
} // }}}

/*
 * convert blank to special string
 * need freed
 */
char * convert_quoted_blank (const char * stream) // {{{
{
	int len, newlen;
	int white, i, size;
	int squote = 0;
	int dquote = 0;
	int openquote = 0;
	char * ret;

	newlen = 0;
	len = strlen (stream);
	white = get_whitespace (stream);
	size = sizeof (char) * ((white * 8) + len + 1);

	oc_malloc_r (ret, size, NULL);
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

/*
 * convert sepcail string to blank
 * need freed
 */
char * unconvert_quoted_blank (const char * stream) // {{{
{
	int i, j, len, size;
	char * ret;

	if ( ! stream || ! strlen (stream) )
		return NULL;

	len = stream ? strlen (stream) : 0;
	size = sizeof (char) * (len + 1);

	oc_malloc_r (ret, size, NULL);
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

OLIBC_API
int o_getopt (int oargc, char ** oargv, const char * opt, const struct o_option * longopt) // {{{
{
	char ** opt_t;
	char * x;
	int ret;
	int cint, arglen;
	int optargs_chk;
	int chklong, longno;
	char * longopt_sep;
	char longopt_sep_arg[ARGLENGTH];

retry:
	/* init argument string length */
	o_optlen = 0;
	ret = 0;
	chklong = 0;

	if ( oargc < 2 ) return -1;

	if ( longopt != NULL )
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
		if ( longopt_sep != NULL ) {
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

				if ( longopt_sep != NULL ) {
					/* if argument with equer (=) */
					strncpy (o_optarg, longopt_sep_arg, ARGLENGTH);
				} else {
					/* elseif argument with white charactor */
					opt_t++;
	
					if ( *opt_t == NULL || ! strncmp ("-", *opt_t, 1) ) {
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

			if ( *opt_t == NULL || ! strncmp ("-", *opt_t, 1) ) {
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

		o_cmdarg[_ogetopt_cmd_int] = NULL;

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

/*
 * build argv array variables. must freed
 */
OLIBC_API
char ** argv_make (CChar * stream, int * oargc) // {{{
{
	CChar delimiters[] = " \n\t";
	char ** oargv;
	char ** sep, ** sep_t;

	char * tmp, * tmp_t, * tmparg;
	int i, white;

	if ( stream == NULL ) return NULL;
	if ( strlen (stream) < 1 ) return NULL;

	if ( (tmp = convert_quoted_blank (stream)) == NULL )
		return NULL;

	white = get_whitespace (tmp);

	oc_malloc (oargv, (sizeof (char *) * (white + 2)));
	if ( oargv ==  NULL) {
		ofree (tmp);
		return NULL;
	}

	oc_malloc (sep, sizeof (char *) * (white + 2));
	if ( sep == NULL ) {
		ofree (tmp);
		ofree (oargv);
		return NULL;
	}
	sep_t = sep;
	tmp_t = tmp;

	*oargc = i = 0;
	while ( (*sep = strsep (&tmp_t, delimiters) ) != NULL ) {
		if ( **sep != 0 ) {
			if ( (tmparg = unconvert_quoted_blank (*sep)) == NULL ) {
				oargv[i] = NULL;
				ofree_array (oargv);
				ofree (tmp);
				ofree (sep_t);
				return NULL;
			}

			oc_strdup (oargv[i], tmparg, strlen (tmparg));
			if ( oargv[i] == NULL ) {
				ofree_array (oargv);
				ofree (tmparg);
				ofree (tmp);
				ofree (sep_t);
				return NULL;
			}
			ofree (tmparg);

			sep++;
			i++;
		}
	}

	ofree (sep_t);
	ofree (tmp);

	oargv[i] = NULL;
	*oargc = i;

	return oargv;
} // }}}

/* must freed */
/**
 * @brief	Split a string by string
 * @param[in]	src The input string.
 * @param[out]	oargc number of return arraies
 * @param[in]	delimiter The boundary string.
 * @return		Returns an array
 *
 * Returns an array of strings, each of which is a substring of string
 * formed by splitting it on boundaries formed by the string delimiter.
 */
OLIBC_API
char ** split (CChar * src, int * oargc, CChar * delimiter) // {{{
{
	char ** sep;
	char * buf;
	int delno, len, dlen;
	int start, end;
	int i, j, no;

	*oargc = 0;
	if ( src == NULL || delimiter == NULL )
		return NULL;

	/* removed white space of front and end string */
	oc_strdup_r (buf, src, NULL);
	trim (buf);

	len = strlen (buf);
	dlen = strlen (delimiter);

	if ( len < 1 || dlen < 1 ) {
		ofree (buf);
		return NULL;
	}

	delno = get_charcount (buf, delimiter);
	delno++;
	oc_malloc_r (sep, sizeof (char *) * (delno + 1), NULL);

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
				if ( sep[no] == NULL ) {
					ofree (buf);
					ofree_array (sep);
					return NULL;
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

	sep[no] = NULL;
	*oargc = no;

	return sep;
} // }}}

/* free argv_make array */
OLIBC_API
void ofree_array (char ** argv_array) // {{{
{
	int i = 0;

	if ( argv_array != NULL ) {
		while ( argv_array[i] != NULL ) {
			ofree (argv_array[i]);
			i++;
		}
		ofree (argv_array);
	}
} // }}}

/* return number of white space */
OLIBC_API
int get_whitespace (CChar * src) // {{{
{
	int no, i, len;

	no = 0;
	len = strlen (src);

	for ( i = 0; i < len; i++ ) {
		if ( isspace (src[i]) ) no++;
	}

	return no;
} // }}}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
