include ../../../mk/rules.mk
include ../../../mk/init.mk
include ../../../mk/objective.mk

OBJECTIVE_BINS = mcs-info

LDADD = \
	-L../../libmcs -lmcs

CFLAGS += -I../..

SOURCES = mcs_info.c

OBJECTS = ${SOURCES:.c=.o}

mcs-info: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDADD) -o $@
	@printf "%10s     %-20s\n" LINK $@
