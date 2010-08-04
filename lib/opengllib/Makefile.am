## Makefile.am -- Process this file with automake to produce Makefile.in
##
##
##
##

MAINTAINERCLEANFILES = Makefile.in
INCLUDES = -I$(top_builddir)/include  -I$(top_builddir)/init -I/usr/include/GL \
            -I$(top_builddir)/motif -I$(top_builddir)/lesen -I$(top_builddir)/lib \
            -I$(top_builddir)/lib/cii/include $(GL_CFLAGS)  

noinst_LTLIBRARIES = libopengl.la
libopengl_la_SOURCES = bitmaplib.c bitmaplib.h \
                      opengllib.c opengllib.h paulslib.c \
                      paulslib.h  writergb.c
libopengl_la_LIBADD = $(top_builddir)/lib/opengllib/libglut.a 
