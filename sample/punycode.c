#include <olibc/libstring.h>
#include <olibc/libidn.h>
#include "test.h"

int main (int argc, char ** argv) {
	char	domain[32] = { 0, };
	char	* punyc  = "xn---az-eb9lt87c3t8a66a.kr",
			* puny,
			* unpuny;
	UInt l;

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
	if ( l == 0 || puny == null )
		printf ("failed - mem allocate\n");
	else
		printf ("%s\n", strcmp (puny, punyc) ? "failed" : "ok");

	oc_test_banner ("convert_punycode decode");
	unpuny = convert_punycode (puny, "EUC-KR");
	if ( unpuny == null )
		printf ("failed - mem allocate\n");
	else
		printf ("%s\n", strcmp (domain, unpuny) ? "failed" : "ok");

	free (unpuny);
	free (puny);

	return 0;
}
