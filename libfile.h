/** 
 * @file	libfile.h
 * @brief	File API header file
 *
 * This file includes proto type of file apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-02-25 17:39:52 $
 * $Revision: 1.11 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libfile.h,v 1.11 2011-02-25 17:39:52 oops Exp $ */
#ifndef LIBFILE_H
#define LIBFILE_H

#include <olibc/oc_type.h>

#define _IS_NCHK	0
#define _IS_FILE	1
#define _IS_DIR		2
#define _IS_SLINK	3
#define _IS_CDEV	4
#define _IS_BDEV	5
#define _IS_FIFO	6
#define _IS_SOCK	7

bool file_exists (const char *path, int mode);
// fileread is deprecated. replace with readfile
char * fileread (CChar * path);
char * readfile (CChar * path);
int writefile (CChar * path, CChar * data, bool mode);
char * realpath_r (CChar *path);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
