## Makefile.am -- Process this file with automake to produce Makefile.in
##
##
##
##
MAINTAINERCLEANFILES = Makefile.in
EXTRA_DIST = GraphicsGems.h c3d_gl.h files.h gl_local.h\
	keyboard.h ref.h shared.h common.h            

BX_LIB = -lBX
INCLUDES = -I$(top_builddir)/include -I$(top_builddir)/init \
            -I$(top_builddir)/motif -I$(top_builddir)/lesen -I$(top_builddir)/lib \
           -I$(top_builddir)/lib/cii/include $(GL_CFLAGS) 
noinst_LTLIBRARIES = libgraphics.la
libgraphics_la_SOURCES = actions.c \
    grafik.c \
    create_da.c \
    sovlayerutil.c \
    make_matrix.c \
    grafik_cb_handler.c\
    grafik_func.c \
    grafik_var.c \
    make_matrix.c \
    make_objects.c \
    rgbfarben.c \
    schneide.c \
    tools.c \
    vektortransform.c \
    zeichne_abkuehlkurven.c \
    zeichne_geometrie.c \
    zeichne_stoffe.c \
    zeichne_stroemung.c \
    zeichnen.c \
    shade.c \
    util.c \
    zeichne_isolinien.c \
    isosurface.c \
    ewrapper.c \
    sgiobj.c \
    tmeshobj.c \
    tomesh.c \
    label.c \
    trackball.c \
    capping.c \
    aaline.c \
    accum.c \
    plane_cut.c \
    cutting.c \
    GraphicsGems.h \
    math.c \
    build_sgi.c \
    edge.c \
    oglgraphics.c\
    cmd.c \
    sys_irix.c \
    misc.c \
    gl_irix.c \
    glw_imp.c \
    glr_main.c \
    gl_image.c \
    gl_draw.c \
    cvar.c \
    files.c \
    common.c \
    glob.c \
    glut_dstr.c









