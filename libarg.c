/* $Id: libarg.c,v 1.15 2004-08-09 07:47:51 oops Exp $ */
#include <oc_common.h>
#include <libarg.h>

int _ogetopt_chk_int = -1;
int _ogetopt_cmd_int = 0;
int o_optlen;

char o_optarg[ARGLENGTH];
char **o_cmdarg;

int longopt_chk (char *option, const struct o_option *options);
int optvalue_chk (char option, const char *options);
char * convert_quoted_blank ( char * stream );
char * unconvert_quoted_blank ( char * stream );
int only_whitespace ( char * stream, int len );
void trim ( char * str );

int o_getopt (int oargc, char **oargv, const char *opt, const struct o_option *longopt) {
	register char **opt_t;
	char *x;
	int ret;
	int cint;
	int optargs_chk;
	int chklong, longno;
	char *longopt_sep;
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

	x = malloc (sizeof (char) * (strlen (*opt_t) + 1));
	strcpy(x, *opt_t);

	if ( chklong && ! strncmp ( "--", x, 2) ) {
		memset (longopt_sep_arg, 0, ARGLENGTH);

		longopt_sep = strchr (x, '=');
		if ( longopt_sep != NULL ) {
			strncpy (longopt_sep_arg, longopt_sep + 1, ARGLENGTH);
			*longopt_sep = 0;
		}
		
		longno = longopt_chk ( x + 2, longopt );

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
						fprintf (stderr, "No value of %s optoin\n\n", x);
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

		if ( strlen (x) > 2 ) {
			fprintf (stderr, "Unsupported option %s\n\n", x);
		}

		optargs_chk = optvalue_chk (x[1], opt);

		if ( optargs_chk == 1 ) {
			opt_t++;
			memset (o_optarg, 0, ARGLENGTH);

			if ( *opt_t == NULL || ! strncmp ("-", *opt_t, 1) ) {
				fprintf (stderr, "No value of -%c optoin\n\n", x[1]);
			} else {
				strncpy (o_optarg, *opt_t, ARGLENGTH);
				_ogetopt_chk_int--;
			}
		} else {
			memset (o_optarg, 0, ARGLENGTH);
		}
		ret = x[1];
	} else {
		ret = 0;
		memset (o_optarg, 0, ARGLENGTH);

		/* increased command line number argument */
		_ogetopt_cmd_int++;

		if ( _ogetopt_cmd_int == 1 )
			o_cmdarg = malloc ( sizeof (char *) * _ogetopt_cmd_int + 1 );
		else
			o_cmdarg = realloc ( o_cmdarg, sizeof (char *) * _ogetopt_cmd_int + 1 );

		o_cmdarg[_ogetopt_cmd_int] = NULL;

		o_cmdarg[_ogetopt_cmd_int - 1] = malloc ( sizeof (char) * (strlen (x) + 1));
		strcpy (o_cmdarg[_ogetopt_cmd_int - 1], x);
	}

	free (x);

	_ogetopt_chk_int--;
	o_optlen = strlen (o_optarg);

	if ( ! ret )
		goto retry;

	return ret;
}

/* build argv array variables */
char ** argv_make ( char * stream, int *oargc ) {
	const char delimiters[] = " \n\t";
	char ** oargv;
	char **sep, **sep_t;

	char *tmp, *tmparg;
	int i, white;

	if ( stream == NULL ) return NULL;
	if ( strlen (stream) < 1 ) return NULL;

	tmp = convert_quoted_blank ( stream );
	white = get_whitespace ( tmp );

	oargv = malloc ( sizeof (char *) * (white + 3) );
	sep   = malloc ( sizeof (char *) * (white + 3) );
	sep_t = sep;

	i = 0;
	while ( (*sep = strsep (&tmp, delimiters) ) != NULL ) {
		if ( **sep != 0 ) {
			tmparg = unconvert_quoted_blank ( *sep );
			oargv[i] = strdup ( tmparg );
			ofree ( tmparg );

			sep++;
			i++;
		}
	}

	ofree ( sep_t );
	ofree ( tmp );

	oargv[i] = NULL;
	*oargc = i;

	return oargv;
}

