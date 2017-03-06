#
#                       Written by JoungKyun.Kim
#            Copyright (c) 2017 JoungKyun.Kim <http://oops.org>
#
# -----------------------------------------------------------------------------
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright notice,
#       this list of conditions and the following disclaimer.
#
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
#     * Neither the name of JoungKyun.Kim nor the url of oops.org
#       nor the names of their contributors may be used to endorse or
#       promote products derived from this software without specific prior
#       written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# -----------------------------------------------------------------------------
# This file is part of olibc.
#

# $Id$
#

DISTCLEANFILES =
BUILT_SOURCES =

EXTRA_DIST = COPYING.BSD COPYING.LGPL Changes LICENCE olibc.spec

if ! COMPAT
docdir = $(datadir)/doc/$(PACKAGE)-$(VERSION)
dist_doc_DATA = COPYING.BSD COPYING.LGPL INSTALL README LICENCE
endif

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src man test scripts

BUILT_SOURCES += olibcdir

olibcdir:
	if test ! -L olibc; then \
		$(AM_V_GEN)$(LN_S) src olibc; \
	fi

DISTCLEANFILES += olibc

m4dir = $(datadir)/aclocal
dist_m4_DATA = m4/olibc@COMPAT_VERSION@.m4

if COMPAT
BUILT_SOURCES += m4/olibc@COMPAT_VERSION@.m4
DISTCLEANFILES += olibc m4/olibc@COMPAT_VERSION@.m4
m4/olibc@COMPAT_VERSION@.m4:
	mv m4/olibc.m4 m4/olibc@COMPAT_VERSION@.m4
endif

dist-hook:
	rm -f $(distdir)/m4/olibc.m4
	$(PERL) -pi -e \
		"s/\@PACKAGE_VERSION\@/$(VERSION)/g" \
		 $(distdir)/$(PACKAGE).spec
	$(PERL) -pi -e \
		"s/\@PACKAGE_DATE\@/$(PACKAGE_DATE)/g" \
		$(distdir)/$(PACKAGE).spec

