/* $Id: libpcre.c,v 1.14 2011-02-20 10:24:59 oops Exp $ */
#include <oc_common.h>
#include <libpcre.h>

#define DELIMITERS ".\\+*?[^]$(){}=!><|:"
#define DELIMITERS_LEN 19

bool lib_preg_parse (char * regex, char *pattern, int *option, int *study) // {{{
{
	int len, i, start, end;
	int preg_opt = 0, delnum = 0;
	char delimiter = ' ';
	char * opt;

	len = i = start = end = 0;

	if ( regex == NULL )
		return false;

	len = strlen (regex);

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
		oc_error ("Wrong uses delimiters on regex rule\n");
		return false;
	}

	memcpy (pattern, regex + start + 1, end - start - 1);
	memset (pattern + (end - start -1), 0, 1);

	opt = regex + end + 1;

	while ( *opt ) {
		switch (*opt) {
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
				oc_error ("Unknown modifier '%c'", *opt);
				return false;
		}
		opt++;
	}

	*option = preg_opt;
	return true;
} // }}}

static int lib_preg_get_backref (char ** str, int * backref) // {{{
{
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
} // }}}

/**
 * @brief	Perform a regular expression match
 * @param	regex The pattern to search for, as a string.
 * @param	subject The pattern to search for, as a string.
 * @param	matches array of matching strings
 * @return	returns the number of times pattern matches.
 *          0: function failed
 *          -1: no matched
 *
 * This function is privated.
 * 
 * The matches parameter is provided with the results of search.
 * matches[0] will contain the text that matched the full pattern,
 * matches[1] will have the text that matched the first captured
 * parenthesized subpattern, and so on.
 *
 * If return value is bigger than 0, matches is must memory freed
 * with free() function.
 */
int lib_preg_match (CChar * regex, CChar * subject, CChar *** matches) // {{{
{
	pcre		* re = NULL;
	pcre_extra	* extra = NULL;
	int			preg_options,
				* offsets,
				size_offsets,
				count,
				subjlen,
				study,
				erroffset,
				rc,
				num_subpats;
	CChar		* error;
	char		* pattern = NULL,
				* regex_t;
	UCChar		* tables = NULL;

	count = subjlen = study = 0;
	preg_options = erroffset = 0;
	num_subpats = 0;
	error = NULL;
	*matches = NULL;

	if ( regex == NULL || subject == NULL )
		return 0;

	subjlen = strlen (subject);
	oc_malloc_r (pattern, sizeof (char) * (strlen (regex) + 1), 0);

	/* parse delimiters and regex string and option */
	regex_t = (char *) regex;
	if ( lib_preg_parse (regex_t, pattern, &preg_options, &study) == false ) {
		ofree (pattern);
		return 0;
	}

#if HAVE_SETLOCALE
	{
		char * locale = setlocale (LC_CTYPE, NULL);

		if ( strcmp (locale, "C") )
			tables = pcre_maketables ();
	}
#endif

	/* Compile pattern and display a warning if compilation failed. */
	re = pcre_compile (pattern, preg_options, &error, &erroffset, tables);

	OC_DEBUG ("Pattern: %s\n", pattern);
	ofree (pattern);
	ofree ((UChar *) tables);

	if ( re == NULL ) {
		oc_error ("Compilation failed: %s at offset %d\n", error, erroffset);
		return 0;
	}

	if ( study ) {
		extra = pcre_study (re, 0, &error);
		if ( error != NULL )
			oc_error ("Error while studying pattern\n");
		if ( extra )
			extra->flags |= PCRE_EXTRA_MATCH_LIMIT | PCRE_EXTRA_MATCH_LIMIT_RECURSION;
	}

	rc = pcre_fullinfo (re, extra, PCRE_INFO_CAPTURECOUNT, &num_subpats);
	if ( rc < 0 ) {
		oc_error ("Internal pcre_fullinfo() error %d\n", rc);
		ofree (extra);
		ofree (re);
		return 0;
	}
	num_subpats++;
	size_offsets = num_subpats * 3;

	oc_malloc (offsets, sizeof (int) * size_offsets);
	if ( offsets == NULL ) {
		ofree (extra);
		ofree (re);
		return 0;
	}

	/* Execute the regular expression. */
	count = pcre_exec(re, extra, subject, subjlen, 0, 0, offsets, size_offsets);
	OC_DEBUG ("Matched count : %d\n", count);

	ofree (extra);
	ofree (re);

	if ( count == 0 ) {
		oc_error ("Matched, but too many substings\n");
		count = size_offsets / 3;
	}

	if ( count > 0 ) {
		int i;
		const char ** stringlist;

		if ( pcre_get_substring_list (subject, offsets, count, &stringlist) < 0) {
			ofree (offsets);
			return 0;
		}

#ifdef __OCDEBUG__
		for ( i=0; i<count; i++ )
			OC_DEBUG ("Matched String[%d] : %s\n", i, (char *) stringlist[i]);
#endif

		*matches = stringlist;
	}

	ofree (offsets);

	return count;
} // }}}

/**
 * @brief	quoted pcre regex reserved word
 * @param	src source string
 * @param	delim user defined delimiters
 * @return	quoted string
 *
 * The preg_quote() function returns allocated memory, so
 * need memory free with free() function
 */
