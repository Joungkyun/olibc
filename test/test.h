#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef TEST_H
#define TEST_H

int ret = 0;

/*
#define oc_test_banner(x) \
	{ \
		char buf[1024] = { 0, }; \
		sprintf (buf, "  + %s test", x); \
		printf ("%-40s .. ", buf); \
	}
*/
#define oc_test_banner(x) apiname = x;

#define Success printf ("PASS:   + %s\n", apiname)
#define Failure(x) \
	{ \
		printf ("FAIL:   + %s %s%s\n", apiname, strlen(x) ? "=> " : "", x); \
		ret++; \
	}

#define RESULT(x,...) \
	{ \
		printf ("%s:   + %s ", x == true ? "PASS" : "FAIL", apiname); \
		printf (__VA_ARGS__); \
	}

#endif
