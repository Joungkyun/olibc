/* $Id: libfile.c,v 1.10 2004-08-09 07:47:51 oops Exp $ */
#include <oc_common.h>

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

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
