#
#                       Written by JoungKyun.Kim
#            Copyright (c) 2011 JoungKyun.Kim <http://oops.org>
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

AM_CFLAGS = -Wall
AM_CPPFLAGS = -I$(top_srcdir)
LIBS += $(LTLIBICONV)

noinst_LTLIBRARIES = libbuiltin_idn.la
libbuiltin_idn_la_SOURCES = config.h gunicomp.h gunidecomp.h \
	idna.h punycode.h stringprep.h \
	idna.c nfkc.c profiles.c punycode.c stringprep.c rfc3454.c \
	toutf8.c

BUILT_SOURCES = config.h idn-int.h

config.h: $(top_srcdir)/src/oc_common.h
	$(AM_V_GEN)rm -f ./config.h || true
	$(AM_V_GEN)$(LN_S) -f $(top_srcdir)/src/oc_common.h ./config.h

DISTCLEANFILES = $(BUILT_SOURCES)
