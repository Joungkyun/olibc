/* $Id: acconfig.h,v 1.5 2011-02-10 09:51:46 oops Exp $ */
#ifndef OLIBC_CONFIG_H
#define OLIBC_CONFIG_H

#define FILEBUF		4096
#define NETWBUF		1024

/* config.h:  a general config file */

@TOP@

@BOTTOM@

#define HAVE_LIBIDN 1

#define HAVE_LIBPCRE 1

#define EANBLE_NLS 1

#ifndef HAVE_SETLOCALE_H
#undef EANBLE_NLS
#endif

#ifndef HAVE_LIBINTL_H
#undef EANBLE_NLS
#endif

#endif /*  OLIBC_CONFIG_H */
