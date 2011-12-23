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

AC_DEFUN([AX_IDN],
[
	AC_ARG_WITH(idn,
		[  --with-idn=PATH         prefix of idn [[default: /usr]]])

	if test "x$with_idn" != "xno"; then
		if test "x$with_idn" != "x"; then
			if test "x$with_idn" = "xyes"; then
				idn_prefix="/usr"
			else
				idn_prefix="$with_idn"
			fi
		else
			idn_prefix="/usr"
		fi

		idn_incdir="$idn_prefix/include"
		idn_cppflags="-I$idn_incdir"
		if test -f "$idn_prefix/lib64/libidn.so"; then
			idn_libdir="$idn_prefix/lib64"
		elif test -f "$idn_prefix/lib64/libidn.a"; then
			idn_libdir="$idn_prefix/lib64"
		else
			idn_libdir="$idn_prefix/lib"
		fi

		idn_ldflags="-L$idn_libdir"

		#
		# idn library check
		#
		idn_OLD_FLAGS="$LDFLAGS"
		LDFLAGS="$idn_ldflags"

		AC_CHECK_LIB(
			idn, stringprep, [
				AC_DEFINE(
					[HAVE_LIBIDN], 1,
					[Define to 1 if you have the `idn' library (-lidn).]
				)
				LIBS="-lidn $LIBS"
			],[
				AC_MSG_ERROR([Error.. you must need idn library!])
			]
		)

		LDFLAGS="$idn_OLD_LDFLAGS"

		ret_ldflags=0
		for check_ldflags in $LDFLAGS
		do
			test -z "$idn_prefix" && break

			if test "$check_ldflags" = "$idn_ldflags"; then
				ret_ldflags=1
				break
			fi
		done

		test $ret_ldflags -eq 0 && LDFLAGS="$idn_ldflags $LDFLAGS"

		#
		# idn header check
		#
		ret_cppflags=0
		for check_cppflags in $CPPFLAGS
		do
			test -z "$idn_prefix" && break

			if test "$check_cppflags" = "$idn_cppflags"; then
				ret_cppflags=1
				break
			fi
		done
		test $ret_cppflags -eq 0 && CPPFLAGS="$idn_cppflags $CPPFLAGS"
		AC_CHECK_HEADERS(stringprep.h)
		if test "$ac_cv_header_stringprep_h" != "yes"; then
			AC_MSG_ERROR([You must need stringprep.h header file!])
		fi
	fi
]
