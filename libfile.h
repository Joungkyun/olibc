/* $Id: libfile.h,v 1.5 2009-10-29 14:01:36 oops Exp $ */
#ifndef LIBFILE_H
#define LIBFILE_H

#define _IS_NCHK	0
#define _IS_FILE	1
#define _IS_DIR		2
#define _IS_SLINK	3
#define _IS_CDEV	4
#define _IS_BDEV	5
#define _IS_FIFO	6
#define _IS_SOCK	7

/* file_exists follows BPL License v.1 <http://devel.oops.org/document/bpl>
 * check whether exists path
 * path => check file
 * int  =>
 *         _IS_NCHK  : whether exists path
 *         _IS_FILE  : whether path is regular file
 *         _IS_DIR   : whether path is directory
 *         _IS_SLINK : whether path is symbolic link
 *         _IS_CDEV  : whether path is character device
 *         _IS_BDEV  : whether path is block device
 *         _IS_FIFO  : whether path is FIFO
 *         _SI_SOCK  : whether path is socket file */
int file_exists (const char *path, int mode);

/* fileread follow BPL License v.1 <http://devel.oops.org/document/bpl>
 * read file
 *
 * path  => path of read file
 * return values must allocated memory */
char * fileread (char * path);

/* writefile follow BPL License v.1 <http://devel.oops.org/document/bpl>
 * write file
 *
 * filename => write file name
 * str      => write context
 * mode     => write mode
 *             0 : new file
 *             1 : attach file */
int writefile(char *filename, char *str, int mode);

/* writefile follow BPL License v.1 <http://devel.oops.org/document/bpl>
 * write file
 *
 * path     => search path
 */
char * realpath_r (char *path);

#endif
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