OLIBC_API
char * preg_quote (CChar * src, CChar * delim) // {{{
{
	char	* delim_t,
			* buf;
	UInt	inlen,
			dellen,
			rlen;
	char	* outbuf,
			* ot;;

	if ( src == NULL )
		return NULL;

	inlen = strlen (src);
	dellen = delim ? strlen (delim) : 0;
	dellen += 19;

	oc_malloc_r (delim_t, sizeof (char) * dellen, NULL);
	memcpy (delim_t, DELIMITERS, DELIMITERS_LEN);
	memset (delim_t + dellen, 0, 1);

	if ( dellen > 19 ) {
		buf = delim_t + 19;
		while ( *delim ) {
			if ( strchr (delim_t, *delim) == NULL ) {
				*buf = *delim;
				buf++;
			}
			delim++;
		}
		*buf = 0;
	}

	OC_DEBUG ("DELIMETERS: %s\n", delim_t);

	rlen = inlen + 8;
	oc_malloc (outbuf, sizeof (char) * rlen);
	if ( outbuf == NULL ) {
		ofree (delim_t);
		return NULL;
	}

	buf = (char *) src;
	ot = outbuf;

	inlen = 1;
	while ( *buf ) {
		char *p;
		if ( (p = strchr (delim_t, *buf)) != NULL ) {
			if ( rlen <= inlen ) {
				rlen += 8;
				oc_realloc (outbuf, sizeof (char) * rlen);
				if ( outbuf == NULL ) {
					ofree (delim_t);
					ofree (outbuf);
					return NULL;
				}

				ot = outbuf + inlen - 1;
			}
			*ot++ = '\\';
			inlen++;
		}
		*ot++ = *buf++;
		inlen++;
	}
	*ot = 0;
	ofree (delim_t);

	return outbuf;
} // }}}

/**
 * @brief	Perform a regular expression match
 * @param	regex The pattern to search for, as a string.
 * @param	subject The pattern to search for, as a string.
 * @return	bool
 */
OLIBC_API
bool preg_match (CChar * regex, CChar * subject) // {{{
{
	CChar	** matches;
	int		count;

	count = lib_preg_match (regex, subject, &matches);
	ofree (matches);

	return (count > 0) ? true : false;
} // }}}

/**
 * @brief	Perform a regular expression match
 * @param	regex The pattern to search for, as a string.
 * @param	subject The pattern to search for, as a string.
 * @param	matches array of matching strings
 * @return	returns the number of times pattern matches.
 *          returns 0, it's function failed, and returns -1, no matched.
 *
 * The matches parameter is provided with the results of search.
 * matches[0] will contain the text that matched the full pattern,
 * matches[1] will have the text that matched the first captured
 * parenthesized subpattern, and so on.
 *
 * If return value is bigger than 0, matches is must memory freed
 * with free() function.
 */
OLIBC_API
int preg_match_r (CChar * regex, CChar * subject, CChar *** matches) // {{{
{
	return lib_preg_match (regex, subject, matches);
} // }}}


OLIBC_API
char * preg_grep (char *regex, char *subject, int opt) // {{{
{
	char *token, *btoken;
	char *bufstr, buf[4096], *str = NULL;
	const char delimiters[] = "\n";
	int retval = 0, len = 0, buflen = 0;
	CChar ** matches = NULL;

	bufstr = strdup (subject);
	token = strtok_r (bufstr, delimiters, &btoken);

	while (token != NULL) {
		memset (buf, 0, 4096);
		sprintf (buf, "%s\n", token);
		buflen = strlen (buf);

		retval = lib_preg_match (regex, buf, &matches);

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
} // }}}

OLIBC_API
char * preg_replace_arr (char *regex[], char *replace[], char *subject, int regarr_no) // {{{
{
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
} // }}}

/* follows PHP license 2.02
 * this function must free
 */
OLIBC_API
char * preg_replace (char *regex, char *replace, char *subject, int *retlen) // {{{
{
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
	int				 study = 0, erroffset;
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
	oc_malloc_r (pattern, sizeof (char) * (strlen (regex) + 1), NULL);

	/* parse delimiters and regex string and option */
	if ( lib_preg_parse (regex, pattern, &preg_options, &study) == false ) {
		ofree (pattern);
		return NULL;
	}

#if HAVE_SETLOCALE
	if ( strcmp (locale, "C") )
		tables = pcre_maketables();
#endif

	/* Compile pattern and display a warning if compilation failed. */
	re = pcre_compile (pattern, preg_options, &error, &erroffset, tables);
	if ( re == NULL ) {
		oc_error ("Compilation failed: %s at offset %d", error, erroffset);
		ofree (pattern);
		ofree ((UChar *) tables);
		return NULL;
	}

	/* If study option was specified, study the pattern and
	 * store the result in extra for passing to pcre_exec. */
	if ( study ) {
		extra = pcre_study(re, 0, &error);
		if (error != NULL) {
			oc_error ("While studying pattern\n");
			ofree (pattern);
			ofree ((UChar *) tables);
			ofree (re);
			return NULL;
		}
		if ( extra )
			extra->flags |= PCRE_EXTRA_MATCH_LIMIT | PCRE_EXTRA_MATCH_LIMIT_RECURSION;
	}

	ofree (pattern);
	ofree ((UChar *) tables);

	replace_len = strlen(replace);
	replace_end = replace + replace_len;

	/* Calculate the size of the offsets array, and allocate memory for it. */
	size_offsets = (pcre_info(re, NULL, NULL) + 1) * 3;
	offsets = (int *) malloc (size_offsets * sizeof(int));
	oc_malloc (offsets, sizeof (int) * size_offsets);
	if ( offsets == NULL ) {
		ofree (re);
		return NULL;
	}
	
	alloc_len = 2 * subjlen + 1;
	oc_malloc (result, sizeof (char) * alloc_len);
	if ( result == NULL ) {
		ofree (offsets);
		ofree (re);
		return NULL;
	}

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
			oc_error ("Matched, but too many substrings\n");
			ofree (offsets);
			ofree (result);
			ofree (re);
			return NULL;
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
} // }}}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
