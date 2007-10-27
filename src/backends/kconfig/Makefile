PLUGIN = kconfig${PLUGIN_SUFFIX}
SRCS = kconfig.cc

include ../../../buildsys.mk
include ../../../extra.mk

CPPFLAGS +=  ${MOWGLI_CFLAGS} -I../.. -DPLUGIN_DIR=\"${libdir}/mcs\" ${PLUGIN_CPPFLAGS} ${CXXFLAGS}
CXXFLAGS += ${PLUGIN_CFLAGS} ${KCONFIG_FLAGS}
LIBS += -L../../libmcs -lmcs -lkdecore ${MOWGLI_LIBS} ${KCONFIG_LIBS}
