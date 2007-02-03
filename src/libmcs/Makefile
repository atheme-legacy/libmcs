include ../../mk/rules.mk
include ../../mk/init.mk

PICLDFLAGS = $(LIBLDFLAGS)

OBJECTIVE_LIBS = libmcs$(SHARED_SUFFIX)
OBJECTIVE_SONAME_SUFFIX = 1

CFLAGS += $(PICFLAGS) -I.. -DPLUGIN_DIR=\"${libdir}/mcs\"

SOURCES = \
	mcs_backends.c \
	mcs_handle_factory.c \
	mcs_init.c \
	mcs_list.c \
	mcs_loader.c

LIBADD += -ldl

OBJECTS = ${SOURCES:.c=.o}

INCLUDEDIR = ${prefix}/include/libmcs

HEADERS = mcs.h mcs_config.h

include ../../mk/objective.mk

install-posthook:
	@mv ${DESTDIR}/${LIBDIR}/libmcs$(SHARED_SUFFIX) ${DESTDIR}/${LIBDIR}/libmcs$(SHARED_SUFFIX).1.0.0
	@ln -sf libmcs$(SHARED_SUFFIX).1.0.0 \
		${DESTDIR}/${LIBDIR}/libmcs$(SHARED_SUFFIX).1
	@ln -sf libmcs$(SHARED_SUFFIX).1 \
		${DESTDIR}/${LIBDIR}/libmcs$(SHARED_SUFFIX)
