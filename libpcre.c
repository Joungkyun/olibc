/* $Id: libpcre.c,v 1.21 2011-02-21 12:02:41 oops Exp $ */
#include <oc_common.h>
#include <libpcre.h>

#define DELIMITERS ".\\+*?[^]$(){}=!><|:"
#define DELIMITERS_LEN 19

typedef struct {
	pcre		* re;
	pcre_extra	* extra;
	char		* regex;
	char		* subject;
	int			* offsets;
	size_t		reglen;
	size_t		subjlen;
	int			size_offsets;
	int			g_notempty;
	int			exoptions;
	int			start_offset;
} PregArg;

bool libpreg_arg_init (PregArg ** pa) // {{{
{
	oc_malloc_r (*pa, sizeof (PregArg), false);

	(*pa)->re           = NULL;
	(*pa)->extra        = NULL;
	(*pa)->regex        = NULL;
	(*pa)->subject      = NULL;
	(*pa)->offsets      = NULL;
	(*pa)->reglen       = 0;
	(*pa)->subjlen      = 0;
	(*pa)->size_offsets = 0;
	(*pa)->g_notempty   = 0;
	(*pa)->exoptions    = 0;
	(*pa)->start_offset = 0;

	return true;
} // }}}

void libpreg_arg_free (PregArg ** pa) // {{{
{
	ofree ((*pa)->re);
	ofree ((*pa)->extra);
	ofree ((*pa)->offsets);
	(*pa)->regex = NULL;
	(*pa)->subject = NULL;
	(*pa)->reglen = 0;
	(*pa)->subjlen = 0;
	(*pa)->size_offsets = 0;

	ofree (*pa);
} // }}}

bool libpreg_parse (char * regex, char *pattern, int *option, int *study) // {{{
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
				oc_error ("Unknown modifier '%c'\n", *opt);
				return false;
		}
		opt++;
	}

	*option = preg_opt;
	return true;
} // }}}

static int libpreg_get_backref (char ** str, int * backref) // {{{
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

bool libpreg_compile (PregArg ** pa) // {{{
{
	PregArg		* p;
	int			preg_options,
				study,
				erroffset,
				rc,
				num_subpats;
	CChar		* error;
	char		* pattern = NULL;
	UCChar		* tables = NULL;

	p = *pa;
	oc_malloc_r (pattern, sizeof (char) * (p->reglen + 1), false);
	preg_options = study = 0;
	erroffset = num_subpats = 0;
	error = NULL;

	/* parse delimiters and regex string and option */
	if ( libpreg_parse (p->regex, pattern, &preg_options, &study) == false )
		return false;

#if HAVE_SETLOCALE
	{
		char * locale = setlocale (LC_CTYPE, NULL);

		if ( strcmp (locale, "C") )
			tables = pcre_maketables ();
	}
#endif

	/* Compile pattern and display a warning if compilation failed. */
	p->re = pcre_compile (pattern, preg_options, &error, &erroffset, tables);

	OC_DEBUG ("Pattern: %s\n", pattern);
	ofree (pattern);
	ofree ((UChar *) tables);

	if ( p->re == NULL ) {
		oc_error ("Compilation failed: %s at offset %d\n", error, erroffset);
		return false;
	}

	if ( study ) {
		p->extra = pcre_study (p->re, 0, &error);
		if ( error != NULL )
			oc_error ("Error while studying pattern\n");
		if ( p->extra )
			p->extra->flags |= PCRE_EXTRA_MATCH_LIMIT | PCRE_EXTRA_MATCH_LIMIT_RECURSION;
	}

	rc = pcre_fullinfo (p->re, p->extra, PCRE_INFO_CAPTURECOUNT, &num_subpats);
	if ( rc < 0 ) {
		oc_error ("Internal pcre_fullinfo() error %d\n", rc);
		return false;
	}
	num_subpats++;
	p->size_offsets = num_subpats * 3;

	return true;
} // }}}

