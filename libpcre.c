/* $Id: libpcre.c,v 1.10 2003-09-27 09:11:24 oops Exp $ */
#include <common.h>
#include <libpcre.h>

#ifdef HAVE_LIBPCRE
int lib_preg_match (char *regex, char *subject);
void lib_preg_parse (char *regex, char *pattern, int *option, int *study);
static int lib_preg_get_backref(char **str, int *backref);

/* follows PHP license 2.02 */
char * preg_quote (char *str, char *delimiters) {
	char *str_end,        /* End of the input string */
	     *out_str,        /* Output string with quoted characters */
	     *p,              /* Iterator for input string */
	     *q,              /* Iterator for output string */
	     delim_char=0,    /* Delimiter character to be quoted */
	     c;               /* Current character */
	int  quote_delim = 0; /* Whether to quote additional delim char */
	int  reallocsize = 0;
	int  chklen = 0, dellen = 0;

	str_end = str + strlen(str);

	if (delimiters == NULL)
		dellen = 0;
	else
		dellen = strlen (delimiters);

    /* Nothing to do if we got an empty string */
	if (str == str_end) {
		return ("");	// return empty_string
	}

	if ( dellen > 0 ) {
		delim_char = delimiters[0];
		quote_delim = 1;
	}

    /* Allocate enough memory so that even if each character
       is quoted, we won't run out of room */
	out_str = malloc(sizeof (char) * (strlen (str) + 33));
	reallocsize = sizeof (char) * (strlen (str) + 32);

    /* Go through the string and quote necessary characters */
    for(p = str, q = out_str; p != str_end; p++) {
        c = *p;
        switch(c) {
            case '.':
            case '\\':
            case '+':
            case '*':
            case '?':
            case '[':
            case '^':
            case ']':
            case '$':
            case '(':
            case ')':
            case '{':
            case '}':
            case '=':
            case '!':
            case '>':
            case '<':
            case '|':
            case ':':
                *q++ = '\\';
                *q++ = c;
                break;

            default:
                if (quote_delim && c == delim_char)
                    *q++ = '\\';
                *q++ = c;
                break;
        }
    }
    *q = '\0';

	chklen = strlen(out_str);

	if ( chklen == reallocsize ) {
		out_str = realloc (out_str, sizeof(char) * ( chklen + 32 ));
		reallocsize += 32;
	}

	return out_str;
}

void lib_preg_parse (char *regex, char *pattern, int *option, int *study) {
	int len = 0, i = 0, start = 0, end = 0, optlen = 0;
	int preg_opt = 0, delnum = 0;
	char delimiter = ' ', opt[16];

	len = strlen (regex);

	memset (opt, 0, 16);
	memset (pattern, 0, strlen (regex));

	for ( i=0; i<len; i++ ) {
		// check first delimiter
		if ( delimiter == ' ' ) {
			if ( ! isspace(regex[i]) ) {
				delimiter = regex[i];
				start = i;	
				delnum++;
				continue;
			} else
				continue;
		}

		if ( regex[i-1] != '\\' && regex[i] == delimiter ) {
			end = i;
			delnum++;
		}
	}

	if ( delnum != 2 ) {
		fprintf (stderr, "ERROR: wrong uses delimiters on regex rule\n");
		ofree (pattern);
		exit (FAILURE);
	}

	memcpy (pattern, regex + start + 1, end - start - 1);
	memset (pattern + end, 0, 1);

	strcpy (opt, regex + end + 1);
	optlen = strlen (opt);

	for ( i=0; i<optlen; i++ ) {
		switch (opt[i]) {
			// Perl compatible options
			case 'i' :	preg_opt |= PCRE_CASELESS;			break;
			case 'm' :	preg_opt |= PCRE_MULTILINE;			break;
			case 's' :	preg_opt |= PCRE_DOTALL;			break;
			case 'x' :	preg_opt |= PCRE_EXTENDED;			break;

			// PCRE specific options
			case 'A' :	preg_opt |= PCRE_ANCHORED;			break;
			case 'D' :	preg_opt |= PCRE_DOLLAR_ENDONLY;	break;
			case 'S' :	*study  = 1;						break;
			case 'U' :	preg_opt |= PCRE_UNGREEDY;			break;
			case 'X' :	preg_opt |= PCRE_EXTRA;				break;
			case 'u' :	preg_opt |= PCRE_UTF8;				break;

			case ' ':
			case '\n':
						break;
			default:
				fprintf (stderr, "ERROR: Unknown modifier '%c'", opt[i]);
				exit (FAILURE);
		}
	}

	*option = preg_opt;
}

