#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([libmcs], [0.6.0], [bugs+libmcs@atheme.org])
AC_CONFIG_SRCDIR([src/libmcs/mcs_loader.c])
AC_CONFIG_HEADER([src/libmcs/mcs_config.h])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# Checks for programs.
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_ISC_POSIX

# XXX workaround
PACKAGE="libmcs"
AC_SUBST(PACKAGE)

# Checks for libraries.
AM_SHARED_LIB
AM_DL

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h string.h unistd.h locale.h stdarg.h sys/types.h sys/stat.h errno.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_CHECK_FUNCS([strstr], , [AC_ERROR([strstr is required for the plugin loader, sorry.])])
AC_CHECK_FUNCS([memset setlocale strcasecmp strchr strdup strerror strtol strtod])
AC_FUNC_STAT

PKG_CHECK_MODULES(MOWGLI, [libmowgli >= 0.4.0], [], [AC_MSG_ERROR([libmowgli 0.4.0 or greater required])])

# Check for optional features.
AC_ARG_ENABLE(gconf,
    [  --enable-gconf          enable GConf support (default=disabled)],
    [enable_gconf=$enableval],
    [enable_gconf="no"]
)

if test "$enable_gconf" = "yes"; then
    PKG_CHECK_MODULES(GCONF, [gconf-2.0 >= 2.6.0], ,
        [AC_MSG_ERROR([Cannot find GConf])]
    )
fi

if test "$enable_gconf" = "yes"; then
	GCONF_BUILD="gconf"
fi

AC_SUBST(GCONF_BUILD)

AC_ARG_ENABLE(kconfig,
    [  --enable-kconfig        enable KDE KConfig support (default=disabled)],
    [enable_kconfig=$enableval],
    [enable_kconfig="no"]
)

if test "$enable_kconfig" = "yes"; then
   AC_PROG_CXX
   # XXX - is this Gentoo-specific?
   CXXFLAGS="$CXXFLAGS -I/usr/kde/3.5/include -I/usr/qt/3/include"
   LDFLAGS="$LDFLAGS -L/usr/kde/3.5/lib -I/usr/qt/3/lib"

   AC_CHECK_LIB([kdecore], [main], [], [AC_MSG_ERROR([Cannot find libkdecore])])

   KCONFIG_BUILD="kconfig"
fi

AC_SUBST(KCONFIG_BUILD)

AC_CONFIG_FILES([buildsys.mk extra.mk libmcs.pc])
AC_OUTPUT

cat << _EOF_

Configuration:
        Keyfile backend: yes
        GConf backend  : $enable_gconf
	KConfig backend: $enable_kconfig

Now type "make" to build, and "make install" to install.
Thank you for using mcs.

_EOF_
