include ../../../mk/rules.mk
include ../../../mk/init.mk
include ../../../mk/objective.mk

OBJECTIVE_BINS = mcs-getconfval

LDADD = \
	-L../../libmcs -lmcs

CFLAGS += -I../..

SOURCES = mcs_getconfval.c

OBJECTS = ${SOURCES:.c=.o}

mcs-getconfval: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDADD) -o $@
	@printf "%10s     %-20s\n" LINK $@
