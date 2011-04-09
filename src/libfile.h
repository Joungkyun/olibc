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
 * @file	libfile.h
 * @brief	File API header file
 *
 * This file includes proto type of file apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-04-09 16:03:05 $
 * $Revision: 1.2 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libfile.h,v 1.2 2011-04-09 16:03:05 oops Exp $ */
#ifndef LIBFILE_H
#define LIBFILE_H

#include <olibc/oc_type.h>

# ifdef __cplusplus
extern "C"
{
# endif

/** @defgroup file_exists_constant file_exsits constants
 * These constants are used by file_exsists() api, and defined
 * in libfile.h
 * @{
 */
#define OC_IS_NCHK	0 //!< check only exists 
#define OC_IS_FILE	1 //!< check whether is regular file or not
#define OC_IS_DIR	2 //!< check whether is regular directory or not
#define OC_IS_SLINK	3 //!< check whether is symbolic link or not
#define OC_IS_CDEV	4 //!< check whether is character device or not
#define OC_IS_BDEV	5 //!< check whether is block device or not
#define OC_IS_FIFO	6 //!< check whether is FIFO or not
#define OC_IS_SOCK	7 //!< check whether is socket or not
/** @} end of file_exists_constant group */

extern int file_status (CChar * path);
extern bool file_exists (CChar * path, int mode);
extern size_t readfile (CChar * path, char ** buf);
extern bool writefile (CChar * path, CChar * data, size_t size, bool mode);
extern char * realpath_r (CChar * path);

# ifdef __cplusplus
}
# endif
#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
