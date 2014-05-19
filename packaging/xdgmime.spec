Name:           xdgmime
Version:        0.0.12
Release:        0
License:        LGPL-2.0 and AFL-2.0
Summary:        Pkg xdgmime
Group:          Applications/Other
Source0:        xdgmime-%{version}.tar.gz
Source1001:     xdgmime.manifest
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

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libxdgmime.so.*
%manifest xdgmime.manifest
%license COPYING

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/xdgmime.h
%{_libdir}/libxdgmime.so
%{_libdir}/pkgconfig/xdgmime.pc
