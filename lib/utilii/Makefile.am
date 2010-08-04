#
#
#
#
NOWEAVE=@NOWEAVE@
NOTANGLE=@NOTANGLE@
MAINTAINERCLEANFILES = Makefile.in
INCLUDES = -I$(top_builddir)/lib/cii/include
noinst_LTLIBRARIES = libcvs.la libgen.la
libcvs_la_SOURCES =  csvgetline.c csv.h
libgen_la_SOURCES = gendlist.c genlist.c genstack.c \
gendlist.h genlist.h genstack.h

all: genlist.c genlist.tex

genlist.c: genlist.nw
	$(NOTANGLE) -L genlist.nw > genlist.c
genlist.tex: genlist.nw
	$(NOWEAVE)  genlist.nw > genlist.tex
