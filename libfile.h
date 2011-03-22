/** 
 * @file	libfile.h
 * @brief	File API header file
 *
 * This file includes proto type of file apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-22 15:52:53 $
 * $Revision: 1.13 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libfile.h,v 1.13 2011-03-22 15:52:53 oops Exp $ */
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

extern bool file_exists (const char * path, int mode);
// fileread is deprecated. replace with readfile
extern char * fileread (CChar * path);
extern char * readfile (CChar * path);
extern int writefile (CChar * path, CChar * data, bool mode);
extern char * realpath_r (CChar * path);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
