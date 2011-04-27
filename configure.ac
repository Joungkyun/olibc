# Process this file with autoconf to produce a configure script.
# Configure template for oops C library
#
# $Id: configure.ac,v 1.30.2.2 2011-03-29 18:42:12 oops Exp $

#
AC_PREREQ(2.59)

AC_INIT([olibc], [0.1.4], [http://oops.org])
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
SONAME_REVISION=4
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

#AC_ARG_WITH(pcre,
#[  --with-pcre=PATH      compiled with pcre frontend api support. [[default: /usr]]])

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

#if test "x$with_pcre" != "x"; then
#  SEARCHPATH="$with_pcre /usr /usr/local /opt"
#
#  for i in $SEARCHPATH
#  do
#    if test -f "$i/lib/libpcre.so" -o -f "$i/lib/libpcre.a"; then
#      pcrelib="L$i/lib"
#    elif test -f "$i/lib64/libpcre.so" -o -f "$i/lib64/libpcre.a"; then
#      pcrelib="L$i/lib64"
#    fi
#  done
#
#  if test -z "$pcrelib"; then
#    if test -f "/lib/libpcre.so" -o -f "/lib/libpcre.a"; then
#      pcrelib="L/lib"
#    elif test -f "/lib64/libpcre.so" -o -f "/lib64/libpcre.a"; then
#      pcrelib="L/lib64"
#    fi
#  fi
#
#  libchk=`echo $LDFLAGS | ${GREP} "${pcrelib} "`
#  if test -z "$libchk"; then
#    LDFLAGS="-$pcrelib $LDFLAGS"
#  fi
#
#
#  for i in $SEARCHPATH
#  do
#    if test -f "$i/include/pcre/pcre.h"; then
#      pcreinc="I$i/include/pcre"
#    elif test -f "$i/include/pcre.h"; then
#      pcreinc="I$i/include"
#    fi
#  done
#
#  libchk=`echo $LDFLAGS | ${GREP} "${pcreinc} "`
#  if test -z "$pcreinc"; then
#    CPPLFAGS="-${pcreinc} $CPPLFAGS"
#  fi
#fi
#
#AC_CHECK_LIB(
#  pcre, pcre_compile,,
#  [AC_MSG_ERROR([Can't not found pcre library (-lpcre)])]
#)

AC_CHECK_LIB(rt, clock_gettime,,[])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([langinfo.h libintl.h locale.h time.h sys/time.h \
				  errno.h ctype.h sys/type.h sys/stat.h pcre/pcre.h \
				  iconv.h stdbool.h])

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
AC_CHECK_FUNCS([strtoll gettimeofday setlocale memmove bcopy strerror])

AC_CONFIG_FILES([Makefile
                 man/Makefile
				 sample/Makefile
                 pcrelib/Makefile
                 libidn/Makefile])
AC_OUTPUT