char ** split ( char * stream, int * oargc, char *delimiter ) {
	char **sep;
	int delno, len, dlen;
	int start, end;
	int i, j, no;

	/* removed white space of front and end string */
	trim (stream);

	if ( stream == NULL || delimiter == NULL ) {
		*oargc = 0;
		return NULL;
	}

	len = strlen (stream);
	dlen = strlen (delimiter);

	if ( len < 1 || dlen < 1 ) {
		*oargc = 0;
		return NULL;
	}

	delno = get_charcount (stream, delimiter);
	sep = malloc ( sizeof (char *) * (delno + 3) );

	start = 0;
	end = 0;
	no = 0;

	trim (stream);

	for ( i=0; i<len; i++ ) {
		for ( j=0; j<dlen; j++ ) {
			if ( stream[i] == delimiter[j] ) {
				if ( stream[i-1] != '\\' ) {
					end = i;
				}

				if ( start == end ) {
					end--;
					start++;
					break;
				}
			}

			if ( end > start && end - start != 0 ) {
				if ( only_whitespace ( stream + start, end - start ) ) {
					start = end + 1;
					break;
				}

				sep[no] = malloc ( sizeof (char) * ( end - start + 2 ) );
				memset ( sep[no], 0, end - start + 2 );
				memcpy ( sep[no], stream + start, end - start );
				trim (sep[no]);

				start = end + 1;
				no++;
				break;
			}
		}
	}

	if ( end != len && ! only_whitespace ( stream + start, 0 ) ) {
		sep[no] = strdup ( stream + start );
		no++;
	}

	sep[no] = NULL;
	*oargc = no;

	return sep;
}

/* free argv_make array */
void ofree_array ( char **oargv ) {
	int i = 0;

	if ( oargv != NULL ) {
		while ( oargv[i] != NULL ) {
			ofree (oargv[i]);
			i++;
		}
		ofree (oargv);
	}
}

/* convert blank to special string */
char * convert_quoted_blank ( char * stream ) {
	int len, newlen;
	int white, i, size;
	int squote = 0;
	int dquote = 0;
	int openquote = 0;
	char *ret;

	newlen = 0;
	len = strlen (stream);
	white = get_whitespace (stream);
	size = sizeof (char) * ((white * 8) + len + 1);

	ret = malloc ( size );
	memset  (ret, 0, size);

	for ( i = 0; i < len; i++ ) {
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

		if ( openquote == 1 && isspace ( stream[i] ) ) {
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
}


/* convert sepcail string to blank */
char * unconvert_quoted_blank ( char * stream ) {
	int i, j, len, size;
	char *ret;

	len = strlen (stream);
	size = sizeof (char) * (len + 1);

	ret = malloc ( size );
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
}

/* return number of white space */
int get_whitespace ( char * stream ) {
	int no, i, len;

	no = 0;
	len = strlen (stream);

	for ( i = 0; i < len; i++ ) {
		if ( isspace ( stream[i] ) ) no++;
	}

	return no;
}

int only_whitespace ( char * stream, int length ) {
	int i, len;

	len = length ? length : strlen (stream);

	for ( i = 0; i < len; i++ )
		if ( ! isspace (stream[i]) )
			return 0;

	return 1;
}

int optvalue_chk (char option, const char *options) {
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

	if ( val == -1 ) {
		fprintf (stderr, "Unsupported option -%c\n\n", option);
	}

	return val;
}

int longopt_chk (char *option, const struct o_option *options) {
	int i = 0;

	while ( 1 ) {
		if ( options[i].value == 0 )
			break;

		if ( ! strcmp ( option, options[i].name )) {
			return i;
		}
		i++;
	}

	fprintf (stderr, "Unsupported option --%s\n\n", option);

	return -1;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
