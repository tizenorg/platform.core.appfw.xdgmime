Name:       xdgmime
Summary:    Pkg xdgmime
Version:    0.0.12
Release:    1
Group:      TO_BE/FILLED_IN
License:    LGPLv2, AFLv2
Source0:    xdgmime-%{version}.tar.gz
Requires(post): /sbin/ldconfig, /bin/chown, /bin/chmod
Requires(postun): /sbin/ldconfig
BuildRequires:  cmake

%description
Package xdgmime 

%package devel
Summary:    Pkg xdgmime (devel)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Package xdgmime (devel)

%prep
%setup -q -n %{name}-%{version}

%build
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post
chown root:root /usr/lib/libxdgmime.so.1.1.0
chmod 644 /usr/lib/libxdgmime.so.1.1.0
/sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
/usr/lib/libxdgmime.so.*

%files devel
%defattr(-,root,root,-)
/usr/include/xdgmime.h
/usr/lib/libxdgmime.so
/usr/lib/pkgconfig/xdgmime.pc
