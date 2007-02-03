include ../../../mk/rules.mk
include ../../../mk/init.mk

OBJECTIVE_LIBS = libgconf.so

libdir = ${exec_prefix}/lib/mcs

CFLAGS += -fPIC -DPIC -I../.. -DPLUGIN_DIR=\"${libdir}/mcs\" ${GCONF_CFLAGS}

LIBADD += ${GCONF_LIBS}

SOURCES = gconf.c
OBJECTS = ${SOURCES:.c=.o}

include ../../../mk/objective.mk
