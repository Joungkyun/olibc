/* $Id: libarg.c,v 1.5 2003-10-28 17:39:24 oops Exp $ */
#include <common.h>
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

int o_getopt (int oargc, char **oargv, const char *opt, const struct o_option *longopt) {
	register char **opt_t;
	char *x;
	int ret = 0;
	int cint;
	int optargs_chk;
	int chklong = 0, longno;
	char *longopt_sep;
	char longopt_sep_arg[ARGLENGTH];

	/* init argument string length */
	o_optlen = 0;

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

		if ( longopt[longno].required == required_arguments ) {
			memset (o_optarg, 0, ARGLENGTH);

			if ( longopt_sep != NULL ) {
				/* if argument with equer (=) */
				strncpy (o_optarg, longopt_sep_arg, ARGLENGTH);
			} else {
				/* elseif argument with white charactor */
				opt_t++;

				if ( *opt_t == NULL || ! strncmp ("-", *opt_t, 1) ) {
					fprintf (stderr, "No value of %s optoin\n", x);
					exit (1);
				}

				strncpy (o_optarg, *opt_t, ARGLENGTH);
				_ogetopt_chk_int--;
			}
		} else {
			memset (o_optarg, 0, ARGLENGTH);
		}

		ret = longopt[longno].value;
	} else if ( x[0] == '-' ) {

		if ( strlen (x) > 2 ) {
			fprintf (stderr, "Unsupported option %s\n", x);
			exit (1);
		}

		optargs_chk = optvalue_chk (x[1], opt);

		if ( optargs_chk == 1 ) {
			opt_t++;

			if ( *opt_t == NULL || ! strncmp ("-", *opt_t, 1) ) {
				fprintf (stderr, "No value of -%c optoin\n", x[1]);
				exit (1);
			}

			memset (o_optarg, 0, ARGLENGTH);
			strncpy (o_optarg, *opt_t, ARGLENGTH);
			_ogetopt_chk_int--;
		} else {
			memset (o_optarg, 0, ARGLENGTH);
		}
		ret = x[1];
	} else {
		ret = 0;
		memset (o_optarg, 0, ARGLENGTH);

		/* increased command line number argument */
		_ogetopt_cmd_int++;

		if ( o_cmdarg == NULL )
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

	return ret;
}

/* build argv array variables */
char ** argv_make ( char * stream, int *oargc ) {
	const char delimiters[] = " \n\t";
	char ** oargv;
	char *tmp, *tmparg;
	char *token, *t_token, *btoken;
	int len, i, white;

	i = 0;
	len = strlen (stream);

	if ( len < 1 ) return NULL;

	tmp = convert_quoted_blank ( stream );
	white = get_whitespace ( tmp );


	oargv = malloc ( sizeof (char *) * white + 2 );

	t_token = strdup (tmp);
	token = strtok_r (t_token, delimiters, &btoken);

	while ( token != NULL ) {
		tmparg = unconvert_quoted_blank ( token );
		oargv[i] = strdup ( tmparg );
		ofree ( tmparg );

		i++;

		token = strtok_r (NULL, delimiters, &btoken);
	}

	oargv[i] = NULL;

	ofree ( tmp );

	*oargc = i;

	return oargv;
}

/* free argv_make array */
void ofree_array ( char **oargv ) {
	register char **i;

	if ( oargv != NULL ) {
		for ( i = oargv; *i != NULL; i++ ) {
			if ( *i != NULL ) { ofree ( *i ); }
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
		fprintf (stderr, "Unsupported option -%c\n", option);
		exit (1);
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

	fprintf (stderr, "Unsupported option --%s\n", option);
	exit (1);

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
