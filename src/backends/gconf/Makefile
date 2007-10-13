PLUGIN = gconf${PLUGIN_SUFFIX}
SRCS = gconf.c

include ../../../buildsys.mk
include ../../../extra.mk

CPPFLAGS += ${PLUGIN_CPPFLAGS} ${MOWGLI_CFLAGS} -I../.. -DPLUGIN_DIR=\"${libdir}/mcs\" ${GCONF_CFLAGS}
CFLAGS += ${PLUGIN_CFLAGS}
LIBS += -L../../libmcs -lmcs ${MOWGLI_LIBS} ${GCONF_LIBS}
