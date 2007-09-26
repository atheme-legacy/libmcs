PROG = mcs-getconfval${PROG_SUFFIX}
SRCS = mcs_getconfval.c

include ../../../buildsys.mk
include ../../../extra.mk

CPPFLAGS += ${MOWGLI_CFLAGS} -I../.. -I../../libmcs
LIBS += ${MOWGLI_LIBS} -L../../libmcs -lmcs
