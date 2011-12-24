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
 * $Id$
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

#ifdef HAVE_PCRE_H
#include <pcre.h>
#endif

#ifndef PCRE_EXTRA_MATCH_LIMIT_RECURSION
#define PCRE_EXTRA_MATCH_LIMIT_RECURSION 0x0010
#endif

# ifdef __cplusplus
extern "C"
{
# endif

extern char * preg_quote (CChar * src, CChar * delim);
extern char * preg_replace (char * regex, char * replace, char * subject, int * retlen);
extern char * preg_replace_arr (char ** regex, char ** replace, char * subject, int regarr_no);
extern char * preg_grep (CChar * regex, CChar * subject, bool reverse);
extern char * preg_fgrep (CChar * regex, CChar * path, bool reverse);
extern bool preg_match (CChar * regex, CChar * subject);
extern int preg_match_r (CChar * regex, CChar * subject, CChar *** matches);

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
