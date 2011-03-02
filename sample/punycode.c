#include <olibc/libstring.h>
#include <olibc/libidn.h>
#include "test.h"

int main (int argc, char ** argv) {
	UChar domain[32] = { 0, };
	UChar * punyc  = "xn---az-eb9lt87c3t8a66a.kr";

	UChar * puny, * unpuny;
	UInt l;

	// "한글-a밝혀z.kr" EUC-KR
	sprintf (
		domain,
		"%c%c%c%c-a%c%c%c%cz.kr",
		0xc7, 0xd1, 0xb1, 0xdb,
		0xb9, 0xe0, 0xc7, 0xf4
	);

	stringprep_locale_charset_cache = "EUC-KR";

	oc_test_banner ("convert_punycode encode");
	puny = strdup (convert_punycode(domain, 0, 1));
	printf ("%s\n", strcmp (puny, punyc) ? "failed" : "ok");

	stringprep_locale_charset_cache = "EUC-KR";

	oc_test_banner ("convert_punycode decode");
	unpuny = strdup (convert_punycode (punyc, 1, 1));
	printf ("%s\n", strcmp (domain, unpuny) ? "failed" : "ok");

	free (puny);
	free (unpuny);

	//printf ("################ %s\n", domain);
	//printf ("################ %s\n", unpuny);

	oc_test_banner ("convert_punycode_r encode");
	l = convert_punycode_r (domain, &puny, false, "EUC-KR");
	if ( l == 0 || puny == null )
		printf ("failed - mem allocate\n");
	else
		printf ("%s\n", strcmp (puny, punyc) ? "failed" : "ok");

	oc_test_banner ("convert_punycode_r decode");
	l = convert_punycode_r (puny, &unpuny, true, "EUC-KR");
	if ( l == 0 || unpuny == null )
		printf ("failed - mem allocate\n");
	else
		printf ("%s\n", strcmp (domain, unpuny) ? "failed" : "ok");

	free (unpuny);
	free (puny);

	return 0;
}
