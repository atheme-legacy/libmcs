include mk/rules.mk
include mk/init.mk

SUBDIRS = src

OBJECTIVE_DATA = libmcs.pc:$(LIBDIR)/pkgconfig

include mk/objective.mk
