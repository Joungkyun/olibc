# Process this file with autoconf to produce a configure script.
# Configure template for oops C library
#
# $Id: configure.ac,v 1.10 2004-02-04 05:38:46 oops Exp $

#
AC_PREREQ(2.53)

AC_INIT([olibc], [0.0.8], [http://www.oops.org])
AC_CONFIG_SRCDIR([libstring.h])
AC_CONFIG_HEADER([olibc-config.h])
AC_PREFIX_DEFAULT([/usr/local])

SONAME_MAJOR=0
SONAME_MINOR=0
SONAME_REVISION=1

AC_SUBST(SONAME_MAJOR)
AC_SUBST(SONAME_MINOR)
AC_SUBST(SONAME_REVISION)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_AWK
AC_PROG_MAKE_SET

AC_PATH_PROG(MAKE, make)
if test ! -f "$ac_cv_path_MAKE"; then
  AC_MSG_ERROR(Can't not found make)
fi

AC_PATH_PROG(PERL, perl)
if test ! -f "$ac_cv_path_PERL"; then
  AC_MSG_ERROR(Can't not found perl)
fi

AC_PATH_PROG(CP, cp)
if test ! -f "$ac_cv_path_CP"; then
  AC_MSG_ERROR(Can't not found cp)
fi

AC_PATH_PROG(AR, ar)
if test ! -f "$ac_cv_path_AR"; then
  AC_MSG_ERROR(Can't not found ar)
fi

AC_PATH_PROG(STRIP, strip)
if test ! -f "$ac_cv_path_STRIP"; then
  AC_MSG_ERROR(Can't not found strip)
fi

AC_PATH_PROG(LS, ls)
if test ! -f "$ac_cv_path_LS"; then
  AC_MSG_ERROR(Can't not found ls)
fi

AC_PATH_PROG(RM, rm)
if test ! -f "$ac_cv_path_RM"; then
  AC_MSG_ERROR(Can't not found rm)
fi

AC_PATH_PROG(MKDIR, mkdir)
if test ! -f "$ac_cv_path_MKDIR"; then
  AC_MSG_ERROR(Can't not found mkdir)
fi

AC_PATH_PROG(GZIP, gzip)
if test ! -f "$ac_cv_path_GZIP"; then
  AC_MSG_ERROR(Can't not found gzip)
fi

AC_PATH_PROG(MV, mv)
if test ! -f "$ac_cv_path_MV"; then
  AC_MSG_ERROR(Can't not found mv)
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([langinfo.h libintl.h locale.h stdlib.h string.h sys/time.h unistd.h errno.h ctype.h sys/type.h sys/stat.h pcre/pcre.h iconv.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([gettimeofday memmove memset setlocale strchr strdup])

AC_CONFIG_FILES([Makefile
                 man/Makefile
                 pcrelib/Makefile
                 libidn/Makefile])
AC_OUTPUT
