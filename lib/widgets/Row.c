/********************************************************************
 *         This example code is from the book:
 *
 *           The X Window System: Programming and Applications with Xt
 *           Second OSF/Motif Edition
 *         by
 *           Douglas Young
 *           Prentice Hall, 1994
 *
 *         Copyright 1994 by Prentice Hall
 *         All Rights Reserved
 *
 *  Permission to use, copy, modify, and distribute this software for 
 *  any purpose except publication and without fee is hereby granted, provided 
 *  that the above copyright notice appear in all copies of the software.
 * *****************************************************************************/

/********************************************************
 * Row.c: Methods for the Row widget
 *******************************************************/
#include    <Xm/XmP.h>
#include    "RowP.h"

static void Initialize ( Widget req, Widget new_w,
                         ArgList args, Cardinal *numArgs );
static void Resize ( Widget w );
static void ChangeManaged ( Widget w );
static void Redisplay ( Widget w, XEvent *event, Region region );

static XtGeometryResult GeometryManager ( Widget            w, 
                                          XtWidgetGeometry *request,
                                          XtWidgetGeometry *reply );
static XtGeometryResult QueryGeometry ( Widget            widget,
                                        XtWidgetGeometry *intended,
                                        XtWidgetGeometry *reply );

static void PreferredSize ( XsRowWidget  rw,
                            Dimension   *w,
                            Dimension   *h );

static void DoLayout ( XsRowWidget w );
    
XsRowClassRec xsRowClassRec = {
{
    /* core_class members      */
    ( WidgetClass )  &xmManagerClassRec, /* superclass         */
    "XsRow",                          /* class_name         */
    sizeof ( XsRowRec ) ,             /* widget_size        */
    NULL,                             /* class_initialize   */
    NULL,                             /* class_part_init    */  
    FALSE,                            /* class_inited       */  
    Initialize,                       /* initialize         */
    NULL,                             /* initialize_hook    */  
    XtInheritRealize,                 /* realize            */
    NULL,                             /* actions            */
    0,                                /* num_actions        */  
    NULL,                             /* resources          */
    0,                                /* num_resources      */
    NULLQUARK,                        /* xrm_class          */
    TRUE,                             /* compress_motion    */  
    XtExposeCompressMaximal,          /* compress_exposure  */  
    TRUE,                             /* compress_enterleave*/  
    FALSE,                            /* visible_interest   */
    NULL,                             /* destroy            */
    Resize,                           /* resize             */
    Redisplay,                        /* expose             */
    NULL,                             /* set_values         */
    NULL,                             /* set_values_hook    */
    XtInheritSetValuesAlmost,         /* set_values_almost  */
    NULL,                             /* get_values_hook    */  
    NULL,                             /* accept_focus       */
    XtVersion,                        /* version            */  
    NULL,                             /* callback_private   */
    XtInheritTranslations,                             /* tm_table           */
    QueryGeometry,                    /* query_geometry     */  
    NULL,                             /* display_accelerator*/
    NULL,                             /* extension          */
},
{
    /* composite_class members */
    GeometryManager,                  /* geometry_manager   */
    ChangeManaged,                    /* change_managed     */
    XtInheritInsertChild,             /* insert_child       */  
    XtInheritDeleteChild,             /* delete_child       */  
    NULL,                             /* extension          */
},
{      /* constraint_class fields */
    NULL,                             /* resource list        */   
    0,                                /* num resources        */   
    0,                                /* constraint size      */   
    NULL,                             /* init proc            */   
    NULL,                             /* destroy proc         */   
    NULL,                             /* set values proc      */   
    NULL,                             /* extension            */
},
{      /* manager class     */
    XtInheritTranslations,            /* translations         */
    NULL,                             /* syn resources        */
    0,                                /* num syn_resources    */
    NULL,                             /* syn_cont_resources     */
    0,                                /* num_syn_cont_resources */
    XmInheritParentProcess,           /* parent_process       */
    NULL,                             /* extension            */    
},
{
    /* Row class members */
    0,                                /* empty              */  
}
};
WidgetClass xsRowWidgetClass =  ( WidgetClass )  &xsRowClassRec;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
static void Initialize ( Widget   request,
                         Widget    new_w,
                         ArgList   args,
                         Cardinal *numArgs ) 
{
    if  ( XtWidth ( new_w )  <= 0 ) 
        XtWidth ( new_w )  = 5;
    if  ( XtWidth ( new_w )  <= 0 ) 
        XtHeight ( new_w )  = 5;
} 
    
