PLUGIN = gconf${PLUGIN_SUFFIX}
SRCS = gconf.c

plugindir = mcs

include ../../../buildsys.mk
include ../../../extra.mk

CPPFLAGS += ${PLUGIN_CPPFLAGS} ${MOWGLI_CFLAGS} -I../.. -DPLUGIN_DIR=\"${libdir}/mcs\" ${GCONF_CFLAGS}
CFLAGS += ${PLUGIN_CFLAGS}
LIBS += ${MOWGLI_LIBS} ${GCONF_LIBS}
