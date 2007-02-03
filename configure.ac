#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([libmcs], [0.2.0], [bugs+libmcs@atheme.org])
AC_CONFIG_SRCDIR([src/libmcs/mcs_loader.c])
AC_CONFIG_HEADER([src/libmcs/mcs_config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_ISC_POSIX

# XXX workaround
PACKAGE="libmcs"
AC_SUBST(PACKAGE)

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [dlopen], , [AC_ERROR([libdl is required for compilation])])

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_CHECK_FUNCS([strstr], , [AC_ERROR([strstr is required for the plugin loader, sorry.])])

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

AC_CONFIG_FILES([libmcs.pc mk/rules.mk])
AC_OUTPUT

cat << _EOF_

Configuration:
        Keyfile backend: yes
        GConf backend  : $enable_gconf

Now type "make" to build, and "make install" to install.
Thank you for using mcs.

_EOF_