static void Resize ( Widget w ) 
{
    DoLayout ( ( XsRowWidget ) w );
} 
    
static void DoLayout ( XsRowWidget rw ) 
{
    Widget     child;
    int        i;
    Dimension  childwidth;
    Position   xpos;
    Dimension  pad;
    int        numManagedChildren;

   /*
    * Compute the total width of all managed children 
    */
    
    childwidth = 0;
    numManagedChildren = 0;

    for  ( i = 0; i < rw->composite.num_children; i++ ) 
    {
        child = rw -> composite.children[i];
       
        if ( child->core.managed )
        {
            childwidth += XtWidth ( child );
            numManagedChildren++;
        }
    }
    
   /*
    *  Divide any remaining space by the number 
    *  of spaces between managed children.
    */
    
    pad = 0;
    
    if ( XtWidth ( rw )  > childwidth ) 
        pad =  ( XtWidth ( rw )  - childwidth )  /
           ( numManagedChildren + 1 );
    
   /*
    * Position all managed children.
    */
    
    xpos = pad;
    
    for  ( i = 0; i < rw->composite.num_children; i++ ) 
    {
        int verticalPad = 0;
        child = rw -> composite.children[i];

        if ( child->core.managed )
        {
            if ( XtHeight( child) + 
                 2 * XtBorderWidth ( child ) < XtHeight ( rw ) )
                verticalPad =  ( XtHeight ( rw )  - 
                                  ( XtHeight ( child )  -
                                    2 * XtBorderWidth ( child ) ) ) / 2;
        
            _XmConfigureObject ( child, xpos, verticalPad,
                                 XtWidth ( child ), XtHeight ( child ),
                                 XtBorderWidth ( child ) );
        
            xpos += pad + XtWidth ( child );
        }
    }
}
                    
static void Redisplay ( Widget w, XEvent *event, Region region ) 
{
   /*
    * Redraw all gadgets.
    */
    
    _XmRedisplayGadgets ( w, event, region );
}
        
static void PreferredSize ( XsRowWidget  rw,
                            Dimension   *w,
                            Dimension   *h ) 
{
    int i;
    
    *w = 0;
    *h = 0;

   /*
    * Loop through the list of children, computing the
    * maximum height and the total width of all managed children.
    */

    for  ( i = 0; i < rw->composite.num_children; i++ ) 
    {
        Widget child = rw->composite.children[i];
        
        if ( child->core.managed )
        {
            *w += XtWidth ( child );
        
            if ( *h < XtHeight ( child ) ) 
                *h = XtHeight ( child );
        }
    }
}
            
#if 0
static void PreferredSize ( XsRowWidget  rw,
                            Dimension   *w,
                            Dimension   *h ) 
{
    int i;
    
    *w = 0;
    *h = 0;

   /*
    * Loop through the list of children, computing the
    * maximum height and the total width of all children
    * based on their preferred sizes.
    */

    for  ( i = 0; i < rw->composite.num_children; i++ ) 
    {
        XtWidgetGeometry preferred;
        Widget child = rw->composite.children[i];

        if ( child->core.managed )
        {
            XtQueryGeometry ( child, NULL, &preferred );

            *w += preferred.width;;
        
            if ( *h < preferred.height ) 
                *h = preferred.height;
        }
    }
}
                    #endif

