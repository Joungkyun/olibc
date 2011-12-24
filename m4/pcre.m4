dnl Copyright (C) 2011 JoungKyun.Kim <http://oops.org>
dnl 
dnl This file is part of olibc
dnl 
dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU Lesser General Public License as published
dnl by the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl 
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl 
dnl You should have received a copy of the GNU Lesser General Public License
dnl along with this program.  If not, see <http://www.gnu.org/licenses/>.
dnl 
dnl $Id$

AC_DEFUN([AX_PCRE],
[
	AC_ARG_WITH(pcre,
		[  --with-pcre=PATH        prefix of pcre [[default: /usr]]])

	if test "x$with_pcre" != "xno"; then
		if test "x$with_pcre" != "x"; then
			if test "x$with_pcre" = "xyes"; then
				pcre_prefix="/usr"
			else
				pcre_prefix="$with_pcre"
			fi
		else
			pcre_prefix="/usr"
		fi

		pcre_incdir="$pcre_prefix/include"
		pcre_cppflags="-I$pcre_incdir -I$pcre_incdir/pcre"
		if test -f "$pcre_prefix/lib64/libpcre.so"; then
			pcre_libdir="$pcre_prefix/lib64"
		elif test -f "$pcre_prefix/lib64/libpcre.a"; then
			pcre_libdir="$pcre_prefix/lib64"
		else
			pcre_libdir="$pcre_prefix/lib"
		fi

		pcre_ldflags="-L$pcre_libdir"

		#
		# pcre library check
		#
		pcre_OLD_FLAGS="$LDFLAGS"
		LDFLAGS="$pcre_ldflags"

		AC_CHECK_LIB(
			pcre, pcre_compile, [
				AC_DEFINE(
					[HAVE_LIBIDN], 1,
					[Define to 1 if you have the `pcre' library (-lpcre).]
				)
				#LIBS="-lpcre $LIBS"
			],[
				AC_MSG_ERROR([Error.. you must need pcre library!])
			]
		)

		LDFLAGS="$pcre_OLD_LDFLAGS"

		ret_ldflags=0
		for check_ldflags in $LDFLAGS
		do
			test -z "$pcre_prefix" && break

			if test "$check_ldflags" = "$pcre_ldflags"; then
				ret_ldflags=1
				break
			fi
		done

		test $ret_ldflags -eq 0 && LDFLAGS="$pcre_ldflags $LDFLAGS"

		#
		# pcre header check
		#
		ret_cppflags=0
		for check_cppflags in $CPPFLAGS
		do
			test -z "$pcre_prefix" && break

			if test "$check_cppflags" = "$pcre_cppflags"; then
				ret_cppflags=1
				break
			fi
		done
		test $ret_cppflags -eq 0 && CPPFLAGS="$pcre_cppflags $CPPFLAGS"
		AC_CHECK_HEADERS(pcre.h)
		if test "$ac_cv_header_pcre_h" != "yes"; then
			AC_MSG_ERROR([You must need pcre.h header file!])
		fi
	fi
])
