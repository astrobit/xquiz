AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I./include

lib_LTLIBRARIES = libxquiz.la
libxquiz_la_SOURCES = src/libquiz.cpp
libxquiz_la_CXXFLAGS = ${xml2_CFLAGS}
libxquiz_la_LDFLAGS = -version-info 0:0:0

bin_PROGRAMS = xquiz-gen xquiz-bank

xquiz_gen_SOURCES = src/testgen.cpp
xquiz_gen_CXXFLAGS = ${xml2_CFLAGS}
xquiz_gen_LDADD = -L. -lxquiz ${xml2_LIBS}

xquiz_bank_SOURCES = src/printbank.cpp 
xquiz_bank_CXXFLAGS = ${xml2_CFLAGS}
xquiz_bank_LDADD = -L. -lxquiz ${xml2_LIBS}

include_HEADERS = include/xquiz.h

# Install the pkg-config file; the directory is set using
# PKG_INSTALLDIR in configure.ac.
pkgconfig_DATA = ${PACKAGE_NAME}-${API_VERSION}.pc

EXTRA_DIST = LICENSE README.md ${PACKAGE_NAME}-${API_VERSION}.pc
CLEANFILES = ${PACKAGE_NAME}-${API_VERSION}.pc

