/** 
 * @file	_race.h
 * @brief	RACE code API header file
 *
 * This file includes race code apis header file
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-01 04:17:47 $
 * $Revision: 1.11 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/*
 * Follow functions follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * $Id: _race.h,v 1.11 2011-03-01 04:17:47 oops Exp $
 */
#ifndef __RACE_H
#define __RACE_H

#include "libstring.h"

/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST
#define RacePrefix	"bq--"
#define COLOR		34

char * encode_race (char * domain, char * charset, int debug);
char * decode_race (char * domain, char * charset, int debug);
int permit_extension (char * tail);
int string_convert (char * dest, char * src, char * from, char * to, int debug);
char * race_compress (char * src, int len);
void race_uncompress (char * ret, char * src, int retsize);
int utf16_length (char * src);
int race_check_same (const char * src, int len);
int race_check_simple (const char * src, int len);
char * race_base32_encode (char * src);
char * race_base32_decode (char * src);
char en_base32 (char * src);
char * de_base32 (char src);
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
