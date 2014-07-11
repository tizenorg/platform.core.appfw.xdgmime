Name:           xdgmime
Version:        0.0.12
Release:        0
License:        LGPL-2.0 and AFL-2.0
Summary:        Querying information about file type
Group:          Applications/Other
Source0:        xdgmime-%{version}.tar.gz
Source1001:     xdgmime.manifest
BuildRequires:  cmake

%description
Querying information about file type package.

%package devel
Summary:        Querying information about file type (devel)
Group:          Development/Libraries
Requires:       %{name} = %{version}

%description devel
Querying information about file type (devel) package.

%prep
%setup -q
cp %{SOURCE1001} .

%build
%cmake . 
%__make %{?_smp_mflags}

%install
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libxdgmime.so.*
%license COPYING

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/xdgmime.h
%{_libdir}/libxdgmime.so
%{_libdir}/pkgconfig/xdgmime.pc
