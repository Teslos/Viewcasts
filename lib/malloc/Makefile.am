## Makefile.am -- Process this file with automake to produce Makefile.in
##
##
##
##

MAINTAINERCLEANFILES = Makefile.in
INCLUDES = -I$(top_builddir)/include  -I$(top_builddir)/init -I/usr/include/GL \
            -I$(top_builddir)/motif -I$(top_builddir)/lesen -I$(top_builddir)/lib
noinst_LTLIBRARIES = libgnumalloc.la 
libgnumalloc_la_SOURCES = calloc.c free.c malloc.c \
 malloc.h morecore.c realloc.c valloc.c 
