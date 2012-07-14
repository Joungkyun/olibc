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
 *     * Neither the name of JoungKyun.Kim nor the url of oops.org
 *       nor the names of their contributors may be used to endorse or
 *       promote products derived from this software without specific prior
 *       written permission.
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
 *  This file is part of olibc.
 */

/** 
 * @file	libfile.c
 * @brief	File API
 *
 * This file includes file apis for easliy using
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date$
 * $Revision$
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id$ */
#include <oc_common.h>

#include <limits.h>
#include <libfile.h>
#include <libstring.h>
#include <libarg.h>

#ifndef PATH_MAX
#ifdef _POSIX_PATH_MAX
#define PATH_MAX _POSIX_PATH_MAX
#else
#define PATH_MAX 256 //!< Be declared If undefined PATH_MAX and _POSIX_PATH_MAX
#endif
#endif

/**
 * @brief	Checks type of file or directory
 * @param	path Path to the file or directory
 * @return	false or file status constant
 * @retval	false Failure
 * @retval	OC_IS_FILE
 * @retval	OC_IS_DIR
 * @retval	OC_IS_SLINK
 * @retval	OC_IS_CDEV
 * @retval	OC_IS_BDEV
 * @retval	OC_IS_FIFO
 * @retval	OC_IS_SOCK
 */
OLIBC_API
int file_status (CChar * path) // {{{
{
	struct	stat f;
	int		ret;

	f.st_size = 0;
	ret = safe_lstat (path, &f);

	if ( ret == -1 )
		return false;

	if ( S_ISREG (f.st_mode) )
		return OC_IS_FILE;
	else if ( S_ISDIR (f.st_mode) )
		return OC_IS_DIR;
	else if ( S_ISLNK (f.st_mode) )
		return OC_IS_SLINK;
	else if ( S_ISCHR (f.st_mode) )
		return OC_IS_CDEV;
	else if ( S_ISBLK (f.st_mode) )
		return OC_IS_BDEV;
	else if ( S_ISFIFO (f.st_mode) )
		return OC_IS_FIFO;
	else if ( S_ISSOCK (f.st_mode) )
		return OC_IS_SOCK;

	return false;
} // }}}

/**
 * @brief	Checks whether a file or directory exists
 * @param	path Path to the file or directory
 * @param	mode check conditions.
 * @code
 *   OC_IS_NCHK
 *   OC_IS_FILE
 *   OC_IS_DIR
 *   OC_IS_SLINK
 *   OC_IS_CDEV
 *   OC_IS_BDEV
 *   OC_IS_FIFO
 *   OC_IS_SOCK
 * @endcode
 * @return	bool
 * @retval	true Success
 * @retval	false Failure
 */
OLIBC_API
bool file_exists (CChar * path, int mode) // {{{
{
	struct	stat f;
	int		ret;

	f.st_size = 0;
	ret = safe_lstat (path, &f);

	if ( ret == -1 )
		return false;

	switch ( mode ) {
		case OC_IS_FILE :
			return S_ISREG (f.st_mode);
			break;
		case OC_IS_DIR :
			return S_ISDIR (f.st_mode);
			break;
		case OC_IS_SLINK :
			return S_ISLNK (f.st_mode);
			break;
		case OC_IS_CDEV :
			return S_ISCHR (f.st_mode);
			break;
		case OC_IS_BDEV :
			return S_ISBLK (f.st_mode);
			break;
		case OC_IS_FIFO :
			return S_ISFIFO (f.st_mode);
			break;
		case OC_IS_SOCK :
			return S_ISSOCK (f.st_mode);
			break;
		default:
			return true;
	}
} // }}}

/**
 * @brief	Reads entire file into a string.
 * @param	path The filename being read.
 * @param	buf The pointer of file context
 * @return	length of file
 * @sa		writefile
 * @exception DEALLOCATE
 *   When occurs internal error, 2th argument of readfile() has null.
 *   If the @e buf is not null, the caller should
 *   deallocate this buffer using @e free()
 *
 * The readfile() api reads data from path, and return length of
 * read data. This api is <b>binary safe</b>.
 */
