PROG = mcs-walk-config${PROG_SUFFIX}
SRCS = mcs_walk_config.c

include ../../../buildsys.mk
include ../../../extra.mk

CPPFLAGS += ${MOWGLI_CFLAGS} -I../.. -I../../libmcs
LIBS += -L../../libmcs -lmcs ${MOWGLI_LIBS}
