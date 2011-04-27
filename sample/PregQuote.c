#include <olibc/libpcre.h>
#include "test.h"

char * source[2] = {
	"asdf/asdf",
	"asdf/asdf.$^*(){}.$^+*"
};

char * answer[2] = {
	"as\\df\\/as\\df",
	"asdf/asdf\\.\\$\\^\\*\\(\\)\\{\\}\\.\\$\\^\\+\\*"
};

int main (int argc, char ** argv) {
	int i = 0;
	char * buf;

	oc_test_banner ("preg_quote");
	buf = preg_quote (source[i], "/d");
	printf ("%s\n", strcmp (buf, answer[i]) ? "failed" : "ok");
	ofree (buf);
	i++;

	oc_test_banner ("preg_quote overflow");
	buf = preg_quote (source[i], null);
	printf ("%s\n", strcmp (buf, answer[i]) ? "failed" : "ok");
	ofree (buf);

	return 0;
}
