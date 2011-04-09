/*
 *  Copyright (C) 2011 JoungKyun.Kim <http://oops.org/>
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
 * $Date: 2011-04-09 15:54:21 $
 * $Revision: 1.1 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libidn.h,v 1.1 2011-04-09 15:54:21 oops Exp $ */
#ifndef LIBIDN_H
#define LIBIDN_H

#include <olibc/oc_type.h>

#define ENDANSI		0
#define GRAY		1
#define BGRAY		2
#define RED			3
#define BRED		4
#define GREEN		5
#define BGREEN		6
#define YELLOW		7
#define BYELLOW		8
#define BLUE		9
#define BBLUE		10
#define MAGENTA		11
#define BMAGENTA	12
#define CYAN		13
#define BCYAN		14
#define WHITE		15
#define BWHITE		16

extern CChar * stringprep_locale_charset_cache;
extern char * convert_punycode (CChar * src, CChar * charset);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
