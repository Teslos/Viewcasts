## Makefile.am -- Process this file with automake to produce Makefile.in
##
##
##
##

MAINTAINERCLEANFILES = Makefile.in
BX_LIB = -lBX
INCLUDES = -I$(top_builddir)/include -I$(top_builddir)/init \
           -I$(top_builddir)/motif -I$(top_builddir)/lesen \
	   -I$(top_builddir)/lib  \
	   -I$(top_builddir)/lib/cii/include -I$(top_builddir)/lib/utilii/ $(X_CFLAGS)
noinst_LTLIBRARIES = libioclient.la
libioclient_la_SOURCES = lese_colormap.c lese_loesung.c \
    read_geop.c read_img.c write_img.c file.c keys.c
