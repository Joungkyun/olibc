%define major 0
%define minor 1
%define rev   1
Summary: useful high level C library
Summary(ko): 유용한 하이레벨 C 라이브러리
Name: olibc
Version: %{major}.%{minor}.%{rev}
Release: 1
Epoch: 18
Copyright: BPL/LGPL
Group: System Environment/Libraries
Source0: ftp://mirror.oops.org/pub/Linux/OOPS/Source/olibc/%{name}-%{version}.tar.bz2
URL: http://devel.oops.org
Buildroot: /var/tmp/%{name}-%{version}-root

Packager: JoungKyun Kim <http://www.oops.org>
Vendor: OOPS Development ORG

%description
The olibc (oops C library) is useful high level c library. You can make 
easy code with olibc

%description -l ko
olibc 는 아주 유용한 C 라이브러리이다. 이 라이브러리를 이용하여 C 코드를
쉽게 작성할 수 있다.

%package devel
Summary: Header and object files for development using oops C libraries.
Summary(ko): olibc 를 이용하여 개발하기 위한 header 파일과 목적 파일들
Group: ystem Environment/Libraries

%description devel
The olibc-devel package contains the header and object files necessary
for developing programs which use the olibc libraries.

%description devel -l ko
olibc-devel 패키지는 olibc 라이브러리를 이용하여 개발을 하기 위한 목적
코드와 헤더 파일들을 포함하고 있다.

%prep
rm -rf $RPM_BUILD_ROOT
%setup -q

%build
CFLAGS="$RPM_OPT_FLAGS" \
CXXFLAGS="$RPM_OPT_FLAGS" \
FFLAGS="$RPM_OPT_FLAGS" \
./configure i686-annyung-linux \
            --prefix=/usr \
            --exec-prefix=/usr \
            --bindir=/usr/bin \
            --sbindir=/usr/sbin \
            --sysconfdir=/etc \
            --datadir=/usr/share \
            --includedir=/usr/include/olibc \
            --libdir=/usr/lib \
            --libexecdir=/usr/libexec \
            --localstatedir=/var \
            --sharedstatedir=/usr/com \
            --mandir=/usr/share/man \
            --infodir=/usr/share/info
make

%install
make DESTDIR=${RPM_BUILD_ROOT} install

%clean
rm -rf $RPM_BUILD_ROOT

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%defattr(0755,root,root)
%{_bindir}/olibc-config
%{_libdir}/libolibc.so.%{version}
%{_libdir}/libogc.so.%{version}
%{_libdir}/liboc.so.%{version}
%{_libdir}/libolibc.so.%{major}
%{_libdir}/libogc.so.%{major}
%{_libdir}/liboc.so.%{major}
%{_mandir}/ko/man1/*.1.*

%files devel
%defattr(0644,root,root,0755)
%{_libdir}/libolibc.so
%{_libdir}/libogc.so
%{_libdir}/liboc.so
%{_libdir}/libolibc.a
%{_libdir}/libogc.a
%{_libdir}/liboc.a
%{_includedir}/olibc/*.h

%changelog
* Mon Aug  9 2004 JoungKyun Kim <http://www.oops.org> 0.1.1-1
- update 0.1.1

* Fri Feb 20 2004 JoungKyun Kim <http://www.oops.org> 0.1.0-1,17
- update 0.1.0

* Thu Feb  5 2004 JoungKyun Kim <http://www.oops.org> 0.0.8-1,16
- update 0.0.8

* Fri Nov 10 2003 JoungKyun Kim <http://www.oops.org> 0.0.7-1,15
- update 0.0.7

* Fri Nov 10 2003 JoungKyun Kim <http://www.oops.org> 0.0.6-1,14
- update 0.0.6

* Fri Nov  7 2003 JoungKyun Kim <http://www.oops.org> 0.0.5-1,13
- update 0.0.5

* Sat Sep 27 2003 JoungKyun Kim <http://www.oops.org> 0.0.4-2,11
- added official patch 1

* Fri Sep 26 2003 JoungKyun Kim <http://www.oops.org> 0.0.4-1,10
- update 0.0.4

* Thu Sep 25 2003 JoungKyun Kim <http://www.oops.org> 0.0.3-3,9
- fixed tmp variable reset size

* Sat Sep 20 2003 JoungKyun Kim <http://www.oops.org> 0.0.3-2,8
- added official patch 1
  this bug is seriously --; (str2long, str2double)

* Sat Sep 20 2003 JoungKyun Kim <http://www.oops.org> 0.0.3-1,7
- update 0.0.3

* Thu Sep 18 2003 JoungKyun Kim <http://www.oops.org> 0.0.2-5,6
- added second official patch

* Sun Sep 14 2003 JoungKyun Kim <http://www.oops.org> 0.0.2-4,5
- fixed lib_preg_match bug

* Wed Sep 10 2003 JoungKyun Kim <http://www.oops.org> 0.0.2-3,4
- changed don't memory free return value of convert_punyconv

* Tue Sep  9 2003 JoungKyun Kim <http://www.oops.org> 0.0.2-2,3
- fixed trash print

* Tue Sep  9 2003 JoungKyun Kim <http://www.oops.org> 0.0.2-1,2
- 0.0.2 release
- added internal libidn
- added internal pcrelib

* Tue Sep  9 2003 JoungKyun Kim <http://www.oops.org> 0.0.1-1,1
- first build

