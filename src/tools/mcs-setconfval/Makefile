PROG = mcs-setconfval${PROG_SUFFIX}
SRCS = mcs_setconfval.c

include ../../../buildsys.mk
include ../../../extra.mk

CPPFLAGS += ${MOWGLI_CFLAGS} -I../.. -I../../libmcs
LIBS += -L../../libmcs -lmcs ${MOWGLI_LIBS}
