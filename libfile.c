/* $Id: libfile.c,v 1.3 2003-09-19 10:56:37 oops Exp $ */
#include <common.h>

#include <libfile.h>

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
	size_t fsize = 0, len = 0, length = 0;
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

	while ( (length = fread (tmp, sizeof (char), FILEBUF, fp)) > 0 ) {
		memmove (text + len, tmp, length);
		len += length;
		memset (tmp, 0, sizeof (tmp));
	}
	memset (tmp + len, 0, 1);

	fclose (fp);

	return text;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
