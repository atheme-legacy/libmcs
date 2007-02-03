include ../../../mk/rules.mk
include ../../../mk/init.mk
include ../../../mk/objective.mk

OBJECTIVE_BINS = mcs-query-backends

LDADD = \
	-L../../libmcs -lmcs

CFLAGS += -I../..

SOURCES = mcs_query_backends.c

OBJECTS = ${SOURCES:.c=.o}

mcs-query-backends: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDADD) -o $@
	@printf "%10s     %-20s\n" LINK $@
