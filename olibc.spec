%define major 0
%define minor 1
%define rev   3
Summary: useful high level C library
Summary(ko): ������ ���̷��� C ���̺귯��
Name: olibc
Version: %{major}.%{minor}.%{rev}
Release: 1
Epoch: 21
License: BPL/LGPL
Group: System Environment/Libraries
Source0: http://mirror.oops.org/pub/oops/olibc/%{name}-%{version}.tar.bz2
URL: http://devel.oops.org
Buildroot: /var/tmp/%{name}-%{version}-root

Packager: JoungKyun.Kim <http://oops.org>
Vendor: OOPS Development ORG

%description
The olibc (oops C library) is useful high level c library. You can make 
easy code with olibc

%description -l ko
olibc �� ���� ������ C ���̺귯���̴�. �� ���̺귯���� �̿��Ͽ� C �ڵ带
���� �ۼ��� �� �ִ�.

%package devel
Summary: Header and object files for development using oops C libraries.
Summary(ko): olibc �� �̿��Ͽ� �����ϱ� ���� header ���ϰ� ���� ���ϵ�
Group: ystem Environment/Libraries

%description devel
The olibc-devel package contains the header and object files necessary
for developing programs which use the olibc libraries.

%description devel -l ko
olibc-devel ��Ű���� olibc ���̺귯���� �̿��Ͽ� ������ �ϱ� ���� ����
�ڵ�� ��� ���ϵ��� �����ϰ� �ִ�.

%prep
%{__rm} -rf %{buildroot}
%setup -q

%build
CFLAGS="$RPM_OPT_FLAGS" \
CXXFLAGS="$RPM_OPT_FLAGS" \
FFLAGS="$RPM_OPT_FLAGS" \
%{configure} --includedir=%{_includedir}/olibc
%{__make}

%install
make DESTDIR=%{buildroot} install

%clean
%{__rm} -rf %{buildroot}

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%defattr(0755,root,root)
%{_libdir}/*.so.*

%files devel
%defattr(0644,root,root,0755)
%{_bindir}/oc-config
%{_bindir}/ogc-config
%{_bindir}/olibc-config
%{_libdir}/*.so
%{_libdir}/*.a
%{_libdir}/*.la
%{_includedir}/olibc/*.h
%{_mandir}/ko/man1/*.3.*

%changelog
* Thu Mar 21 2011 JoungKyun.Kim <http://oops.org> 0.1.3-1
- update version 0.1.3

* Thu Sep  1 2005 JoungKyun.Kim <http://oops.org> 0.1.2-1
- fixed  CAN-2005-2491 in pcrelib under 0.1.1
- update 0.1.2

* Mon Aug  9 2004 JoungKyun.Kim <http://oops.org> 0.1.1-1
- update 0.1.1

* Fri Feb 20 2004 JoungKyun.Kim <http://oops.org> 0.1.0-1
- update 0.1.0

* Thu Feb  5 2004 JoungKyun.Kim <http://oops.org> 0.0.8-1
- update 0.0.8

* Fri Nov 10 2003 JoungKyun.Kim <http://oops.org> 0.0.7-1
- update 0.0.7

* Fri Nov 10 2003 JoungKyun.Kim <http://oops.org> 0.0.6-1
- update 0.0.6

* Fri Nov  7 2003 JoungKyun.Kim <http://oops.org> 0.0.5-1
- update 0.0.5

* Sat Sep 27 2003 JoungKyun.Kim <http://oops.org> 0.0.4-2
- added official patch 1

* Fri Sep 26 2003 JoungKyun.Kim <http://oops.org> 0.0.4-1
- update 0.0.4

* Thu Sep 25 2003 JoungKyun.Kim <http://oops.org> 0.0.3-3
- fixed tmp variable reset size

* Sat Sep 20 2003 JoungKyun.Kim <http://oops.org> 0.0.3-2
- added official patch 1
  this bug is seriously --; (str2long, str2double)

* Sat Sep 20 2003 JoungKyun.Kim <http://oops.org> 0.0.3-1
- update 0.0.3

* Thu Sep 18 2003 JoungKyun.Kim <http://oops.org> 0.0.2-5
- added second official patch

* Sun Sep 14 2003 JoungKyun.Kim <http://oops.org> 0.0.2-4
- fixed lib_preg_match bug

* Wed Sep 10 2003 JoungKyun.Kim <http://oops.org> 0.0.2-3
- changed don't memory free return value of convert_punyconv

* Tue Sep  9 2003 JoungKyun.Kim <http://oops.org> 0.0.2-2
- fixed trash print

* Tue Sep  9 2003 JoungKyun.Kim <http://oops.org> 0.0.2-1
- 0.0.2 release
- added internal libidn
- added internal pcrelib

* Tue Sep  9 2003 JoungKyun.Kim <http://oops.org> 0.0.1-1
- first build

