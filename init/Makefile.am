## Makefile.am -- Process this file with automake to produce Makefile.in
##
##
##
##
CFLAGS=@CFLAGS@
MAINTAINERCLEANFILES = Makefile.in
EXTRA_DIST= c3dinit.h toolCheckLicense.h
BX_LIB = -lBX
INCLUDES = -I$(top_builddir)/include $(X_CFLAGS) -I$(top_builddir)/init -I$(top_builddir)/FLEXlm \
-I$(top_builddir)/motif -I$(top_builddir)/lesen -I$(top_builddir)/lib \
-I/usr/Motif-1.2/include -I$(top_builddir)/lib/cii/include 
noinst_LTLIBRARIES = libinit.la liberror.la
libinit_la_SOURCES = color3d.c \
basis_data_func.c \
color3d_var.c \
font.c \
fenster.c \
init_data.c \
toolCheckLicenseC.c \
toolCheckLicense.c \
c3dinit.c

liberror_la_SOURCES = errors.c

