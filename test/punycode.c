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

/* $Id$ */

#include <olibc/libstring.h>
#include <olibc/libidn.h>
#include "test.h"

int main (int argc, char ** argv) {
	char	domain[32] = { 0, },
			* punyc  = "xn---az-eb9lt87c3t8a66a.kr",
			* puny,
			* unpuny,
			* apiname;

	// "한글-a밝혀z.kr" EUC-KR
	sprintf (
		domain,
		"%c%c%c%c-a%c%c%c%cz.kr",
		0xc7, 0xd1, 0xb1, 0xdb,
		0xb9, 0xe0, 0xc7, 0xf4
	);

	//stringprep_locale_charset_cache = "EUC-KR";


	oc_test_banner ("convert_punycode encode");
	puny = convert_punycode (domain, "EUC-KR");
	if ( puny == null )
		Failure ("mem allocate");
	else {
		if ( ! strcmp (puny, punyc) )
			Success;
		else
			Failure ("");
	}

	oc_test_banner ("convert_punycode decode");
	unpuny = convert_punycode (puny, "EUC-KR");
	if ( unpuny == null )
		Failure ("mem allocate");
	else {
		if ( ! strcmp (domain, unpuny) )
			Success;
		else
			Failure ("");
	}

	free (unpuny);
	free (puny);

	return 0;
}
