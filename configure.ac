# Release version
m4_define([package_version_major],[1])
m4_define([package_version_minor],[0])
m4_define([package_version_micro],[1])
m4_define([package_repository],[https://github.com/astrobit/xquiz])
m4_define([package_auth_email],[bwmulligan@astronaos.com])

# API version
m4_define([api_version],package_version_major)

AC_INIT(xquiz,package_version_major.package_version_minor.package_version_micro,package_auth_email,xquiz,package_repository)
AC_CONFIG_MACRO_DIR([m4])

LT_INIT

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

MOD_PROG_ENVMOD
MOD_INSTALLDIR

AM_INIT_AUTOMAKE

#AC_PROG_RANLIB

AC_LANG(C++)

AC_PROG_CXX

#AX_CXX_COMPILE_STDCXX(11,noext,mandatory) #use c++11 standard, required in a variety of places

AX_PTHREAD #needed for anything using EngineMk1
PKG_CHECK_MODULES([xml2],[libxml-2.0],[],[AC_MSG_ERROR([" Could not find libxml2 development package"])])


AC_SUBST([PACKAGE_VERSION_MAJOR],package_version_major)
AC_SUBST([PACKAGE_VERSION_MINOR],package_version_minor)
AC_SUBST([PACKAGE_VERSION_MICRO],package_version_micro)
AC_SUBST([PACKAGE_REPOSITORY],package_repository)
AC_SUBST([PACKAGE_AUTH_EMAIL],package_auth_email)
AC_SUBST([API_VERSION],api_version)

AC_SUBST([LIB_NAME],[xquiz])
AC_SUBST([LIB_DESCRIPTION],["XML quiz library and quiz generator"])
AC_SUBST([LIB_REQUIRES],["libxml-2.0"])



AC_CONFIG_FILES([Makefile $PACKAGE_NAME-$API_VERSION.pc:lib.pc.in],[],[PACKAGE_NAME='$PACKAGE_NAME' API_VERSION='$API_VERSION'])

AC_OUTPUT