/* follows PHP license 2.02 */
static int lib_preg_get_backref(char **str, int *backref) {
	register char in_brace = 0;
	register char *walk = *str;

	if (walk[1] == 0)
		return 0;

	if (*walk == '$' && walk[1] == '{') {
		in_brace = 1;
		walk++;
	}
	walk++;

	if (*walk >= '0' && *walk <= '9') {
		*backref = *walk - '0';
		walk++;
	} else
		return 0;
	
	if (*walk && *walk >= '0' && *walk <= '9') {
		*backref = *backref * 10 + *walk - '0';
		walk++;
	}

	if (in_brace) {
		if (*walk == 0 || *walk != '}')
			return 0;
		else
			walk++;
	}
	
	*str = walk;
	return 1;	
}

int lib_preg_match (char *regex, char *subject) {
	pcre *re = NULL;
	pcre_extra *extra = NULL;
	int preg_options = 0, *offsets, size_offsets;
	int val = 0, subjlen = 0, study = 0, erroffset;
	const char *error;
	char *pattern = NULL;
	unsigned const char *tables = NULL;
#if HAVE_SETLOCALE
	char			*locale = setlocale(LC_CTYPE, NULL);

	if ( strcmp (locale, "C") )
		tables = pcre_maketables();
#endif

	subjlen = strlen (subject);
	pattern = malloc (sizeof (char) * (strlen (regex) + 1));

	/* parse delimiters and regex string and option */
	lib_preg_parse (regex, pattern, &preg_options, &study);

	/* Compile pattern and display a warning if compilation failed. */
	re = pcre_compile (pattern, preg_options, &error, &erroffset, tables);
	if ( re == NULL ) {
		fprintf (stderr, "ERROR: Compilation failed: %s at offset %d", error, erroffset);
		ofree (pattern);
		exit (FAILURE);
	}

	size_offsets = (pcre_info(re, NULL, NULL) + 1) * 3;
	offsets = (int *) malloc (3 * sizeof (int) );
	/* Execute the regular expression. */
	if ((pcre_exec(re, extra, subject, subjlen, 0, 0, offsets, size_offsets)) > 0)
		val = 1;

	ofree(offsets);
	ofree(re);

	return val;
}

int preg_match (char *regex, char *subject) {
	char *tmp;
	int ret;

	tmp = malloc (sizeof (char) * (strlen (subject) + 1));
	/* if failed memory allocation, return unmatched */
	if ( tmp == NULL ) return 0;

	strcpy (tmp, subject);

	if ( lib_preg_match (regex, subject) )
		ret = 1;
	else
		ret = 0;

	ofree (tmp);
	return ret;
}

char * preg_grep (char *regex, char *subject, int opt) {
	char *token, *btoken;
	char *bufstr, buf[4096], *str = NULL;
	const char delimiters[] = "\n";
	int retval = 0, len = 0, buflen = 0;

	bufstr = strdup (subject);
	token = strtok_r (bufstr, delimiters, &btoken);

	while (token != NULL) {
		memset (buf, 0, 4096);
		sprintf (buf, "%s\n", token);
		buflen = strlen (buf);

		retval = lib_preg_match (regex, buf);

		/* print matched */
		if ( opt )
			retval = !retval ? 1 : 0;

		if ( retval ) {
			if ( len < 1 )
				str = malloc (sizeof (char) * (buflen + 2));
			else
				str = realloc (str, sizeof (char) * (len + buflen + 2));

			memcpy (str + len, buf, buflen);
			len += buflen;
			memset (str + len, 0, 1);
		}

		token = strtok_r (NULL, delimiters, &btoken);
	}

	memset (str + len -1, 0, 1);
	ofree (bufstr);

	return str;
}

