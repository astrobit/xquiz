Name:           xquiz
Version:        1.0.3
Release:        1%{?dist}
Summary:        Quiz form generating software

License:        LGPLv3+
URL:            https://github.com/astrobit/xquiz
Source0:        https://www.astronaos.com/xquiz/1.0.3/xquiz-1.0.3.tar.gz

#The versions listed for requires here are based on what is on my developer
# system; the code may work with earlier versions of each of these libraries.
# If you have a system with an older version of one of these libraries and
# cannot update it, please try building the source; if you find that it
# compiles correctly, let me know and I will make the update to the RPM, or
# please generate a pull request on github
BuildRequires:  gcc >= 6.0 
#Requires:       

%description
Application for generating multiple choice quizzes. Output is as a LaTeX quiz 
document. Makes use of a user specified question bank, then will generate one
or more quiz forms with the specified questions, with the question order
and answer orders randomized. Also provides a key and solution for each form.

%package        devel
Summary:        Development needs for building apps that use the xquiz library
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}

%description    devel
This package contains headers and libraries required to build applications that
use the xquiz library.


%prep
%autosetup


%build
%configure
%make_build


%install
rm -rf $RPM_BUILD_ROOT
%make_install


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
%license LICENSE
%doc README.md
%{_bindir}/*
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
#%%doc examples
%license LICENSE
%doc README.md
%{_includedir}/*.h
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/*.so
%{_libdir}/pkgconfig/*.pc


%changelog
* Thu Dec 12 2019 Brian W. Mulligan <bwmulligan@astronaos.com>
- Add package tikz to tex files
* Thu Oct 4 2018 Brian W. Mulligan <bwmulligan@astronaos.com>
- Initial RPM release


