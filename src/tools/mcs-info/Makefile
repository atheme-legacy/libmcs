PROG = mcs-info${PROG_SUFFIX}
SRCS = mcs_info.c

include ../../../buildsys.mk
include ../../../extra.mk

CPPFLAGS += ${MOWGLI_CFLAGS} -I../.. -I../../libmcs
LIBS += -L../../libmcs -lmcs ${MOWGLI_LIBS}