int libpreg_execute (PregArg ** pa, bool cont_offset) // {{{
{
	PregArg		* p;
	int			count;

	p = *pa;

	if ( cont_offset == true ) {
		ofree (p->offsets);
		oc_malloc_r (p->offsets, sizeof (int) * p->size_offsets, 0);
		OC_DEBUG ("SUBJECT: %s\n", p->subject);
	}

	/* Execute the regular expression. */
	count = pcre_exec(
		p->re,
		p->extra,
		p->subject,
		p->subjlen,
		p->start_offset,
		(p->exoptions)|(p->g_notempty),
		p->offsets,
		p->size_offsets
	);
	OC_DEBUG ("Matched count : %d\n", count);
	OC_DEBUG ("Offsets first : %d\n", p->offsets[0]);

	if ( count == 0 ) {
		oc_error ("Matched, but too many substings\n");
		count = p->size_offsets / 3;
	}

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
 * @param	regex The pattern to search for, as a string
 * @param	subject The input string
 * @return	bool
 */
OLIBC_API
bool preg_match (CChar * regex, CChar * subject) // {{{
{
	PregArg	* pa;
	int		count;

	if ( regex == NULL || subject == NULL )
		return false;

	if ( libpreg_arg_init (&pa) == false )
		return false;

	pa->regex = (char *) regex;
	pa->reglen = strlen (regex);
	pa->subject = (char *) subject;
	pa->subjlen = strlen (subject);

	if ( libpreg_compile (&pa) == false ) {
		libpreg_arg_free (&pa);
		return false;
	}

	count = libpreg_execute (&pa, true);
	libpreg_arg_free (&pa);

	return (count > 0) ? true : false;
} // }}}

/**
 * @brief	Perform a regular expression match
 * @param	regex The pattern to search for, as a string.
 * @param	subject The input string
 * @param	matches array of matched strings
 * @return	returns the number of times pattern matches.
 *          returns 0, it's function failed, and returns -1, no matched.
 *
 * The matches parameter is provided with the results of search.
 * matches[0] will contain the text that matched the full pattern,
 * matches[1] will have the text that matched the first captured
 * parenthesized subpattern, and so on.
 *
 * If return value is bigger than 0, matches is must freed memory
 * with free() function.
 */
OLIBC_API
int preg_match_r (CChar * regex, CChar * subject, CChar *** matches) // {{{
{
	PregArg	* pa;
	int		count;

	*matches = NULL;

	if ( regex == NULL || subject == NULL )
		return 0;

	if ( libpreg_arg_init (&pa) == false )
		return 0;

	pa->regex = (char *) regex;
	pa->reglen = strlen (regex);
	pa->subject = (char *) subject;
	pa->subjlen = strlen (subject);

	if ( libpreg_compile (&pa) == false ) {
		libpreg_arg_free (&pa);
		return 0;
	}

	count = libpreg_execute (&pa, true);
	if ( count == 0 ) {
		libpreg_arg_free (&pa);
		return 0;
	}

	if ( count > 0 ) {
		int i;
		const char ** stringlist;

		if ( pcre_get_substring_list (subject, pa->offsets, count, &stringlist) < 0) {
			libpreg_arg_free (&pa);
			return 0;
		}

#ifdef __OCDEBUG__
		for ( i=0; i<count; i++ )
			OC_DEBUG ("Matched String[%d] : %s\n", i, (char *) stringlist[i]);
#endif

		*matches = stringlist;
	}

	libpreg_arg_free (&pa);
	return count;
} // }}}

/**
 * @brief	return matched line of given strings
 * @param	regex The pattern to search for, as a string.
 * @param	subject The input string
 * @param	reverse Set true, returns unmatched line
 * @return	matched string
 *
 * If return value is not NULL, it is must freed memory with
 * free() function.
 */
OLIBC_API
char * preg_grep (CChar *regex, CChar *subject, bool reverse) // {{{
{
	PregArg	* pa;
	int		count    = 0,
			linelen  = 0,
			chklen   = OC_LINEBUF,
			buflen   = 0,
			last     = 0;
	char	* buf    = NULL,
			* buf_t  = NULL,
			* start  = NULL,
			* end    = NULL,
			* subj,
			* subj_p = NULL,
			subj_t[OC_LINEBUF];

	if ( regex == NULL || subject == NULL )
		return NULL;

	if ( libpreg_arg_init (&pa) == false )
		return NULL;

	pa->regex = (char *) regex;
	pa->reglen = strlen (regex);

	if ( libpreg_compile (&pa) == false ) {
		libpreg_arg_free (&pa);
		return NULL;
	}

	start = (char *) subject;
	while ( (end = strchr (start, '\n')) != NULL ) {
		linelen = end - start + 1;

last_line:
		if ( linelen == 0 ) {
			start = end + 1;
			continue;
		}

		if ( linelen > (OC_LINEBUF + (-1)) ) {
			oc_strdup (subj_p, start, linelen + 1);
			if ( subj_p == NULL ) {
				libpreg_arg_free (&pa);
				return NULL;
			}
			subj = subj_p;
		} else {
			memcpy (subj_t, start, linelen);
			memset (subj_t + linelen, 0, 1);
			subj = subj_t;
		}

		pa->subject = (char *) subj;
		pa->subjlen = linelen;

		count = libpreg_execute (&pa, true);
		// regex fault
		if ( count == 0 ) {
			ofree (subj_p);
			libpreg_arg_free (&pa);
			return NULL;
		}

		if ( (count > 0 && reverse) || (count < 0 && ! reverse) )
			goto skip_print;

		if ( buflen == 0 ) {
			oc_malloc (buf, sizeof (char) * chklen);
			if ( buf == NULL ) {
				ofree (subj_p);
				libpreg_arg_free (&pa);
				return NULL;
			}
		}

		buf_t = buf + buflen;
		buflen += linelen;

		if ( buflen >= chklen ) {
			chklen *= 2;
			oc_realloc (buf, sizeof (char) * chklen);
			if ( buf == NULL ) {
				ofree (subj_p);
				libpreg_arg_free (&pa);
				return NULL;
			}
			buf_t = buf + buflen;
		}

		memcpy (buf_t, subj, linelen);

skip_print:

		ofree (subj_p);
		if ( last ) break;

		start = end + 1;
	}

	if ( ! last ) {
		last++;
		linelen = strlen (start);
		if ( linelen > 0 )
			goto last_line;
	}

	if ( buf != NULL ) {
		int l = 0;
		if ( *(buf + buflen - 1) == '\n' )
			l = -1;
		memset (buf + buflen + l, 0, 1);
	}

	libpreg_arg_free (&pa);
	return buf;
} // }}}

/**
 * @brief	return matched line of given file
 * @param	regex The pattern to search for, as a string.
 * @param	path The input file
 * @param	reverse Set true, returns unmatched line
 * @return	matched string
 *
 * If return value is not NULL, it is must freed memory with
 * free() function.
 */
OLIBC_API
char * preg_fgrep (CChar * regex, CChar * path, bool reverse) // {{{
{
	PregArg	* pa;
	FILE	* fp;
	struct	stat f;
	int		count    = 0,
			linelen  = 0,
			chklen   = OC_LINEBUF,
			buflen   = 0;
	char	* buf    = NULL,
			* buf_t  = NULL,
			line[OC_LINEBUF];

	if ( regex == NULL || path == NULL )
		return NULL;

	if ( lstat (path, &f) == -1 ) {
		oc_error ("File not found: %s\n", path);
		return NULL;
	}

	if ( f.st_size < 1 ) {
		oc_error ("The file(%s) is empty\n", path);
		return NULL;
	}

	if ((fp = fopen(path, "rb")) == NULL) {
		oc_error ("Can not open %s with read mode\n", path);
		return NULL;
	}

	if ( libpreg_arg_init (&pa) == false )
		return NULL;

	pa->regex = (char *) regex;
	pa->reglen = strlen (regex);

	if ( libpreg_compile (&pa) == false ) {
		libpreg_arg_free (&pa);
		fclose (fp);
		return NULL;
	}

	while ( fgets (line, OC_LINEBUF, fp) != NULL ) {
		linelen = strlen (line);
		pa->subject = (char *) line;
		pa->subjlen = linelen;

		count = libpreg_execute (&pa, true);
		// regex fault
		if ( count == 0 ) {
			libpreg_arg_free (&pa);
			fclose (fp);
			return NULL;
		}

		if ( (count > 0 && reverse) || (count < 0 && ! reverse) )
			goto skip_print;

		if ( buflen == 0 ) {
			oc_malloc (buf, sizeof (char) * chklen);
			if ( buf == NULL ) {
				libpreg_arg_free (&pa);
				fclose (fp);
				return NULL;
			}
		}

		buf_t = buf + buflen;
		buflen += linelen;

		if ( buflen >= chklen ) {
			chklen *= 2;
			oc_realloc (buf, sizeof (char) * chklen);
			if ( buf == NULL ) {
				libpreg_arg_free (&pa);
				fclose (fp);
				return NULL;
			}
			buf_t = buf + buflen;
		}

		memcpy (buf_t, line, linelen);
skip_print:
	}

	fclose (fp);
	libpreg_arg_free (&pa);

	if ( buf != NULL ) {
		int l = 0;
		if ( *(buf + buflen - 1) == '\n' )
			l = -1;
		memset (buf + buflen + l, 0, 1);
	}

	return buf;
} // }}}

OLIBC_API
char * preg_replace_arr (char ** regex, char ** replace, char * subject, int regarr_no) // {{{
{
	int i, blen = 0;
	char * buf[regarr_no];
	char * subj;

	for (i = 0; i<regarr_no; i++ ) {
		subj = (i > 0) ? buf[i-1] : subject;
		buf[i] = preg_replace (*regex++, *replace++, subj, &blen);
		if ( i > 0 )
			ofree (buf[i-1]);
	}

	i--;
	oc_strdup (subj, buf[i], strlen (buf[i]));
	ofree (buf[i]);
	
	return subj;
} // }}}

/**
 * @brief	Perform a regular expression search and replace
 * @parm[in]	regex The pattern to search for. It can be either
 *				a string or an array with strings.
 * @param[in]	replace Will replacing string
 * @param[in]	subject The input string
 * @retlen[out]	retlen The length of returned string
 * @return		The replaced string
 *
 * If matches are found, the new subject will be returned, otherwise
 * will be returned NULL if an error occurred.
 *
 * If return value is not null, it is must freed memory with
 * free() function
 *
 * This function is see also 'preg_replace of PHP'
 */
OLIBC_API
char * preg_replace (char *regex, char *replace, char *subject, int *retlen) // {{{
{
	PregArg		* pa = null;
	int			count = 0,			/* Count of matched subpatterns */
				new_len,			/* Length of needed storage */
				alloc_len,			/* Actual allocated length */
				match_len,			/* Length of the current match */
				backref,			/* Backreference number */
				replace_len = 0;	/* Length of replacement string */
	char		* result,			/* Result of replacement */
				* new_buf,			/* Temporary buffer for re-allocation */
				* walkbuf,			/* Location of current replacement in the result */
				* walk,				/* Used to walk the replacement string */
				* match,			/* The current match */
				* piece,			/* The current piece of subject */
				* replace_end = null,	/* End of replacement string */
				walk_last;			/* Last walked character */

	*retlen = 0;

	if ( regex == null && subject == null )
		return null;

	if ( replace == null )
		replace = "";

	if ( libpreg_arg_init (&pa) == false )
		return null;

	pa->regex = (char *) regex;
	pa->reglen = strlen (regex);
	pa->subject = (char *) subject;
	pa->subjlen = strlen (subject);

	if ( libpreg_compile (&pa) == false ) {
		libpreg_arg_free (&pa);
		return null;
	}

	oc_malloc (pa->offsets, sizeof (int) * pa->size_offsets);
	if ( pa->offsets == null ) {
		libpreg_arg_free (&pa);
		return NULL;
	}

	replace_len = strlen(replace);
	replace_end = replace + replace_len;

	alloc_len = 2 * pa->subjlen + 1;
	oc_malloc (result, sizeof (char) * alloc_len);
	if ( result == NULL ) {
		libpreg_arg_free (&pa);
		return NULL;
	}

	/* Initialize */
	match = NULL;
	
	while (1) {
		/* Execute the regular expression. */
		if ( (count = libpreg_execute (&pa, true)) == 0 ) {
			libpreg_arg_free (&pa);
			ofree (result);
			return null;
		}
		/* the string was already proved to be valid UTF-8 */
		pa->exoptions |= PCRE_NO_UTF8_CHECK;

		piece = subject + pa->start_offset;

		if (count > 0) {
			/* Set the match location in subject */
			match = subject + pa->offsets[0];

			new_len = *retlen + pa->offsets[0] - pa->start_offset; /* part before the match */
			
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
					if (libpreg_get_backref(&walk, &backref)) {
						if (backref < count)
							new_len += pa->offsets[(backref<<1)+1] - pa->offsets[backref<<1];
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
					if (libpreg_get_backref(&walk, &backref)) {
						if (backref < count) {
							match_len = pa->offsets[(backref<<1)+1] - pa->offsets[backref<<1];
							memcpy(walkbuf, subject + pa->offsets[backref<<1], match_len);
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
			if (pa->g_notempty != 0 && pa->start_offset < pa->subjlen) {
				pa->offsets[0] = pa->start_offset;
				pa->offsets[1] = pa->start_offset + 1;
				memcpy(&result[*retlen], piece, 1);
				(*retlen)++;
			} else {
				new_len = *retlen + pa->subjlen - pa->start_offset;
				if (new_len + 1 > alloc_len) {
					alloc_len = new_len + 1; /* now we know exactly how long it is */
					new_buf = malloc (alloc_len * sizeof(char));
					memcpy(new_buf, result, *retlen);
					ofree(result);
					result = new_buf;
				}
				/* stick that last bit of string on our output */
				memcpy(&result[*retlen], piece, pa->subjlen - pa->start_offset);
				*retlen += pa->subjlen - pa->start_offset;
				result[*retlen] = '\0';
				break;
			}
		}
			
		/* If we have matched an empty string, mimic what Perl's /g options does.
		   This turns out to be rather cunning. First we set PCRE_NOTEMPTY and try
		   the match again at the same point. If this fails (picked up above) we
		   advance to the next character. */
		pa->g_notempty = (pa->offsets[1] == pa->offsets[0])? PCRE_NOTEMPTY | PCRE_ANCHORED : 0;
		
		/* Advance to the next piece. */
		pa->start_offset = pa->offsets[1];
	}
	
	libpreg_arg_free (&pa);

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
