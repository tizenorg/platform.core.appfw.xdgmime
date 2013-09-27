Name:           xdgmime
Version:        0.0.12
Release:        3
License:        LGPLv2, AFLv2
Summary:        Pkg xdgmime
Group:          TO_BE/FILLED_IN
Source0:        xdgmime-%{version}.tar.gz
Source1001: 	xdgmime.manifest
BuildRequires:  cmake

%description
Package xdgmime

%package devel
Summary:        Pkg xdgmime (devel)
Group:          Development/Libraries
Requires:       %{name} = %{version}

%description devel
Package xdgmime (devel)

%prep
%setup -q
cp %{SOURCE1001} .

%build
%cmake . 

make %{?_smp_mflags}

%install
%make_install
mkdir -p %{buildroot}/usr/share/license
install COPYING %{buildroot}/usr/share/license/%{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libxdgmime.so.*
%manifest xdgmime.manifest
/usr/share/license/%{name}

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/xdgmime.h
%{_libdir}/libxdgmime.so
%{_libdir}/pkgconfig/xdgmime.pc
