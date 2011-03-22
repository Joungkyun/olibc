/** 
 * @file	libidn.h
 * @brief	IDN API header file
 *
 * This file includes proto type of idn apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-22 16:11:18 $
 * $Revision: 1.11 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libidn.h,v 1.11 2011-03-22 16:11:18 oops Exp $ */
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
extern UInt convert_punycode_r (CChar * src, UChar ** dst, bool mode, CChar * charset);
extern char * convert_punycode (char * domain, int mode, int debug);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
