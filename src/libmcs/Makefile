include ../../extra.mk

LIB = ${LIB_PREFIX}mcs${LIB_SUFFIX}
LIB_MAJOR = 2
LIB_MINOR = 0

SRCS = mcs_backends.c		\
       mcs_handle_factory.c	\
       mcs_init.c		\
       ${MCS_LOADER}	\
       mcs_util.c

INCLUDES = mcs.h

include ../../buildsys.mk

CPPFLAGS += ${MOWGLI_CFLAGS} -I. -I.. -DBIN_DIR=\"${bindir}\" -DPLUGIN_DIR=\"${plugindir}\" -DPLUGIN_SUFFIX=\"${PLUGIN_SUFFIX}\" -D_MCS_CORE -DMCS_SYSCONFDIR=\"${sysconfdir}\"
CFLAGS += ${LIB_CFLAGS}
LIBS += ${MOWGLI_LIBS}
