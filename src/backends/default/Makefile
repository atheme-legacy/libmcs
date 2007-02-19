include ../../../mk/rules.mk
include ../../../mk/init.mk

OBJECTIVE_LIBS = libkeyfile.so

LIBDIR = ${libdir}/mcs

CFLAGS += -fPIC -DPIC -I../.. -DPLUGIN_DIR=\"${libdir}/mcs\"

SOURCES = keyfile.c
OBJECTS = ${SOURCES:.c=.o}

include ../../../mk/objective.mk