char * preg_replace_arr (char *regex[], char *replace[], char *subject, int regarr_no) {
	int i, blen = 0;
	char * buf[regarr_no];

	for (i = 0; i<regarr_no; i++ ) {
		if ( i == 0 )
			buf[i] = (char *) preg_replace (regex[i], replace[i], subject, &blen);
		else {
			buf[i] = (char *) preg_replace (regex[i], replace[i], buf[i-1], &blen);
			ofree (buf[i-1]);
		}
	}
	
	return buf[regarr_no - 1];
}

/* follows PHP license 2.02
 * this function must free
 */
char * preg_replace (char *regex, char *replace, char *subject, int *retlen) {
	pcre			*re = NULL;			/* Compiled regular expression */
	pcre_extra		*extra = NULL;		/* Holds results of studying */
	int				 subjlen = 0;		/* subject length */
	int			 	 exoptions = 0;		/* Execution options */
	int			 	 preg_options = 0;	/* Custom preg options */
	int			 	 count = 0;			/* Count of matched subpatterns */
	int			 	*offsets;			/* Array of subpattern offsets */
	int			 	 size_offsets;		/* Size of the offsets array */
	int				 new_len;			/* Length of needed storage */
	int				 alloc_len;			/* Actual allocated length */
	int				 match_len;			/* Length of the current match */
	int				 backref;			/* Backreference number */
	int				 start_offset;		/* Where the new search starts */
	int				 g_notempty=0;		/* If the match should not be empty */
	int				 replace_len=0;		/* Length of replacement string */
	int				 study = 0, erroffset, soptions = 0;
	const char		*error;
	char			*pattern = NULL,
					*result,			/* Result of replacement */
					*new_buf,			/* Temporary buffer for re-allocation */
					*walkbuf,			/* Location of current replacement in the result */
					*walk,				/* Used to walk the replacement string */
					*match,				/* The current match */
					*piece,				/* The current piece of subject */
					*replace_end=NULL,	/* End of replacement string */
					 walk_last;			/* Last walked character */
	unsigned const char *tables = NULL;
#if HAVE_SETLOCALE
	char			*locale = setlocale(LC_CTYPE, NULL);
#endif

	subjlen = strlen (subject);
	pattern = malloc (sizeof (char) * (strlen (regex) + 1));

	/* parse delimiters and regex string and option */
	lib_preg_parse (regex, pattern, &preg_options, &study);

#if HAVE_SETLOCALE
	if ( strcmp (locale, "C") )
		tables = pcre_maketables();
#endif

	/* Compile pattern and display a warning if compilation failed. */
	re = pcre_compile (pattern, preg_options, &error, &erroffset, tables);
	if ( re == NULL ) {
		fprintf (stderr, "ERROR: Compilation failed: %s at offset %d", error, erroffset);
		ofree (pattern);
		exit (FAILURE);
	}

	/* If study option was specified, study the pattern and
	 * store the result in extra for passing to pcre_exec. */
	if ( study ) {
		extra = pcre_study(re, soptions, &error);
		if (error != NULL) {
			fprintf (stderr, "ERROR: While studying pattern\n");
			ofree (pattern);
			exit (FAILURE);
		}
	}

	ofree (pattern);

	replace_len = strlen(replace);
	replace_end = replace + replace_len;

	/* Calculate the size of the offsets array, and allocate memory for it. */
	size_offsets = (pcre_info(re, NULL, NULL) + 1) * 3;
	offsets = (int *) malloc (size_offsets * sizeof(int));
	
	alloc_len = 2 * subjlen + 1;
	result = malloc (alloc_len * sizeof(char));

	/* Initialize */
	match = NULL;
	*retlen = 0;
	start_offset = 0;
	
	while (1) {
		/* Execute the regular expression. */
		count = pcre_exec(re, extra, subject, subjlen, start_offset,
						  exoptions|g_notempty, offsets, size_offsets);

		/* Check for too many substrings condition. */
		if (count == 0) {
			fprintf (stderr, "ERROR: Matched, but too many substrings\n");
			exit (FAILURE);
		}

		piece = subject + start_offset;

		if (count > 0) {
			/* Set the match location in subject */
			match = subject + offsets[0];

			new_len = *retlen + offsets[0] - start_offset; /* part before the match */
			
			/* do regular substitution */
			walk = replace;
			walk_last = 0;
			while (walk < replace_end) {
				if ('\\' == *walk || '$' == *walk) {
					if (walk_last == '\\') {
						walk++;
						walk_last = 0;
						continue;
					}
					if (lib_preg_get_backref(&walk, &backref)) {
						if (backref < count)
							new_len += offsets[(backref<<1)+1] - offsets[backref<<1];
						continue;
					}
				}
				new_len++;
				walk++;
				walk_last = walk[-1];
			}

			if (new_len + 1 > alloc_len) {
				alloc_len = 1 + alloc_len + 2 * new_len;
				new_buf = malloc(alloc_len);
				memcpy(new_buf, result, *retlen);
				ofree(result);
				result = new_buf;
			}
			/* copy the part of the string before the match */
			memcpy(&result[*retlen], piece, match-piece);
			*retlen += match-piece;

			/* copy replacement and backrefs */
			walkbuf = result + *retlen;
			
			/* do regular backreference copying */
			walk = replace;
			walk_last = 0;
			while (walk < replace_end) {
				if ('\\' == *walk || '$' == *walk) {
					if (walk_last == '\\') {
						*(walkbuf-1) = *walk++;
						walk_last = 0;
						continue;
					}
					if (lib_preg_get_backref(&walk, &backref)) {
						if (backref < count) {
							match_len = offsets[(backref<<1)+1] - offsets[backref<<1];
							memcpy(walkbuf, subject + offsets[backref<<1], match_len);
							walkbuf += match_len;
						}
						continue;
					}
				}
				*walkbuf++ = *walk++;
				walk_last = walk[-1];
			}
			*walkbuf = '\0';
			/* increment the result length by how much we've added to the string */
			*retlen += walkbuf - (result + *retlen);

		} else { /* Failed to match */
			/* If we previously set PCRE_NOTEMPTY after a null match,
			   this is not necessarily the end. We need to advance
			   the start offset, and continue. Fudge the offset values
			   to achieve this, unless we're already at the end of the string. */
			if (g_notempty != 0 && start_offset < subjlen) {
				offsets[0] = start_offset;
				offsets[1] = start_offset + 1;
				memcpy(&result[*retlen], piece, 1);
				(*retlen)++;
			} else {
				new_len = *retlen + subjlen - start_offset;
				if (new_len + 1 > alloc_len) {
					alloc_len = new_len + 1; /* now we know exactly how long it is */
					new_buf = malloc (alloc_len * sizeof(char));
					memcpy(new_buf, result, *retlen);
					ofree(result);
					result = new_buf;
				}
				/* stick that last bit of string on our output */
				memcpy(&result[*retlen], piece, subjlen - start_offset);
				*retlen += subjlen - start_offset;
				result[*retlen] = '\0';
				break;
			}
		}
			
		/* If we have matched an empty string, mimic what Perl's /g options does.
		   This turns out to be rather cunning. First we set PCRE_NOTEMPTY and try
		   the match again at the same point. If this fails (picked up above) we
		   advance to the next character. */
		g_notempty = (offsets[1] == offsets[0])? PCRE_NOTEMPTY | PCRE_ANCHORED : 0;
		
		/* Advance to the next piece. */
		start_offset = offsets[1];
	}
	
	ofree (offsets);
	ofree (re);

	return result;
}
#else

char * preg_quote (char *string, char *delimiters) {
	fprintf (stderr, "ERROR: olibc compiled without pcre library\n");
	exit (FAILURE);
}

char * preg_replace (char *regex, char *replace, char *subject, int *retlen) {
	fprintf (stderr, "ERROR: olibc compiled without pcre library\n");
	exit (FAILURE);
}

char * preg_replace_arr (char *regex[], char *replace[], char *subject, int regarr_no) {
	fprintf (stderr, "ERROR: olibc compiled without pcre library\n");
	exit (FAILURE);
}

char * preg_grep (char *regex, char *str, int opt) {
	fprintf (stderr, "ERROR: olibc compiled without pcre library\n");
	exit (FAILURE);
}

int preg_match (char *regex, char *subject) {
	fprintf (stderr, "ERROR: olibc compiled without pcre library\n");
	exit (FAILURE);
}

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
