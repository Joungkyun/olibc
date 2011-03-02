#ifndef TEST_H
#define TEST_H

#define oc_test_banner(x) \
	{ \
		char buf[1024] = { 0, }; \
		sprintf (buf, "  + %s test", x); \
		printf ("%-40s .. ", buf); \
	}

#endif
