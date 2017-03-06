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


COMPAT_VERSION = @COMPAT_VERSION@

bin_SCRIPTS = olibc-config ogc-config oc-config
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = olibc.pc ogc.pc oc.pc

DISTCLEANFILES = *.pc *-config
BUILT_SOURCES = oc.in ogc.in

oc.in:
	cp -af olibc-config oc-config
	cp -af olibc.pc oc.pc
	$(PERL) -pi -e "s/prog=\"olibc/prog=\"oc/g" oc-config

	$(PERL) -pi -e "s!-lolibc$(COMPAT_VERSION)!-loc$(COMPAT_VERSION) $(LDFLAGS) $(LIBS)!g" oc.pc
	$(PERL) -pi -e "s/libolibc$(COMPAT_VERSION)/liboc$(COMPAT_VERSION)/g" oc.pc

ogc.in:
	cp -af olibc-config ogc-config
	cp -af olibc.pc ogc.pc
	$(PERL) -pi -e "s/prog=\"olibc/prog=\"ogc/g" ogc-config

	$(PERL) -pi -e "s!-lolibc$(COMPAT_VERSION)!-logc$(COMPAT_VERSION) -loc$(COMPAT_VERSION) $(LDFLAGS) $(LIBS)!g" ogc.pc
	$(PERL) -pi -e "s/libolibc$(COMPAT_VERSION)/libogc$(COMPAT_VERSION)/g" ogc.pc

if COMPAT
install-exec-hook:
	for i in olibc ogc oc; do \
		desti="$$i"$(COMPAT_VERSION); \
		mv $(DESTDIR)$(bindir)/$$i-config $(DESTDIR)$(bindir)/$$desti-config; \
	done

install-data-hook:
	for i in olibc ogc oc; do \
		desti="$$i"$(COMPAT_VERSION); \
		mv $(DESTDIR)$(pkgconfigdir)/$$i.pc \
			$(DESTDIR)$(pkgconfigdir)/$$desti.pc; \
	done
endif