static XtGeometryResult QueryGeometry ( Widget            widget,
                                        XtWidgetGeometry *intended,
                                        XtWidgetGeometry *reply ) 
{
    XsRowWidget rw =  ( XsRowWidget )  widget;
    Dimension   w, h;

   /*
    * The Row widget does not care about its position, so
    * if a query does not involve a change to height or width
    * just return yes.
    */

    if ( intended->request_mode & ( ~ ( CWWidth | CWHeight ) ) ) 
        return  ( XtGeometryYes );

   /*
    * Compute the Row widgetÕs preferred size
    */

    PreferredSize  ( rw, &w, &h );
 
   /*
    * If the intended size matches the desired size, return yes.
    */

    if ( intended->request_mode & CWWidth &&
         intended->width == w &&
         intended->request_mode & CWHeight &&
         intended->height == h )
        return ( XtGeometryYes );


   /*
    * If the desired size is equal to the current size, return no,
    * to ask to be left alone.
    */

    if ( w == XtWidth ( widget ) &&
         h == XtHeight ( widget ) )
       return ( XtGeometryNo );

   /*
    * In all other cases, fill out the reply structure to 
    * indicate the preferred size and reply almost.
    */

    reply->request_mode = CWWidth | CWHeight;
    reply->width = w;
    reply->height = h;
    return XtGeometryAlmost;
}
                                        
static XtGeometryResult GeometryManager ( Widget             widget, 
                                          XtWidgetGeometry  *request,
                                          XtWidgetGeometry  *reply ) 
{
    XsRowWidget  rw =  ( XsRowWidget )  XtParent ( widget );
    
   /*
    * Disallow all motion
    */
    
    if  ( request->request_mode &  ( CWX | CWY ) ) 
        return  ( XtGeometryNo );
    
   /*
    *  Grant all size requests
    */
    
    if  ( request->request_mode &  ( CWWidth | CWHeight ) ) 
    {
        Dimension w = 0, h = 0;
        int i;
        
       /*
        * Set the childÕs size to the requested size.
        */

        if  ( request->request_mode & CWWidth ) 
            XtWidth ( widget )   = request->width;
        if  ( request->request_mode & CWHeight ) 
            XtHeight ( widget )  = request->height;
       /*
        * Now compute the new minimum size 
        */

        for  ( i = 0; i < rw->composite.num_children; i++ ) 
        {
            Widget child = rw->composite.children[i];
            
            if ( child->core.managed )
            {
                w += XtWidth ( child );
        
                if ( h < XtHeight ( child ) ) 
                    h = XtHeight ( child );
            }
        }
        
       /*
        * if the minimum size required to contain all
        * children is larger than the current size
        * of the Row widget, make a request to grow.
        */

        if ( w > XtWidth ( rw )  ||
             h > XtHeight ( rw ) ) 
        {
            XtGeometryResult result;
           
            Dimension replyWidth, replyHeight;

           /*
            * Request a resize
            */

            result = XtMakeResizeRequest  ( ( Widget ) rw, w, h,
                                            &replyWidth,
                                            &replyHeight );
           /*
            * If an alternative is proposed, accept it by making
            * a second request with the pre-approved values.
            */

            if ( result == XtGeometryAlmost ) 
                XtMakeResizeRequest  ( ( Widget ) rw,
                                       replyWidth, replyHeight, 
                                       NULL, NULL );       
        }
        
       /*
        * Recompute the layout based on the new sizes.
        */

        DoLayout ( rw );
    }
    
    return  ( XtGeometryYes );
}
                                                    
static void ChangeManaged ( Widget child ) 
{
    XsRowWidget rw = ( XsRowWidget ) child; 
    Dimension w, h;

    _XmNavigChangeManaged( child );    
    
   /*
    * Attempt to establish the Row widget's size.
    */
    
    PreferredSize  ( rw, &w, &h );
    
    if ( w > XtWidth ( rw )  || h > XtHeight ( rw ) ) 
    {
        XtGeometryResult result;
        Dimension replyWidth, replyHeight;
        
        result = XtMakeResizeRequest  ( ( Widget ) rw, w, h,
                                        &replyWidth, &replyHeight );
        if ( result == XtGeometryAlmost ) 
            XtMakeResizeRequest  ( ( Widget ) rw,
                                   replyWidth, replyHeight, 
                                   NULL, NULL );   
    }
   /*
    * Position all children
    */

    DoLayout ( rw );
}
            
Widget XsCreateRow ( Widget   parent,
                     char    *name,
                     ArgList  arglist,
                     Cardinal argcount )  
{
    return  ( XtCreateWidget ( name, xsRowWidgetClass, 
                               parent, arglist, argcount ) );
}

