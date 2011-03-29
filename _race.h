/** 
 * @file	_race.h
 * @brief	RACE code API header file
 *
 * This file includes race code apis header file
 *
 * @author	JoungKyun.Kim <http://oops.org>
 * $Date: 2011-03-29 10:12:17 $
 * $Revision: 1.12.2.1 $
 * @attention	Copyright (c) 2011 JoungKyun.Kim all rights reserved.
 */
/*
 * Follow functions follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * $Id: _race.h,v 1.12.2.1 2011-03-29 10:12:17 oops Exp $
 */
#ifndef __RACE_H
#define __RACE_H

#include "libstring.h"

/* Define as const if the declaration of iconv() needs const. */
#define RacePrefix	"bq--"
#define COLOR		34

char * encode_race (char * domain, char * charset, int debug);
char * decode_race (char * domain, char * charset, int debug);
int permit_extension (char * tail);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
