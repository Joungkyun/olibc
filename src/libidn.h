/*
 *  Copyright (C) 2017 JoungKyun.Kim <http://oops.org/>
 *
 *  This file is part of olibc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

/** 
 * @file	libidn.h
 * @brief	IDN API header file
 *
 * This file includes proto type of idn apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date$
 * $Revision$
 * @attention	Copyright (c) 2017 JoungKyun.Kim all rights reserved.
 */
#ifndef LIBIDN_H
#define LIBIDN_H

#include <olibc/oc_type.h>

# ifdef __cplusplus
extern "C"
{
# endif

extern CChar * stringprep_locale_charset_cache;
extern char * convert_punycode (CChar * src, CChar * charset);

# ifdef __cplusplus
}
# endif
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
