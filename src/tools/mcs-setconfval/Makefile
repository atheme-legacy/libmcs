include ../../../mk/rules.mk
include ../../../mk/init.mk
include ../../../mk/objective.mk

OBJECTIVE_BINS = mcs-setconfval

LDADD = \
	-L../../libmcs -lmcs

CFLAGS += -I../..

SOURCES = mcs_setconfval.c

OBJECTS = ${SOURCES:.c=.o}

mcs-setconfval: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDADD) -o $@
	@printf "%10s     %-20s\n" LINK $@
