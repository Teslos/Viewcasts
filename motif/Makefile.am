## Makefile.am -- Process this file with automake to produce Makefile.in
##
##
##
##
EXTRA_DIST = color_l.uil colors_ui.uil fileSel.uil light_l.uil \
light_ui.uil main_mod.uil toolbar_back_1.xpm toolbar_back_2.xpm \
toolbar_forw_1.xpm toolbar_forw_2.xpm zoom.h

MAINTAINERCLEANFILES = Makefile.in
INCLUDES = -I$(top_builddir)/include -I$(top_builddir)/bxwidgets/include \
-I$(top_builddir)/motif -I$(top_builddir)/lesen -I$(top_builddir)/lib/widgets \
-I$(top_builddir)/lib/cii/include -I$(top_builddir)/lib/utilii \
-I/usr/Motif-1.2/include $(GL_CFLAGS) $(X_CFLAGS) 
noinst_LTLIBRARIES = libmotif.la
libmotif_la_LDFLAGS = -L/usr/lib32
libmotif_la_SOURCES = 2dgrafik_func.c \
2dgrafik_interface.c \
anim_zeit.c \
ask_user.c \
color3d_func.c \
color3d_interface.c \
colormap.c \
cursor.c \
fsb_func.c \
help_handler.c \
hilfs_func.c \
init_interface.c \
hilfs_func.c \
init_interface.c \
init_stoffbuttons.c \
isolinien_func.c \
isolinien_interface.c  \
loesung.c loesung_func.c \
loesung_interface.c lsb_func.c \
motif_var.c \
stoffe_func.c \
stoffe_interface.c \
transform_func.c \
transform_interface.c \
light_interface.c \
vektor_func.c \
vektor_interface.c \
warning.c \
pop_menus.c \
sovlayerutil.c \
colors_interface.c \
init_uil.c \
color_chooser.c \
InstallLabeledPixmap.c \
InstallPixmap.c \
cut_interface.c \
cut_func.c \
createmovie.c \
createmovie_func.c 


all: colors_ui.uid fileSel.uid light_ui.uid main_mod.uid

colors_ui.uid: colors_ui.uil
	$(UIL) -o $@ colors_ui.uil
fileSel.uid: fileSel.uil
	$(UIL) -o $@ fileSel.uil
light_ui.uid: light_ui.uil
	$(UIL) -o $@ light_ui.uil
main_mod.uid: main_mod.uil
	$(UIL) -o $@ main_mod.uil
cut.uid: cut.uil
	$(UIL) -o $@ cut.uil






