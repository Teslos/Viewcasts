## Makefile.am -- Process this file with automake to produce Makefile.in
##
## Author: Toni Ivas
##
##
##
auxdir = @ac_aux_dir@
AUX_DIST = $(auxdir)/config.guess $(auxdir)/config.sub \
           $(auxdir)/install-sh $(auxdir)/ltconfig \
           $(auxdir)/missing $(auxdir)/mkinstalldirs 
AUX_DIST_EXTRA = $(auxdir)/ac_path_lib.m4 $(auxdir)/mdl_have_opengl.m4
FLEXLM_PATH = @FLEXlm@
FLEXLM_LIB =   $(FLEXLM_PATH)/lm_new.o -L$(FLEXLM_PATH)/ \
              -llmgr -lcrvs -lsb -lpthread
  
BX_LIB_SGI = -lBXm
OGLPAULSLIB = -lopengllib
GNULIB = -lgnumalloc
CIILIB = -lcii
GLCOLORCHOOSER = @GLCOLORCHOOSER@
SGMLIB = @SGMLIB@
EXTRA_DIST  = bootstrap
AUTOMAKE_OPTIONS = foreign
SUBDIRS = init motif graphics ioclient lib bxwidgets include
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure
LIBADD_ALL = $(GLCOLORCHOOSER) $(SGMLIB) -lGLw -lglut -lX11 \
 -lXext -lXmu -lXt -lXm -lXpm -lMrm -lm -lc 
#if USE_GL
#AM_CFLAGS = -O2 -DUSE_GL -L$(top_builddir)/lib
#endif
AM_CFLAGS = -Wall  $(GL_CFLAGS) -I/usr/include/ -I/usr/X11R6/include

bin_PROGRAMS = viewcasts
viewcasts_SOURCES = 
viewcasts_LDFLAGS = -L$(top_builddir)/lib -L/usr/Motif-1.2/lib32 
viewcasts_LDADD = $(top_builddir)/init/libinit.la \
$(top_builddir)/motif/libmotif.la  \
$(top_builddir)/graphics/libgraphics.la \
$(top_builddir)/bxwidgets/lib/libBXm.a\
$(top_builddir)/ioclient/libioclient.la \
$(top_builddir)/lib/opengllib/libopengl.la \
$(top_builddir)/lib/malloc/libgnumalloc.la \
$(top_builddir)/lib/cii/src/libcii.la \
$(top_builddir)/lib/utilii/libcvs.la \
$(top_builddir)/lib/utilii/libgen.la \
$(top_builddir)/lib/widgets/libwidgets.la \
$(top_builddir)/init/liberror.la $(LIBADD_ALL) $(FLEXLM_LIB)

