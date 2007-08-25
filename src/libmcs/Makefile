include ../../mk/rules.mk
include ../../mk/init.mk

PICLDFLAGS = $(LIBLDFLAGS)

OBJECTIVE_LIBS = libmcs$(SHARED_SUFFIX)
OBJECTIVE_SONAME_SUFFIX = 1

LIBADD += $(DYNAMIC_LD_LIBS)

CFLAGS += $(PICFLAGS) -I.. -DPLUGIN_DIR=\"${libdir}/mcs\" -D_MCS_CORE -DMCS_SYSCONFDIR=\"${sysconfdir}\"

SOURCES = \
	mcs_backends.c \
	mcs_handle_factory.c \
	mcs_init.c \
	mcs_loader.c \
	mcs_logger.c \
	mcs_util.c

OBJECTS = ${SOURCES:.c=.o}

INCLUDEDIR = ${prefix}/include/libmcs

HEADERS = mcs.h

include ../../mk/objective.mk

install-posthook:
	@mv ${DESTDIR}/${LIBDIR}/libmcs$(SHARED_SUFFIX) ${DESTDIR}/${LIBDIR}/libmcs$(SHARED_SUFFIX).1.0.0
	@ln -sf libmcs$(SHARED_SUFFIX).1.0.0 \
		${DESTDIR}/${LIBDIR}/libmcs$(SHARED_SUFFIX).1
	@ln -sf libmcs$(SHARED_SUFFIX).1 \
		${DESTDIR}/${LIBDIR}/libmcs$(SHARED_SUFFIX)