OLIBC_API
size_t readfile (CChar * path, char ** buf) // {{{
{
	FILE	* fp;
	char	tmp[OC_FILEBUF] = { 0, };
	size_t	len = 0,
			length = 0;
	struct	stat f;

	*buf = null;

	if ( safe_lstat (path, &f) == -1 ) {
		oc_error ("File not found : %s\n", path);
		return 0;
	}

	if ( f.st_size < 1 ) {
		OC_DEBUG ("The file(%s) is empty\n", path);
		return 0;
	}

	if ((fp = fopen(path, "rb")) == null) {
		oc_error ("Can not open %s with read mode\n", path);
		return 0;
	}

	/* initialize tmp variavle */
	len = (f.st_size < 4) ? 4 : f.st_size;
	oc_malloc_r (*buf, sizeof (char) * (len + 1), 0);
	len = 0;

	while ( (length = fread (tmp, sizeof (char), OC_FILEBUF - 1, fp)) > 0 ) {
		memmove (*buf + len, tmp, length);
		len += length;
		memset (tmp, 0, OC_FILEBUF);
	}
	memset (*buf + len, 0, 1);
	fclose (fp);

	return len;
} // }}}

/**
 * @brief	Writes to a file
 * @param	path Path to the file where to write the data.
 * @param	data The data to write.
 * @param	size The length of data
 * @param	mode Bool. Set true, append and set false, new file.
 * @return	bool
 * @sa		readfile
 *
 * This api writes data to a file. This api is <b>binary safe</b>!
 */
OLIBC_API
bool writefile (CChar * path, CChar * data, size_t size, bool mode) // {{{
{
	struct	stat s;

	FILE	* fp;
	char	* act = "wb";
	int		ret;

	act = "wb";
	if ( mode == true ) {
		ret = stat (path, &s);
		act = (stat (path, &s) < 0) ? "wb" : "ab";
	}

	if ( (fp = fopen (path, act)) == null ) {
		oc_error ("Can not open %s with write mode\n", path);
		return -1;
	}

	if ( fwrite (data, sizeof(char), size, fp) != size) {
		fclose(fp);
		oc_error ("Writeing failed: %s\n", path);
		return false;
	}

	fclose (fp);

	return true;
} // }}}

/**
 * @brief	Returns canonicalized absolute pathname
 * @param	path The path being checked.
 * @return	Returns the canonicalized absolute pathname or null on failure.
 * @exception DEALLOCATE
 *   When occurs internal error, realpath_r() returns null.
 *   If the return string array pointer is not null, the caller should
 *   deallocate this buffer using @e free()
 *
 * The realpath_r() expands all symbolic links and resolves references to
 * '/./', '/../' and extra '/' characters in the null terminated string
 * named by path and returns the canonicalized absolute pathname in the
 * buffer of size PATH_MAX. The resulting path will have no symbolic link,
 * '/./' or '/../' components.
 *
 * If it returns NULL pointer, the global variable errno set follows:
 *
 * @code
 *   ENOENT
 *     The named file does not exist.
 *   ENOMEM
 *     The user memory cannot be mapped
 *   ERANGE
 *     Not enough space available for storing the path
 *   EFAULT
 *     Memory access violation occurs while copying
 *   ENAMETOOLONG
 *     A component of a pathname exceeded NAME_MAX characters, or an
 *     entire pathname exceeded PATH_MAX characters.
 * @endcode
 *
 * and so on.
 *
 * See also /usr/include/asm/errno.h
 */
OLIBC_API
char * realpath_r (CChar * path) // {{{
{
	char	* buf,
			resolved[PATH_MAX] = { 0, };

	if ( realpath (path, resolved) == null )
		return null;

	oc_strdup (buf, resolved, strlen (resolved));
	if ( buf == null )
		errno = ENOMEM;

	return buf;
} // }}}

OLIBC_API
int search_process (char *proc) { // {{{
	DIR * dir;
	struct dirent * entry;
	struct stat f;
	int proclen = safe_strlen (proc);
	int pid = 0;
	int ret = 1;

	if ( (dir = opendir ("/proc")) == NULL )
		return 1;

	while ( (entry = readdir (dir)) != NULL ) {
		char path[256] = { 0, };

		safe_lstat (path, &f);

		if ( (pid = atoi (entry->d_name)) <= 0 )
			continue;

		sprintf (path, "/proc/%d/cmdline", pid);

		{
			FILE * fp;
			char buf[256] = { 0, };
			char *bufp;

			if ( (fp = fopen (path, "r+")) == NULL )
				continue;

			bufp = fgets (buf, 256, fp);

			if ( ! strrncmp (proc, buf, proclen) ) {
				int buflen = strlen (buf);
				if ( proclen < buflen ) {
					if ( buf[buflen - proclen - 1] != '/' )
						continue;
				}

				ret = pid;
				break;
			}

			fclose (fp);
		}
	}

	closedir (dir);

	return ret;
} // }}}

/**
 * @example fileExists.c
 *   The example for file_exists() api
 * @example readfile.c
 *   The example for readfile(), writefile() api
 * @example realpath_r.c
 *   The example for relapath_r() api
 */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
