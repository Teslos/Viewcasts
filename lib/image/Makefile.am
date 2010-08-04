##
##
##
##
##

MAINTAINERCLEANFILES = Makefile.in
INCLUDES = -I$(top_builddir)/include  -I$(top_builddir)/init -I/usr/include/GL \
            -I$(top_builddir)/motif -I$(top_builddir)/lesen -I$(top_builddir)/lib $(GL_CFLAGS)

noinst_LIBRARIES = libfastimg.a
libfastimg_a_SOURCES = fastimg_logl.c file_logl.c misc_logl.c
libfastimg_a_LIBADD = -limage -lm
##bin_PROGRAMS = ipaste_ogl
##ipaste_ogl_SOURCES = ipaste_ogl.c
##ipaste_ogl_LDADD = -lGL -lGLU -lX11 $(top_builddir)/lib/image/libfastimg.a
