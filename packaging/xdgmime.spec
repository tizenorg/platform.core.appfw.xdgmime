Name:           xdgmime
Version:        0.0.12
Release:        2
License:        LGPLv2, AFLv2
Summary:        Pkg xdgmime
Group:          TO_BE/FILLED_IN
Source0:        xdgmime-%{version}.tar.gz
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

%build
%cmake . 

make %{?_smp_mflags}

%install
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/libxdgmime.so.*
%manifest xdgmime.manifest

%files devel
%defattr(-,root,root,-)
%{_includedir}/xdgmime.h
%{_libdir}/libxdgmime.so
%{_libdir}/pkgconfig/xdgmime.pc
