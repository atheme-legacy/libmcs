dnl                                               -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([libmcs], [0.8.0], [bugs+libmcs@atheme.org])
AC_CONFIG_SRCDIR([src/libmcs/mcs_backends.c])
AC_CONFIG_HEADER([src/libmcs/mcs_config.h])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_MAKE_SET

test x"$GCC" = x"yes" && CFLAGS="$CFLAGS -pipe -Wall -O2"

dnl XXX workaround
PACKAGE="libmcs"
AC_SUBST([PACKAGE])

dnl Checks for libraries.
BUILDSYS_SHARED_LIB

PKG_CHECK_MODULES([MOWGLI], [libmowgli >= 0.7.0], [], [AC_MSG_ERROR([libmowgli 0.7.0 or newer required])])

dnl Output files
AC_CONFIG_FILES([
buildsys.mk
extra.mk
libmcs.pc
])
AC_OUTPUT
BUILDSYS_TOUCH_DEPS

dnl Print out results
cat << _EOF_

Now type "make" to build, and "make install" to install.
Thank you for using mcs.

_EOF_
