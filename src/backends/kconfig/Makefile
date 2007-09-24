PLUGIN = kconfig${PLUGIN_SUFFIX}
SRCS = kconfig.cc

plugindir = mcs

include ../../../buildsys.mk
include ../../../extra.mk

CPPFLAGS +=  ${MOWGLI_CFLAGS} -I../.. -DPLUGIN_DIR=\"${libdir}/mcs\" ${PLUGIN_CPPFLAGS} ${CXXFLAGS}
CXXFLAGS += ${PLUGIN_CFLAGS}
LIBS += -L../../libmcs -lmcs -lkdecore ${MOWGLI_LIBS}
