/* $Id: libfile.c,v 1.11 2009-10-29 14:01:36 oops Exp $ */
#include <oc_common.h>

#include <limits.h>
#include <libfile.h>

#ifndef PATH_MAX
#ifdef _POSIX_PATH_MAX
#define PATH_MAX _POSIX_PATH_MAX
#else
#define PATH_MAX 256
#endif
#endif

int file_exists (const char *path, int mode) {
	struct stat f;
	int ret;

	f.st_size = 0;
	ret = stat (path, &f);

	if ( ret == -1 )
		return 0;

	switch ( mode ) {
		case _IS_FILE :
			return S_ISREG (f.st_mode);
			break;
		case _IS_DIR :
			return S_ISDIR (f.st_mode);
			break;
		case _IS_SLINK :
			return S_ISLNK (f.st_mode);
			break;
		case _IS_CDEV :
			return S_ISCHR (f.st_mode);
			break;
		case _IS_BDEV :
			return S_ISBLK (f.st_mode);
			break;
		case _IS_FIFO :
			return S_ISFIFO (f.st_mode);
			break;
		case _IS_SOCK :
			return S_ISSOCK (f.st_mode);
			break;
		default:
			return 1;
	}
}

char * fileread (char * path) {
	FILE *fp;
	size_t len = 0, length = 0;
	char tmp[FILEBUF] = { 0, }, *text;
	struct stat f;

	if ((fp = fopen(path, "rb")) == NULL) {
		fprintf(stderr, "ERROR: Can't open %s in read mode\n", path);
		exit (FAILURE);
	}

	stat (path, &f);
	if ( f.st_size < 1 )
		return NULL;

	/* initialize tmp variavle */
	text = malloc (sizeof (char) * (f.st_size + 1));

	/* if failed memory allocation */
	if ( text == NULL )
		return NULL;

	memset (tmp, 0, FILEBUF);
	while ( (length = fread (tmp, sizeof (char), FILEBUF, fp)) > 0 ) {
		memmove (text + len, tmp, length);
		len += length;
		memset (tmp, 0, FILEBUF);
	}
	memset (text + len, 0, 1);

	fclose (fp);

	return text;
}

int writefile(char *filename, char *str, int mode) {
	struct stat s;

	FILE *fp;
	unsigned char *act, *string;
	int ret;

	if ( mode == 1) {
		ret = stat (filename, &s);

		if (ret < 0) {
			act = "wb";
			string = strdup(str);
		} else {
			act = "ab";
			string = (char *) malloc(strlen(str) + 32);
			sprintf(string, "\n%s", str);
		}
	} else {
		act = "wb";
		string = strdup(str);
	}

	if ( (fp = fopen(filename, act)) == NULL ) {
		fprintf (stderr, "ERROR: Can't open %s in write mode\n", filename);
		ofree (string);
		return -1;
	}

	if (fwrite(string, sizeof(char), strlen(string), fp) != strlen(string)) {
		fclose(fp);
		fprintf (stderr, "ERROR: writing to file %s\n", filename);
		ofree (string);
		return -1;
	}

	ofree(string);
	fclose(fp);

	return 0;
}

char * realpath_r (char *path) // {{{
{
	struct stat f;
	int r;
	short isdir;
	char filename[PATH_MAX + 1] = { 0, };
	char dirpath[PATH_MAX + 1] = { 0, };
	char curpath[PATH_MAX + 1] = { 0, };
	char *buf;

	if ( path[0] == '/' ) {
		r = strlen (path);
		buf = (char *) malloc (sizeof (char) * (r + 1));
		if ( buf == NULL ) {
			errno = ENOMEM;
			return NULL;
		}
		memcpy (buf, path, r);
		memset (buf + r, 0, 1);

		return buf;
	}

	if ( strlen (path) > PATH_MAX ) {
		errno = ENAMETOOLONG;
		return NULL;
	}

	if ( (r = stat (path, &f)) == -1 ) {
		errno = ENOENT;
		return NULL;
	}

	isdir = ( ! S_ISDIR(f.st_mode) ) ? 0 : 1;

	if ( ! isdir ) {
		// if path is filename
		if ( (buf = strrchr (path, '/')) == NULL ) {
			strcpy (filename, path);
			strcpy (dirpath, "./");
		} else {
			strcpy (filename, buf + 1);
			strcpy (dirpath, path);
			buf = strrchr (dirpath, '/') + 1;
			*buf = 0;
		}
	} else
		strcpy (dirpath, path);

	// save current directory
	if ( getcwd (curpath, PATH_MAX) == NULL )
		return NULL;

	if ( chdir (dirpath) == -1 )
		return NULL;

	memset (dirpath, 0, PATH_MAX + 1);
	if ( getcwd (dirpath, PATH_MAX) == NULL ) {
		chdir (curpath);
		return NULL;
	}

	if ( chdir (curpath) == -1 )
		return NULL;

	r = strlen (dirpath) + strlen (filename) + 2;
	buf = (char *) malloc (sizeof (char) * r);
	memset (buf, 0, r);

	r = strlen (dirpath);
	memcpy (buf, dirpath, r);
	memcpy (buf + r++, "/", 1);
	memcpy (buf + r, filename, strlen (filename));

	return buf;
} // }}}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
