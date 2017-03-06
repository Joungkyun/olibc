# Process this file with autoconf to produce a configure script.
# Configure template for oops C library
#
# $Id$

#
AC_PREREQ(2.59)

AC_INIT([olibc], [1.0.2], [http://oops.org])
#AC_CONFIG_MACRO_DIR([./m4])

AC_CONFIG_AUX_DIR([build])
AM_INIT_AUTOMAKE([-Wall -Werror -Wno-override foreign no-dependencies])
AM_MAINTAINER_MODE
#AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([src/oc_type.h.in])
AC_CONFIG_HEADER([src/olibc-config.h src/oc_type.h])
AC_PREFIX_DEFAULT([/usr/local])

AC_DEFUN([AC_DATAROOTDIR_CHECKED])

AC_SUBST(SONAME_INFO)
AC_SUBST(SONAME_VERSION)
AC_SUBST(SONAME_MAJOR)
AC_SUBST(SONAME_MINOR)
AC_SUBST(SONAME_REVISION)

SONAME_MAJOR=1
SONAME_MINOR=0
SONAME_REVISION=0
SONAME_VERSION=$SONAME_MAJOR.$SONAME_MINOR.$SONAME_REVISION

SONAME_VALUE=$(($SONAME_MAJOR + $SONAME_MINOR))
SONAME_INFO="$SONAME_VALUE:$SONAME_REVISION:$SONAME_MINOR"

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AX_GL_VISIBILITY
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
#AM_GNU_GETTEXT(external)
#AM_GNU_GETTEXT_VERSION(0.17)
AM_ICONV

AC_PATH_PROG(PERL, perl)
if test ! -f "$ac_cv_path_PERL"; then
  AC_MSG_ERROR(Can't not found perl)
fi

AC_PATH_PROG(DATE, date)
if test ! -f "$ac_cv_path_DATE"; then
	AC_MSG_ERROR(Can't not found date)
fi

AC_SUBST(PACKAGE_DATE)
PACKAGE_DATE=$($DATE +"%a %b %d %Y")

AX_COMPAT_OPTION

# Checks for libraries.
AC_CHECK_LIB(rt, clock_gettime,,[])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([langinfo.h libintl.h locale.h time.h sys/time.h \
				  errno.h ctype.h sys/type.h sys/stat.h \
				  iconv.h stdbool.h inttypes.h locale.h stddef.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_C_VOLATILE
#AC_CHECK_SIZEOF(long)
#AC_CHECK_TYPES([ushort, uint, ulong, uchar])

# Checks for library functions.
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([strtoll gettimeofday setlocale memmove bcopy strerror])

AX_IDN
AX_PCRE

AX_STATIC_BINDIR

AC_CONFIG_FILES([Makefile
				 src/Makefile
				 test/Makefile
				 man/Makefile
				 scripts/Makefile
				 m4/olibc.m4
				 scripts/olibc.pc
				 scripts/olibc-config])
AC_OUTPUT
