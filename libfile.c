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

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
