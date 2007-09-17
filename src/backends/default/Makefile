PLUGIN = keyfile${PLUGIN_SUFFIX}
SRCS = keyfile.c

plugindir = mcs

include ../../../buildsys.mk
include ../../../extra.mk

CPPFLAGS += ${PLUGIN_CPPFLAGS} ${MOWGLI_CFLAGS} -I../.. -DPLUGIN_DIR=\"${libdir}/mcs\"
CFLAGS += ${PLUGIN_CFLAGS}
LIBS += ${MOWGLI_LIBS}
