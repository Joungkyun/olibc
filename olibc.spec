%define major 1
%define minor 0
%define rev   0
Summary: useful high level C library
Summary(ko): 유용한 하이레벨 C 라이브러리
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
%{_mandir}/man3/*.3.*
%{_mandir}/ko/man3/*.3.*

%changelog
* Thu Mar 21 2011 JoungKyun.Kim <http://oops.org> 1.0.0-1
- update version 1.0.0

