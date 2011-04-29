include ../../extra.mk

LIB = ${LIB_PREFIX}mcs${LIB_SUFFIX}
LIB_MAJOR = 2
LIB_MINOR = 0

SRCS = ../backends/default/keyfile.c \
       mcs_backends.c \
       mcs_handle_factory.c	\
       mcs_init.c		\
       mcs_util.c

INCLUDES = mcs.h

include ../../buildsys.mk

CPPFLAGS += ${MOWGLI_CFLAGS} -I. -I.. -D_MCS_CORE
CFLAGS += ${LIB_CFLAGS}
LIBS += ${MOWGLI_LIBS}
