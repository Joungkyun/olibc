/** 
 * @file	libpcre.h
 * @brief	Extended perl compatible regular expression API header file.
 *
 * This file includes proto type of libpcre apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * @date	2011/02/21
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/*
 * $Id: libpcre.h,v 1.15 2011-03-22 15:52:53 oops Exp $
 *
 * Copyright (C) 2011 JoungKyun.Kim
 *
 * This file is part of olibc
 *
 * olibc is conditional free software; you can redistribute it and/or
 * modify it under the terms of the LESSER BOB PLEASE LICENSE or
 * the GNU Lesser General Public License as published by the Free Software
 * Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * GNU Libidn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GNU Libidn; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef LIBPCRE_H
#define LIBPCRE_H

#include <olibc/oc_type.h>

#ifdef HAVE_LIBPCRE
#include <pcre.h>
#endif

extern char * preg_quote (CChar * src, CChar * delim);
extern char * preg_replace (char * regex, char * replace, char * subject, int * retlen);
extern char * preg_replace_arr (char ** regex, char ** replace, char * subject, int regarr_no);
extern char * preg_grep (CChar * regex, CChar * subject, bool reverse);
extern char * preg_fgrep (CChar * regex, CChar * path, bool reverse);
extern bool preg_match (CChar * regex, CChar * subject);
extern int preg_match_r (CChar * regex, CChar * subject, CChar *** matches);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
