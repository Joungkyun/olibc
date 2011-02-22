/*
 *                       Written by JoungKyun.Kim
 *            Copyright (c) 2011 JoungKyun.Kim <http://oops.org>
 *
 * -----------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of the JoungKyun.Kim nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * -----------------------------------------------------------------------------
 */

/** 
 * @file	libfile.c
 * @brief	File API
 *
 * This file includes file apis for easliy using
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-02-22 18:11:46 $
 * $Revision: 1.18 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libfile.c,v 1.18 2011-02-22 18:11:46 oops Exp $ */
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

/**
 * @brief	Checks whether a file or directory exists
 * @param	path Path to the file or directory
 * @param	mode check conditions.
 * 				_IS_NCHK check only exists
 * 				_IS_FILE check whether is regular file or not
 * 				_IS_DIR  check whether is regular directory or not
 * 				_IS_SLINK check whether is symbolic link or not
 * 				_IS_CDEV check whether is charactor device or not
 * 				_IS_BDEV check whether is block device or not
 * 				_IS_FIFO check whether is FIFO or not
 * 				_IS_SOCK check whether is socket or not
 * @return	bool
 */
OLIBC_API
bool file_exists (CChar *path, int mode) // {{{
{
	struct stat f;
	int ret;

	f.st_size = 0;
	ret = lstat (path, &f);

	if ( ret == -1 )
		return false;

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
			return true;
	}
} // }}}

/**
 * @brief	read file
 * @param	path file path
 * @return	text
 *
 * The result of this function is must freed.
 */
OLIBC_API
char * fileread (CChar * path) // {{{
{
	FILE * fp;
	size_t len = 0, length = 0;
	char tmp[FILEBUF] = { 0, }, * buf;
	struct stat f;

	if ( lstat (path, &f) == -1 ) {
		oc_error ("File not found : %s\n", path);
		return NULL;
	}

	if ( f.st_size < 1 ) {
		OC_DEBUG ("The file(%s) is empty\n", path);
		return NULL;
	}

	if ((fp = fopen(path, "rb")) == NULL) {
		oc_error ("Can not open %s with read mode\n", path);
		return NULL;
	}

	/* initialize tmp variavle */
	len = (f.st_size < 4) ? 4 : f.st_size;
	oc_malloc_r (buf, sizeof (char) * (len + 1), NULL);
	len = 0;

	while ( (length = fread (tmp, sizeof (char), FILEBUF - 1, fp)) > 0 ) {
		memmove (buf + len, tmp, length);
		len += length;
		memset (tmp, 0, FILEBUF);
	}
	memset (buf + len, 0, 1);
	fclose (fp);

	return buf;
} // }}}

/**
 * @brief	write file
 * @param	path of write
 * @param	strings for write
 * @param	mode bool. set true, append and set false, new file.
 * @return	On success return 0, otherwise return -1.
 *
 * The writefile() function is not binary safe.
 */
OLIBC_API
int writefile (CChar * filename, CChar * str, bool mode) // {{{
{
	struct stat s;

	FILE * fp;
	char * act = "wb";
	size_t len = 0;
	int ret;

	act = "wb";
	if ( mode == true ) {
		ret = stat (filename, &s);
		act = (stat (filename, &s) < 0) ? "wb" : "ab";
	}

	if ( (fp = fopen (filename, act)) == NULL ) {
		oc_error ("Can not open %s with write mode\n", filename);
		return -1;
	}

	// On append, add line feed
	if ( ! strcmp (act, "ab") ) {
		if ( fwrite ("\n", sizeof (char), 1, fp) != 1 ) {
			oc_error ("Writeing line feed failed: %s\n", filename);
			fclose (fp);
			return -1;
		}
	}

	len = strlen (str);
	if ( fwrite (str, sizeof(char), len, fp) != len ) {
		fclose(fp);
		oc_error ("Writeing failed: %s\n", filename);
		return -1;
	}

	fclose (fp);

	return 0;
} // }}}

/**
 * @brief	get absolute path
 * @param	path path for searching
 * @return	string that memory allocated
 *
 * The result of realpath_r() function is must free.
 */
OLIBC_API
char * realpath_r (CChar *path) // {{{
{
	struct stat f;
	size_t r;
	bool isdir;
	char filename[PATH_MAX + 1] = { 0, };
	char dirpath[PATH_MAX + 1] = { 0, };
	char curpath[PATH_MAX + 1] = { 0, };
	char *buf;

	if ( path == NULL )
		return NULL;

	// The given path is already real path.
	if ( path[0] == '/' ) {
		r = strlen (path);
		oc_malloc (buf, sizeof (char) * (r + 4));
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

	isdir = S_ISDIR (f.st_mode);

	if ( isdir == false ) {
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

	r = strlen (dirpath) + strlen (filename) + 4;
	oc_malloc (buf, sizeof (char) * r);
	if ( buf == NULL ) {
		errno = ENOENT;
		return NULL;
	}
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
