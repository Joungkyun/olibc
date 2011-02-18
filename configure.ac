# Process this file with autoconf to produce a configure script.
# Configure template for oops C library
#
# $Id: configure.ac,v 1.23 2011-02-18 11:00:39 oops Exp $

#
AC_PREREQ(2.59)

AC_INIT([olibc], [0.1.3], [http://oops.org])
#AM_INIT_AUTOMAKE([1.8 gnits])
AC_CONFIG_SRCDIR([libstring.h])
AC_CONFIG_HEADER([olibc-config.h oc_type.h])
AC_PREFIX_DEFAULT([/usr/local])

AC_SUBST(SONAME_INFO)
AC_SUBST(SONAME_VERSION)
AC_SUBST(SONAME_MAJOR)
AC_SUBST(SONAME_MINOR)
AC_SUBST(SONAME_REVISION)

SONAME_MAJOR=0
SONAME_MINOR=1
SONAME_REVISION=2
SONAME_VERSION=$SONAME_MAJOR.$SONAME_MINOR.$SONAME_REVISION

SONAME_VALUE=$(($SONAME_MAJOR + $SONAME_MINOR))
SONAME_INFO="$SONAME_VALUE:$SONAME_REVISION:$SONAME_MINOR"

# for libidn
AC_SUBST(LIBIDN_NAME)
AC_SUBST(LIBIDN_VERSOIN)

LIBIDN_NAME=libidn
LIBIDN_VERSION=0.3.7

AC_ARG_WITH(iconv,
[  --with-iconv=PATH     compiled with iconv library.])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_AWK
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AC_PATH_PROG(MAKE, make)
if test ! -f "$ac_cv_path_MAKE"; then
  AC_MSG_ERROR(Can't not found make)
fi

AC_PATH_PROG(PERL, perl)
if test ! -f "$ac_cv_path_PERL"; then
  AC_MSG_ERROR(Can't not found perl)
fi

AC_PATH_PROG(HEAD, head)
if test ! -f "$ac_cv_path_HEAD"; then
  AC_MSG_ERROR(Can't not found head)
fi

AC_PATH_PROG(CP, cp)
if test ! -f "$ac_cv_path_CP"; then
  AC_MSG_ERROR(Can't not found cp)
fi

AC_PATH_PROG(AR, ar)
if test ! -f "$ac_cv_path_AR"; then
  AC_MSG_ERROR(Can't not found ar)
fi

AC_PATH_PROG(RANLIB, ranlib)
if test ! -f "$ac_cv_path_RANLIB"; then
  AC_MSG_ERROR(Can't not found ranlib)
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

if test "x$with_iconv" != "x"; then
  if test -f "$with_iconv" ; then
    chk_ext=`echo $with_iconv | ${SED} 's!.*\.\([^.]\+\)$!\1!g'`
    if test "$chk_ext" = "so" -a "$chk_ext" = "a"; then
      with_iconv=`echo $with_iconv | ${SED} 's!/[^/]*$!!g'`
      iconvlib="-L$with_iconv"
      dnl CHECK_RPATH(iconvlib, $with_iconv)

      iconvchk=`echo $with_iconv | ${GERP} '/lib$'`
      if test -n "$iconvchk"; then
        with_iconv=`echo $with_iconv | ${SED} 's!/lib$!!g'`
      fi
    fi
  elif test -d "$with_iconv" ; then
    if test -f $with_iconv/lib/libiconv.a -o -f $with_iconv/lib/libiconv.so; then
      iconvlib="-L$with_iconv/lib"
      dnl CHECK_RPATH(iconvlib, $with_iconv/lib)
    fi
  else
    iconvlib=
  fi

  LDFLAGS="$iconvlib $LDFLAGS"

  AC_CHECK_LIB(iconv, iconv_open,, [AC_MSG_ERROR([Can't not found iconv library (-liconv)])])

  # check of iconv.h
  if test -f "$with_iconv/include/iconv.h" -o "$with_iconv/include/giconv.h"; then
    inschk=`echo $CPPFLAGS | ${GREP} "I$with_iconv/include "`
    if test -z "$inschk"; then
      CPPFLAGS="-I$with_iconv/include $CPPFLAGS"
    fi
  fi
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([langinfo.h libintl.h locale.h sys/time.h errno.h ctype.h sys/type.h sys/stat.h pcre/pcre.h iconv.h stdbool.h])

if test "$GCC" = "yes"; then
	GCC_MAJOR_VERSION=`$CC --version | $HEAD -n 1 | $PERL -pe 's|^([[^0-9]]*)([[0-9]]+)\..*|\2|g'`
	if test $GCC_MAJOR_VERSION -ge 4; then
		CFLAGS="$CFLAGS -fvisibility=hidden"
	fi
fi

# Checks for typedefs, structures, and compiler characteristics for libidn.
AX_CREATE_STDINT_H(libidn/idn-int.h, sys/types.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_CHECK_SIZEOF(long)
#AC_CHECK_TYPES([ushort, uint, ulong, uchar])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([strtoull gettimeofday setlocale memmove])

AC_CONFIG_FILES([Makefile
                 man/Makefile
                 pcrelib/Makefile
                 libidn/Makefile])
AC_OUTPUT
