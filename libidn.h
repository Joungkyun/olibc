/** 
 * @file	libidn.h
 * @brief	IDN API header file
 *
 * This file includes proto type of idn apis
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-29 18:24:11 $
 * $Revision: 1.9.2.1 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/* $Id: libidn.h,v 1.9.2.1 2011-03-29 18:24:11 oops Exp $ */
#ifndef LIBIDN_H
#define LIBIDN_H

#include <olibc/oc_type.h>

extern const char * stringprep_locale_charset_cache;
UInt convert_punycode_r (CChar * src, UChar ** dst, bool mode, CChar * charset);
char * convert_punycode (char * domain, int mode, int debug);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
