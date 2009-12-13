dnl                                               -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([libmcs], [0.7.1], [bugs+libmcs@atheme.org])
AC_CONFIG_SRCDIR([src/libmcs/mcs_loader.c])
AC_CONFIG_HEADER([src/libmcs/mcs_config.h])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

dnl Checks for programs.
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_ISC_POSIX

test x"$GCC" = x"yes" && CFLAGS="$CFLAGS -pipe -Wall"

dnl XXX workaround
PACKAGE="libmcs"
AC_SUBST([PACKAGE])

dnl Checks for libraries.
BUILDSYS_SHARED_LIB
AC_CHECK_LIB([dl], [dlopen], [LIBS="$LIBS -ldl"])

dnl Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h string.h unistd.h locale.h stdarg.h sys/types.h sys/stat.h errno.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_CHECK_FUNCS([strstr], , [AC_ERROR([strstr is required for the plugin loader, sorry.])])
AC_CHECK_FUNCS([memset setlocale strcasecmp strchr strdup strerror strtol strtod])
AC_FUNC_STAT

PKG_CHECK_MODULES([MOWGLI], [libmowgli >= 0.4.0], [], [AC_MSG_ERROR([libmowgli 0.4.0 or newer required])])

dnl gconf
dnl =====
AC_ARG_ENABLE([gconf],
    [AS_HELP_STRING([--disable-gconf], [disable GConf support (default=enabled)])],
    [enable_gconf=$enableval],
    [enable_gconf="yes"]
)

if test "x$enable_gconf" = "xyes"; then
    PKG_CHECK_MODULES([GCONF], [gconf-2.0 >= 2.6.0],
	[
	 GCONF_BUILD="gconf"
	],
	[
	 AC_MSG_WARN([Cannot find GConf, disabling GConf support])
	 enable_gconf="no"
	]
    )
else
    enable_gconf="no"
fi

AC_SUBST([GCONF_BUILD])

dnl KConfig
dnl =======
AC_ARG_ENABLE([kconfig],
    [AS_HELP_STRING([--disable-kconfig], [disable KDE KConfig support (default=enabled)])],
    [enable_kconfig=$enableval],
    [enable_kconfig="yes"]
)
AC_ARG_WITH([qt-path],
	[AS_HELP_STRING([--with-qt-path], [path to Qt3 base directory (required for kconfig)])],
	[qtpath=$withval],
	[qtpath=/usr/qt/3]
)
AC_ARG_WITH([kde-path],
	[AS_HELP_STRING([--with-kde-path], [path to KDE3 base directory (required for kconfig)])],
	[kdepath=$withval],
	[kdepath=/usr/kde/3.5]
)

if test "x$enable_kconfig" = "xyes"; then
    AC_PROG_CXX
    # XXX - does this work?
    KCONFIG_FLAGS="-I${kdepath}/include -I${qtpath}/include"
    KCONFIG_LIBS="-L${kdepath}/lib -L${qtpath}/lib"
	CFLAGS_TMP="${CFLAGS}"
	LDFLAGS_TMP="${LDFLAGS}"
	CFLAGS="${CFLAGS} ${KCONFIG_FLAGS}"
	LDFLAGS="${LDFLAGS} ${KCONFIG_LIBS}"

    LIBS_TMP="${LIBS}"
    AC_CHECK_LIB([kdecore], [main],
		[KCONFIG_BUILD="kconfig"],
		[AC_MSG_WARN([Cannot find libkdecore, disabling KDE KConfig support])
		enable_kconfig="no"]
    )
    KCONFIG_LIBS="${KCONFIG_LIBS} ${LIBS}"
        LIBS="${LIBS_TMP}"
	CFLAGS="${CFLAGS_TMP}"
	LDFLAGS="${LDFLAGS_TMP}"
	AC_SUBST([KCONFIG_FLAGS])
	AC_SUBST([KCONFIG_LIBS])
else
    enable_kconfig="no"
fi

AC_SUBST([KCONFIG_BUILD])

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

Configuration:
        Keyfile backend: yes
        GConf backend  : $enable_gconf
	KConfig backend: $enable_kconfig

Now type "make" to build, and "make install" to install.
Thank you for using mcs.

_EOF_
