#ifndef OLIBC_CONFIG_H
#define OLIBC_CONFIG_H

#define FILEBUF		4096
#define NETWBUF		1024

#define SUCCESS		0
#define FAILURE		1
#define LSUCCESS	1
#define LFAILURE	0

/* config.h:  a general config file */

@TOP@

@BOTTOM@

#define EANBLE_NLS	1

#ifndef HAVE_SETLOCALE_H
#undef EANBLE_NLS
#endif

#ifndef HAVE_LIBINTL_H
#undef EANBLE_NLS
#endif

#endif /*  OLIBC_CONFIG_H */
