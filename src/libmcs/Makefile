LIB = ${LIB_PREFIX}mcs${LIB_SUFFIX}
LIB_MAJOR = 1
LIB_MINOR = 0

SRCS = mcs_backends.c		\
       mcs_handle_factory.c	\
       mcs_init.c		\
       mcs_loader.c		\
       mcs_util.c

INCLUDES = mcs.h

include ../../buildsys.mk
include ../../extra.mk

CPPFLAGS += ${MOWGLI_CFLAGS} -I. -I.. -DPLUGIN_DIR=\"${libdir}/libmcs\" -DPLUGIN_SUFFIX=\"${PLUGIN_SUFFIX}\" -D_MCS_CORE -DMCS_SYSCONFDIR=\"${sysconfdir}\"
CFLAGS += ${LIB_CFLAGS}
LIBS += ${MOWGLI_LIBS}
