include ../../../mk/rules.mk
include ../../../mk/init.mk

OBJECTIVE_LIBS = libkconfig.so

LIBDIR = ${libdir}/mcs

CXXFLAGS += -fPIC -DPIC -I../.. -DPLUGIN_DIR=\"${libdir}/mcs\"

LIBADD += -lkdecore

SOURCES = kconfig.cc
OBJECTS = ${SOURCES:.cc=.o}

include ../../../mk/objective.mk
