PROG = mcs-query-backends${PROG_SUFFIX}
SRCS = mcs_query_backends.c

include ../../../buildsys.mk
include ../../../extra.mk

CPPFLAGS += ${MOWGLI_CFLAGS} -I../.. -I../../libmcs
LIBS += -L../../libmcs -lmcs ${MOWGLI_LIBS}
