/*
    *****************************************************************************
    *                                                                           *
    *   gtkfasttextview.c                                                       *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Copyright (C) 2011-2013 Guillaume Wardavoir                             *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   This program is free software; you can redistribute it and/or modify    *
    *   it under the terms of the GNU General Public License as published by    *
    *   the Free Software Foundation; either version 2 of the License, or       *
    *   (at your option) any later version.                                     *
    *                                                                           *
    *   This program is distributed in the hope that it will be useful,         *
    *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
    *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
    *   GNU General Public License for more details.                            *
    *                                                                           *
    *   You should have received a copy of the GNU General Public License       *
    *   along with this program; if not, write to the Free Software             *
    *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,   *
    *   USA.                                                                    *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Purpose : Simple and fast gtk textview widget. Why not using            *
    *       GtkDrawingArea ? Because we use an explicit  refresh() method to    *
    *       resynchronize the view with the model, instead of having a bunch    *
    *       of events each time the model is modified.                          *
    *                                                                           *
    *****************************************************************************
*/

#include    "string.h"
#include    "math.h"

#include "gtkfasttextview.h"
#include "C/buffers/libgwrc-fast-text-buffer-private.h"

#define GWR_GTK_VERSION_GE(X,Y)                                                 \
    (                                                                           \
        ( GTK_MAJOR_VERSION >= X )      &&                                      \
        ( GTK_MINOR_VERSION >= Y )                                              \
    )

#define GWR_BOUND_LE(V,B)                                                       \
    if ( V > B )                                                                \
        V = B;

#define GWR_BOUND_GE(V,B)                                                       \
    if ( V < B )                                                                \
        V = B;

#define GTK_FAST_TEXT_VIEW_VBAR_AREA_THICKNESS      10
#define GTK_FAST_TEXT_VIEW_HBAR_AREA_THICKNESS      10
#define GTK_FAST_TEXT_VIEW_HANDLE_LENGTH_MIN        10
//  ****************************************************************************
//
//  Forward declarations - GTK
//
//  ****************************************************************************
//  Private class member
#define GTK_FAST_TEXT_VIEW_PRIVATE(obj)                                         \
    G_TYPE_INSTANCE_GET_PRIVATE                                                 \
    (                                                                           \
        (obj)                   ,                                               \
        GTK_FAST_TEXT_VIEW_TYPE ,                                               \
        GtkFastTextViewPrivate                                                  \
    )

typedef struct _GftvHandleProps         GftvHandleProps;

struct  _GftvHandleProps
{
    gdouble     gfx_l_alloc;                                                    //!< Length sense : Pixels allocated for drawing

    gdouble     gfx_l_space_before;                                             //!< Length sense : Empty space before handle
    gdouble     gfx_l_space_after;                                              //!< Length sense : Empty space after  handle

    gdouble     gfx_l_size;                                                     //!< Length sense : handle size
    gdouble     gfx_l_start;                                                    //!< Length sense : pixel offset of handle
    gdouble     gfx_l_start_min;                                                //!< Length sense : minimal pixel offset of handle
    gdouble     gfx_l_start_max;                                                //!< Length sense : maximal pixel offset of handle

    //gdouble     gfx_t_alloc;
    //gdouble     gfx_t_size;
    //gdouble     gfx_t_start;

    gdouble     x;                                                              //!< Handle coords, memorized for mouse clicks
    gdouble     y;                                                              //!< Handle coords, memorized for mouse clicks
    gdouble     w;                                                              //!< Handle coords, memorized for mouse clicks
    gdouble     h;                                                              //!< Handle coords, memorized for mouse clicks

    gboolean    drag_on;                                                        //!< Mouse button1 state memorization
    gdouble     drag_start_pos;                                                 //!< Intermediate variable in handle drag process

    guint32     offset;                                                         //!< Handle offset in the model ( not in pixels )
};

typedef struct _GtkFastTextViewPrivate  GtkFastTextViewPrivate;

struct _GtkFastTextViewPrivate
{
    GwrFastTextBuffer    *   buffer;                                            //!< The buffer we get text to display from

    guint8                  cR[32];                                             //!< Colors allowed for display, Red  component
    guint8                  cG[32];                                             //!< Colors allowed for display, Green component
    guint8                  cB[32];                                             //!< Colors allowed for display, Blue  component

    int                     alloc_width;                                        //!< Widht  ( in pixels ) available for drawing
    int                     alloc_height;                                       //!< Height ( in pixels ) available for drawing

    guint32                 text_area_width;                                    //!< Width  ( in pixels ) avilable for text drawing
    guint32                 text_area_height;                                   //!< Height ( in pixels ) avilable for text drawing

    guint32                 font_size;                                          //!< Font size as set by the user
    gboolean                font_size_changed;                                  //!< Flag
    guint32                 font_width;                                         //!< Font width  in pixels
    guint32                 font_height;                                        //!< Font height in pixels

    guint16                 width_char_x;                                       //!< # of chars the widget can display
    guint16                 width_char_y;                                       //!< # of lines the widget can display

    GftvHandleProps         vhp;                                                //!< Vertical   handle properties
    GftvHandleProps         hhp;                                                //!< Horizontal handle properties
};

static  void        gtk_fast_text_view_get_preferred_width (GtkWidget *widget, int *minimal, int *natural);
static  void        gtk_fast_text_view_get_preferred_height(GtkWidget *widget, int *minimal, int *natural);

static  void        gtk_fast_text_view_realize             (GtkWidget*);
static  void        gtk_fast_text_view_size_allocate       (GtkWidget*, GtkAllocation*);
static  gboolean    gtk_fast_text_view_draw                (GtkWidget*, cairo_t*);
//  ****************************************************************************
//
//  Forward declarations - NON-GTK
//
//  ****************************************************************************
static  void        gtk_fast_text_view_recompute                    (GtkFastTextView*);
static  void        gtk_fast_text_view_compute_scrollbars           (GtkFastTextViewPrivate*, GwrFastTextBufferPrivate*);
static  void        gtk_fast_text_view_compute_font_dimensions      (GtkFastTextViewPrivate*, cairo_t*);
static  void        gtk_fast_text_view_compute_chars_capacity       (GtkFastTextViewPrivate*);

static  void        gtk_fast_text_view_scroll_vertical_up           (GtkFastTextView*, guint32 _ymove);
static  void        gtk_fast_text_view_scroll_vertical_down         (GtkFastTextView*, guint32 _ymove);
//  ----------------------------------------------------------------------------
static  void        gtk_fast_text_view_SIGNAL_map                   (GtkWidget*, gpointer                 );

static  gboolean    gtk_fast_text_view_SIGNAL_key_release_event     (GtkWidget*, GdkEventKey*   , gpointer);

static  gboolean    gtk_fast_text_view_SIGNAL_button_press_event    (GtkWidget*, GdkEvent*      , gpointer);
static  gboolean    gtk_fast_text_view_SIGNAL_button_release_event  (GtkWidget*, GdkEvent*      , gpointer);
static  gboolean    gtk_fast_text_view_SIGNAL_motion_notify_event   (GtkWidget*, GdkEvent*      , gpointer);
//  ****************************************************************************
//
//  GTK stuff
//
//  ****************************************************************************
//  Define the GtkFastTextView type and inherit from GtkContainer
G_DEFINE_TYPE(GtkFastTextView, gtk_fast_text_view, GTK_TYPE_WIDGET);
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_class_init()
//  ----------------------------------------------------------------------------
static void
gtk_fast_text_view_class_init(GtkFastTextViewClass* _klass)
{
    // Override GtkWidget methods
    GtkWidgetClass* widget_class        = GTK_WIDGET_CLASS(_klass);
    widget_class->get_preferred_width   = gtk_fast_text_view_get_preferred_width;  //!< Minimal & natural sizes
    widget_class->get_preferred_height  = gtk_fast_text_view_get_preferred_height; //!< Minimal & natural sizes
    widget_class->size_allocate         = gtk_fast_text_view_size_allocate;        //!< For computing gtk_fast_text_view_draw()'s parameters
    widget_class->draw                  = gtk_fast_text_view_draw;                 //!< For drawing the model's content
    widget_class->realize               = gtk_fast_text_view_realize;              //!< For creating a GdkWindow

    //  Override GtkContainer methods : empty

    //  Add private indirection member
	g_type_class_add_private(_klass, sizeof(GtkFastTextViewPrivate));
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_init()
//  ----------------------------------------------------------------------------
static void
gtk_fast_text_view_init(GtkFastTextView *_textview)
{
    // This means that GtkFastTextView supply its own GdkWindow
	gtk_widget_set_has_window(GTK_WIDGET(_textview), TRUE);

    // WE DONT DO THIS BECAUSE OF SCROLLBARS : THEY CHANGE ON SIZE ALLOCATION
	//      Set redraw on allocate to FALSE if the top left corner of your widget
	//      doesn't change when it's resized; this saves time
	//      gtk_widget_set_redraw_on_allocate(GTK_WIDGET(_textview), FALSE);

	/* Initialize private members */
	GtkFastTextViewPrivate *priv = GTK_FAST_TEXT_VIEW_PRIVATE(_textview);

    bzero( priv, sizeof(GtkFastTextViewPrivate) );

    priv->font_size_changed =   TRUE;
    priv->font_size         =   8;
    priv->font_width        =   0.0;
    priv->font_height       =   0.0;

    priv->vhp.drag_on      =   FALSE;
    priv->vhp.offset       =   0;

    priv->hhp.drag_on      =   FALSE;
    priv->hhp.offset       =   0;

    gtk_widget_set_can_focus( GTK_WIDGET(_textview), TRUE );

    g_signal_connect( _textview, "map"                  , G_CALLBACK(gtk_fast_text_view_SIGNAL_map)                , (gpointer)_textview);
    g_signal_connect( _textview, "key-release-event"    , G_CALLBACK(gtk_fast_text_view_SIGNAL_key_release_event)  , (gpointer)_textview);

    g_signal_connect( _textview, "button-press-event"   , G_CALLBACK(gtk_fast_text_view_SIGNAL_button_press_event)      , (gpointer)_textview);
    g_signal_connect( _textview, "button-release-event" , G_CALLBACK(gtk_fast_text_view_SIGNAL_button_release_event)    , (gpointer)_textview);
    g_signal_connect( _textview, "motion-notify-event"  , G_CALLBACK(gtk_fast_text_view_SIGNAL_motion_notify_event)     , (gpointer)_textview);
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_new()
//  ----------------------------------------------------------------------------
GtkWidget *
gtk_fast_text_view_new(GwrFastTextBuffer* _b)
{
    gpointer    *   p   =   NULL;
    //  ........................................................................
    g_return_val_if_fail( _b, NULL );

    p   =   g_object_new(gtk_fast_text_view_get_type(), NULL);

    gtk_fast_text_view_set_buffer( GTK_FAST_TEXT_VIEW(p), _b );

    return GTK_WIDGET( p );
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_get_preferred_width()
//  ----------------------------------------------------------------------------
static void
gtk_fast_text_view_get_preferred_width(GtkWidget *widget, int *minimal, int *natural)
{
	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_FAST_TEXT_VIEW(widget));

	//get_size(GWRGTK_TEXTVIEW(widget), GTK_ORIENTATION_HORIZONTAL, minimal, natural);
    *minimal    = 100;
    *natural    = 300;
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_get_preferred_height()
//  ----------------------------------------------------------------------------
static void
gtk_fast_text_view_get_preferred_height(GtkWidget *widget, int *minimal, int *natural)
{
	g_return_if_fail(widget != NULL);
	g_return_if_fail(GTK_IS_FAST_TEXT_VIEW(widget));

	//get_size(GWRGTK_TEXTVIEW(widget), GTK_ORIENTATION_VERTICAL, minimal, natural);
    *minimal    = 100;
    *natural    = 300;
}
//  ============================================================================
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_realize()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_realize()
//!
//! \brief  Here we create a GdkWindow and set our events.
void            gtk_fast_text_view_realize(GtkWidget* _w)
{
    GtkAllocation       allocation;
    GdkWindow       *   window;
    GdkWindowAttr       attributes;
    gint                attributes_mask;
    //  ........................................................................
    if ( ! gtk_widget_get_has_window (_w) )
    {
        GTK_WIDGET_CLASS (gtk_fast_text_view_parent_class)->realize (_w);
    }
    else
    {
        gtk_widget_set_realized( _w, TRUE );

        gtk_widget_get_allocation( _w, &allocation );

        attributes.window_type  = GDK_WINDOW_CHILD;
        attributes.x            = allocation.x;
        attributes.y            = allocation.y;
        attributes.width        = allocation.width;
        attributes.height       = allocation.height;
        attributes.wclass       = GDK_INPUT_OUTPUT;
        attributes.visual       = gtk_widget_get_visual (_w);
        attributes.event_mask   = gtk_widget_get_events (_w)        |
                                    GDK_EXPOSURE_MASK               |
                                    GDK_KEY_RELEASE_MASK            |
                                    GDK_BUTTON_PRESS_MASK           |
                                    GDK_BUTTON_RELEASE_MASK         |
                                    GDK_POINTER_MOTION_MASK         |
                                    GDK_POINTER_MOTION_HINT_MASK    ;

        attributes_mask         = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;

        window  = gdk_window_new(
            gtk_widget_get_parent_window(_w)        ,
            &attributes                             ,
            attributes_mask                         );

        //  GdkWindow points on GtkWidget
        #if GWR_GTK_VERSION_GE( 3, 8 )
        gtk_widget_register_window  ( _w, window );
        #else
        gdk_window_set_user_data    ( window, (gpointer)_w );
        #endif

        //  GtkWidget points on GdkWindow
        gtk_widget_set_window       ( _w, window );

        gtk_style_context_set_background(
            gtk_widget_get_style_context (_w)   ,
            window                              );
    }
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_size_allocate()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_size_allocate()
//!
//! \brief  GTK calls us when our allocation changes.
void            gtk_fast_text_view_size_allocate(GtkWidget* _w, GtkAllocation* _allocation)
{
    GtkFastTextView          *   v  =   NULL;
    GtkFastTextViewPrivate   *   vp =   NULL;
    //  ........................................................................
    g_return_if_fail( GTK_IS_FAST_TEXT_VIEW(_w) );
    //  ........................................................................
	gtk_widget_set_allocation(_w, _allocation);                                 //  mandatory
    //  ........................................................................
    v   =   GTK_FAST_TEXT_VIEW          ( _w );
    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE  ( _w );

    vp->alloc_width     =   _allocation->width;
    vp->alloc_height    =   _allocation->height;

    //  update GdkWindow too
    if ( gtk_widget_get_realized(_w) )
    {
        if ( gtk_widget_get_has_window(_w) )
        {
            gdk_window_move_resize(
                gtk_widget_get_window (_w)  ,
                _allocation->x              ,
                _allocation->y              ,
                _allocation->width          ,
                _allocation->height         );

            //  we dont send a configure event, we just call
            //  gtk_fast_text_view_recompute_adjustments() below. It's faster.
            //
            //  gtk_drawing_area_send_configure (GTK_DRAWING_AREA (widget));
        }
    }

    gtk_fast_text_view_recompute(v);

    //printf("gtk_fast_text_view_size_allocate():%i %i %i %i\n", _allocation->x, _allocation->y, _allocation->width, _allocation->height);
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_draw()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_draw()
//!
//! \brief  GTK calls us for drawing our stuff.
gboolean        gtk_fast_text_view_draw(GtkWidget *widget, cairo_t *cr)
{
    static  gchar                       text    [4096];

    static  GtkFastTextViewPrivate  *   vp      =   NULL;
    static  GwrFastTextBuffer       *   b       =   NULL;
    static  GwrFastTextBufferPrivate*   bp      =   NULL;
    static  GftvHandleProps         *   vhp     =   NULL;
    static  GftvHandleProps         *   hhp     =   NULL;

    static  cairo_surface_t         *   surface;

    GwrFastTextBufferLine               line;
    guint16                             ll;

    static  double                      clip_x1, clip_y1, clip_x2, clip_y2;     //  cairo drawing area
    static  double                      clip_width, clip_height;                //  cairo drawing area

    static  guint32                     x0, w3;
    static  guint32                     y0;
    static  guint32                     i       =   0;

    static  double                      sb_area_x   =   0.0;                    //  vertical scrollbar delimiter
    static  double                      sb_area_y   =   0.0;                    //  horizontal scrollbar delimiter

    static  double                      dashes[]    =   { 3.0, 3.0 };
    //  ........................................................................
    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(widget);
    //printf("gtk_fast_text_view_draw():sx=%f sy=%f\n", vp->scroll_pos_x, vp->scroll_pos_y);
    //  ........................................................................
    //  buffer private part
    b   =   vp->buffer;
    bp  =   (GwrFastTextBufferPrivate*)b->priv;
    //  ........................................................................
    vhp =   &( vp->vhp );
    hhp =   &( vp->hhp );
    //  ........................................................................
    //  drawing setup
    cairo_clip_extents(cr,&clip_x1, &clip_y1, &clip_x2, &clip_y2);              //  get window dimensions ( pixels )
    clip_width  =   clip_x2 - clip_x1 + 1.0 - 1.0;
    clip_height =   clip_y2 - clip_y1 + 1.0 - 1.0;                              //  -1.0 : the last row is for gtk window resizing
    surface     =   cairo_get_target( cr );
    //  ........................................................................
    //  font sizes
    cairo_select_font_face  (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size     (cr, vp->font_size);

    //  if font size changed, recompute extents and associated values
    if ( vp->font_size_changed )
    {
        //printf("gtk_fast_text_view_draw():font size changed\n");

        vp->font_size_changed   =   FALSE;

        gtk_fast_text_view_compute_font_dimensions(vp, cr);
        gtk_fast_text_view_compute_chars_capacity(vp);
    }
    //printf("gtk_fast_text_view_draw():fx=%i wx=%i\n", vp->font_width, vp->width_char_x);
    //  ........................................................................
    //  scrollbars drawing setup
    sb_area_x   =   clip_width  >   GTK_FAST_TEXT_VIEW_VBAR_AREA_THICKNESS            ?
                    ( clip_x1 + clip_width ) - GTK_FAST_TEXT_VIEW_VBAR_AREA_THICKNESS :
                    0;
    sb_area_y   =   clip_height >   GTK_FAST_TEXT_VIEW_HBAR_AREA_THICKNESS            ?
                    ( clip_y1 + clip_height) - GTK_FAST_TEXT_VIEW_HBAR_AREA_THICKNESS :
                    0;

    vhp->x      =   sb_area_x + 1.0;
    vhp->y      =   vhp->gfx_l_start;
    vhp->w      =   GTK_FAST_TEXT_VIEW_VBAR_AREA_THICKNESS - 3.0;
    vhp->h      =   vhp->gfx_l_size;

    hhp->x      =   hhp->gfx_l_start;
    hhp->y      =   sb_area_y + 1.0;
    hhp->w      =   hhp->gfx_l_size;
    hhp->h      =   GTK_FAST_TEXT_VIEW_HBAR_AREA_THICKNESS - 2.0;
    //  ........................................................................
    //  ScrollBars drawing
    cairo_set_line_width( cr, 1.0 );
    cairo_set_source_rgb(cr, 0.5, 0.5, 1);

    //  VScrollBar
    cairo_rectangle( cr, vhp->x + 0.5, vhp->y + 0.5, vhp->w, vhp->h );          //  0.5 for having sharp lines
    cairo_fill(cr);

    //  HScrollBar
    cairo_rectangle( cr, hhp->x + 0.5, hhp->y + 0.5, hhp->w, hhp->h );          //  0.5 for having sharp lines
    cairo_fill(cr);
    //  ........................................................................
    //  Separators drawing
    cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);

    //  vertical separator
    cairo_set_dash( cr, dashes, 2, 0.0 );
    cairo_move_to( cr, sb_area_x - 0.5, 0.0                             );      //  0.5 for having sharp lines
    cairo_line_to( cr, sb_area_x - 0.5, clip_y1 + clip_height   - 1.0   );      //  0.5 for having sharp lines
    cairo_stroke(cr);

    //  horizontal separator
    cairo_set_dash( cr, dashes, 2, 0.0 );
    cairo_move_to( cr, 0.0                     , sb_area_y - 0.5    );          //  0.5 for having sharp lines
    cairo_line_to( cr, clip_x1 + clip_width - 1, sb_area_y - 0.5    );          //  0.5 for having sharp lines
    cairo_stroke(cr);
    //  ........................................................................
    //  text drawing setup
    x0  =   (guint32)hhp->offset;                                               //  compute x0 = first char ( in each line ) to be displayed
    y0  =   (guint32)vhp->offset;                                               //  compute y0 = first line to be displayed
    //  ........................................................................
    //  text draw
    for ( i = 0 ; i != vp->width_char_y ; i++ )
    {
        if ( ! gwr_fast_text_buffer_get_line( b, y0 + i, &line ) )              //  no more lines
            break;

        if ( line.a_attr.a_xd )
        {
            printf("xd:%i\n", line.a_attr.a_xd);
        }

        ll = line.a_str_len;

        if ( x0 > (guint32)( ll - 1 ) )                                         //  not enough chars in string to reach the cairo clip
            continue;

        w3  =   MIN( (guint32)( ll - x0 ), vp->width_char_x );                  //  # of chars to be displayed

        memcpy( text, line.a_str + x0, w3 );                                    //  copy text
        text[w3] = 0;                                                           //  add final 0

        //  cairo rgb are between 0.0 and 1.0
        cairo_set_source_rgb(
            cr                                              ,
            (gdouble)vp->cR[ line.a_attr.a_fg ] / 256.0     ,
            (gdouble)vp->cG[ line.a_attr.a_fg ] / 256.0     ,
            (gdouble)vp->cB[ line.a_attr.a_fg ] / 256.0     );

        cairo_move_to           (cr, 1.0, 8.0 + i * vp->font_height);
        cairo_show_text         (cr, text);

        //printf("fg:%i bg:%i [%s]\n", line.a_fg, line.a_bg, text);
    }

    return TRUE;
}
//  ****************************************************************************
//
//  non-GTK stuff
//
//  ****************************************************************************
//  ============================================================================
//  RECOMPUTE
//  ============================================================================
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_compute_font_dimensions()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_compute_font_dimensions()
//!
//! \brief  Compute font dimensions.
static  void    gtk_fast_text_view_compute_font_dimensions(
    GtkFastTextViewPrivate  *   _vp    ,
    cairo_t                 *   _cr     )
{
    static  cairo_font_extents_t        extents;
    //  ........................................................................
    cairo_font_extents      ( _cr, &extents );

    _vp->font_height        =   (guint32)( extents.height          );
    _vp->font_width         =   (guint32)( extents.max_x_advance   );

    //printf("gtk_fast_text_view_compute_font_dimensions():fw=%i\n", _vp->font_width);
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_compute_scrollbars()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_compute_scrollbars()
//!
//! \brief  Compute scrollbars lengths & start offsets.
static  void    gtk_fast_text_view_compute_scrollbars(
    GtkFastTextViewPrivate      *   _vp     ,
    GwrFastTextBufferPrivate    *   _bp     )
{
    gdouble                 r   =   0.0;
    GftvHandleProps     *   vhp =   &( _vp->vhp );
    GftvHandleProps     *   hhp =   &( _vp->hhp );
    //  ........................................................................
    //  allocations
    vhp->gfx_l_space_before =   1.0;
    vhp->gfx_l_space_after  =   1.0;

    hhp->gfx_l_space_before =   1.0;
    hhp->gfx_l_space_after  =   1.0;

    vhp->gfx_l_alloc    =   MAX(
        _vp->alloc_height
            - vhp->gfx_l_space_before                                           //  top space
            - vhp->gfx_l_space_after                                            //  bottom space
            - 1.0                                                               //  HScrollBar separator
            - GTK_FAST_TEXT_VIEW_HBAR_AREA_THICKNESS                            //  HScrollBar space
            - 1.0   ,                                                           //  gtk resizing keep last line for him
        0.0         );

    hhp->gfx_l_alloc    =   MAX(
        _vp->alloc_width
            - hhp->gfx_l_space_before                                           //  left space
            - hhp->gfx_l_space_after                                            //  right space
            - 1.0                                                               //  VScrollBar separator
            - GTK_FAST_TEXT_VIEW_VBAR_AREA_THICKNESS                            //  VScrollBar space
            - 1.0   ,                                                           //  gtk resizing keep last line for him
        0.0         );
    //  ........................................................................
    //  vertical scrollbar
    if ( _bp->lines_card )
    {
        r                           =   (gdouble)_vp->width_char_y / (gdouble)_bp->lines_card;

        vhp->gfx_l_size =   r * (gdouble)vhp->gfx_l_alloc;
        vhp->gfx_l_size =   floor( vhp->gfx_l_size );                           //  work with rounded pixels coords

        GWR_BOUND_LE( vhp->gfx_l_size, (gdouble)vhp->gfx_l_alloc            );
        GWR_BOUND_GE( vhp->gfx_l_size, GTK_FAST_TEXT_VIEW_HANDLE_LENGTH_MIN );

        //  0 <= r <= 1
        if ( _bp->lines_card > 1 )
            r                       =   (gdouble)vhp->offset / ( (gdouble)_bp->lines_card - 1.0 );
        else
            r                       =   0.0;

        vhp->gfx_l_start        =   ( vhp->gfx_l_alloc - vhp->gfx_l_size ) * r;
        vhp->gfx_l_start        =   floor( vhp->gfx_l_start );                  //  work with rounded pixels coords
        vhp->gfx_l_start        =   vhp->gfx_l_start + 1.0;

        vhp->gfx_l_start_min    =   vhp->gfx_l_space_before;
        vhp->gfx_l_start_max    =   vhp->gfx_l_alloc - vhp->gfx_l_size + vhp->gfx_l_space_before;
    }
    else
    {
        vhp->gfx_l_size         =   vhp->gfx_l_alloc;
        vhp->gfx_l_start        =   vhp->gfx_l_start_min;
    }
    //printf("compute scrollbars:V:pos=%f size=%f step=%f\n", _vp->vhp.gfx_l_start, _vp->vhp.gfx_l_size, _vp->vhp.inc_step);
    //  ........................................................................
    //  horizontal scrollbar
    if ( _bp->lines_card )
    {
        r                           =   (gdouble)_vp->width_char_x / (gdouble)_bp->lines_max_len;

        hhp->gfx_l_size =   r * (gdouble)hhp->gfx_l_alloc;
        hhp->gfx_l_size =   floor( hhp->gfx_l_size );                           //  work with rounded pixels coords

        GWR_BOUND_LE( hhp->gfx_l_size, (gdouble)hhp->gfx_l_alloc            );
        GWR_BOUND_GE( hhp->gfx_l_size, GTK_FAST_TEXT_VIEW_HANDLE_LENGTH_MIN );

        if ( _bp->lines_max_len > 1 )
            r                       =   (gdouble)hhp->offset / ( (gdouble)_bp->lines_max_len );
        else
            r                       =   0.0;

        hhp->gfx_l_start        =   ( hhp->gfx_l_alloc - hhp->gfx_l_size ) * r;
        hhp->gfx_l_start        =   floor( hhp->gfx_l_start );                  //  work with rounded pixels coords
        hhp->gfx_l_start        =   hhp->gfx_l_start + 1.0;

        hhp->gfx_l_start_min    =   hhp->gfx_l_space_before;
        hhp->gfx_l_start_max    =   hhp->gfx_l_alloc - hhp->gfx_l_size + hhp->gfx_l_space_before;
    }
    else
    {
        hhp->gfx_l_size         =   hhp->gfx_l_alloc;
        hhp->gfx_l_start        =   hhp->gfx_l_start_min;
    }
    //printf("compute scrollbars:V:pos=%f size=%f step=%f\n", _vp->vhp.gfx_l_start, _vp->vhp.gfx_l_size, _vp->vhp.inc_step);
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_compute_chars_capacity()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_compute_chars_capacity()
//!
//! \brief  Compute the number of chars the widget can display from
//!         screen allocation.
static  void    gtk_fast_text_view_compute_chars_capacity(
    GtkFastTextViewPrivate   *   _vp     )
{
    _vp->text_area_width    =   MAX(
        _vp->alloc_width
            - 1.0                                                               //  left space
            - 1.0                                                               //  right space
            - 1.0                                                               //  VScrollBar separator
            - GTK_FAST_TEXT_VIEW_VBAR_AREA_THICKNESS                            //  VScrollBar space
            - 1.0   ,                                                           //  gtk resizing keep last line for him
        0.0         );

    _vp->text_area_height   =   MAX(
        _vp->alloc_height
            - 1.0                                                               //  top space
            - 1.0                                                               //  bottom space
            - 1.0                                                               //  HScrollBar separator
            - GTK_FAST_TEXT_VIEW_HBAR_AREA_THICKNESS                            //  HScrollBar space
            - 1.0   ,                                                           //  gtk resizing keep last line for him
        0.0         );

    _vp->width_char_x       =   _vp->text_area_width    / _vp->font_width;
    _vp->width_char_y       =   _vp->text_area_height   / _vp->font_height;
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_recompute()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_recompute()
//!
//! \brief  Recompute all.
static  void    gtk_fast_text_view_recompute(
    GtkFastTextView          *   _v      )
{
    GtkFastTextViewPrivate   *   vp  =   NULL;
    GwrFastTextBufferPrivate *   bp  =   NULL;
    //  ........................................................................
    g_return_if_fail( GTK_IS_FAST_TEXT_VIEW(_v) );

    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE  ( _v                    );
    bp  =   (GwrFastTextBufferPrivate*) ( vp->buffer->priv      );

    //  we may be called before realization ( ex by gtk_fast_text_view_refresh()
    //  when the widget has not been yet displayed but some text was added
    //  to the buffer )
    if ( ! gtk_widget_get_realized( GTK_WIDGET( _v ) ) )
        return;
    //  ........................................................................
    //  text relative computations
    gtk_fast_text_view_compute_chars_capacity(vp);
    //  ........................................................................
    //  scrollbars relative computations
    gtk_fast_text_view_compute_scrollbars(vp, bp);
}
//  ----------------------------------------------------------------------------
void            gtk_fast_text_view_set_color            (GtkFastTextView* _v, guint32 _color_index, guint8 _r, guint8 _g, guint8 _b)
{
    GtkFastTextViewPrivate  *   vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(_v);
    //  ........................................................................
    g_return_if_fail( _color_index < 32 );

    vp->cR[_color_index]    =   _r;
    vp->cG[_color_index]    =   _g;
    vp->cB[_color_index]    =   _b;
}
void            gtk_fast_text_view_scroll_vertical_up   (GtkFastTextView* _v, guint32 _ymove)
{
    GtkFastTextViewPrivate  *   vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(_v);
    //  ........................................................................
    if ( vp->vhp.offset < _ymove )
        vp->vhp.offset  =   0;
    else
        vp->vhp.offset  =   vp->vhp.offset - _ymove;

    gtk_fast_text_view_compute_scrollbars(vp, (GwrFastTextBufferPrivate*)vp->buffer->priv);
    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
void            gtk_fast_text_view_scroll_vertical_down (GtkFastTextView* _v, guint32 _ymove)
{
    GtkFastTextViewPrivate  *   vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(_v);
    GwrFastTextBuffer        *   b   =   vp->buffer;
    GwrFastTextBufferPrivate *   bp  =   (GwrFastTextBufferPrivate*)(b->priv);
    //  ........................................................................
    if ( ( vp->vhp.offset  + _ymove ) > ( bp->lines_card - vp->width_char_y ) )
        vp->vhp.offset  =   bp->lines_card - vp->width_char_y;
    else
        vp->vhp.offset  =   vp->vhp.offset + _ymove;

    gtk_fast_text_view_compute_scrollbars(vp, (GwrFastTextBufferPrivate*)vp->buffer->priv);
    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
//  ============================================================================
//  SIGNALS
//  ============================================================================
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_SIGNAL_map()
//  ----------------------------------------------------------------------------
static  void        gtk_fast_text_view_SIGNAL_map(
    GtkWidget       *   _w      ,
    gpointer            _data   )
{
    GtkFastTextView          *   v   =   NULL;
    GtkFastTextViewPrivate   *   vp  =   NULL;

    GdkWindow               *   w   =   NULL;
    cairo_surface_t         *   s   =   NULL;
    cairo_t                 *   c   =   NULL;
    //  ........................................................................
    //  "map" signal is connected because gtk_fast_text_view_draw() can be called
    //  before gtk_fast_text_view_recompute_adjustments() or gtk_fast_text_view_compute_chars_capacity().
    //  So at first display of widget, we do a whole computation

    GdkWindow* ww = gtk_widget_get_window (_w);

    printf("gtk_fast_text_view_SIGNAL_map():%p\n", ww);

    g_return_if_fail( GTK_IS_FAST_TEXT_VIEW(_w) );

    v   =   GTK_FAST_TEXT_VIEW( _w );
    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE( v );

    w   =   gtk_widget_get_window (_w);
    s   =   gdk_window_create_similar_surface(w, CAIRO_CONTENT_COLOR, 10, 10);
    c   =   cairo_create(s);

    //  select font else cairo with use default font, sizes will be different
    //  from our settings
    cairo_select_font_face  (c, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size     (c, vp->font_size);

    gtk_fast_text_view_compute_font_dimensions(vp, c);
    gtk_fast_text_view_compute_chars_capacity(vp);

    cairo_destroy(c);
    cairo_surface_destroy(s);

    gtk_fast_text_view_recompute( v );
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_SIGNAL_key_release_event()
//  ----------------------------------------------------------------------------
static gboolean     gtk_fast_text_view_SIGNAL_key_release_event(
    GtkWidget       *   _w      ,
    GdkEventKey     *   _evt    ,
    gpointer            _data   )
{
    GtkFastTextView          *   v   =   NULL;
    GtkFastTextViewPrivate   *   vp  =   NULL;
    GwrFastTextBuffer        *   b   =   NULL;
    gboolean                    r   =   TRUE;
    //  ........................................................................
    v   =   GTK_FAST_TEXT_VIEW( _data );
    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE( v );
    b   =   vp->buffer;

    if ( !b )
        return TRUE;

    if ( _evt->keyval == GDK_KEY_Up )
    {
        //printf("gtk_fast_text_view_SIGNAL_key_release_event():UP\n");
        gtk_fast_text_view_scroll_vertical_up(v, 1);
        return r;
    }
    if ( _evt->keyval == GDK_KEY_Down )
    {
        //printf("gtk_fast_text_view_SIGNAL_key_release_event():DOWN\n");
        gtk_fast_text_view_scroll_vertical_down(v, 1);
        return r;
    }
    if ( _evt->keyval == GDK_KEY_Page_Up )
    {
        //printf("gtk_fast_text_view_SIGNAL_key_release_event():PAGEUP\n");
        gtk_fast_text_view_scroll_vertical_up(v, vp->width_char_y);
        return r;
    }
    if ( _evt->keyval == GDK_KEY_Page_Down )
    {
        //printf("gtk_fast_text_view_SIGNAL_key_release_event():PAGEDOWN\n");
        gtk_fast_text_view_scroll_vertical_down(v, vp->width_char_y);
        return r;
    }

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_SIGNAL_button_release_event()
//  ----------------------------------------------------------------------------
static  gboolean    gtk_fast_text_view_SIGNAL_button_release_event(
    GtkWidget       *   _w      ,
    GdkEvent        *   _evt    ,
    gpointer            _data   )
{
    GdkEventButton          *   evb =   NULL;
    GtkFastTextView         *   v   =   NULL;
    GtkFastTextViewPrivate  *   vp  =   NULL;
    //  ........................................................................
    //printf("VIEW:button-release\n");
    //  ........................................................................
    evb =   (GdkEventButton*)_evt;
    v   =   (GtkFastTextView*)_data;
    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(v);
    //  ........................................................................
    vp->vhp.drag_on =   FALSE;
    vp->hhp.drag_on =   FALSE;

    return FALSE;
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_SIGNAL_button_press_event()
//  ----------------------------------------------------------------------------
static  gboolean    gtk_fast_text_view_SIGNAL_button_press_event(
    GtkWidget       *   _w      ,
    GdkEvent        *   _evt    ,
    gpointer            _data   )
{
    GdkEventButton          *   evb =   NULL;
    GtkFastTextView         *   v   =   NULL;
    GtkFastTextViewPrivate  *   vp  =   NULL;
    //  ........................................................................
    //printf("VIEW:button-press\n");
    //  ........................................................................
    evb =   (GdkEventButton*)_evt;
    v   =   (GtkFastTextView*)_data;
    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(v);
    //  ........................................................................
    //  only button 1 is intersting for us
    if ( evb->button != 1 )
        return FALSE;
    //  ........................................................................
    //  click on Vertical ScrollBar
    if  (
            ( evb->x >= vp->vhp.x ) && ( evb->x <= vp->vhp.x + vp->vhp.w )  &&
            ( evb->y >= vp->vhp.y ) && ( evb->y <= vp->vhp.y + vp->vhp.h )
        )
    {
        //printf("VBar x=%f y=%f\n", evb->x, evb->y);
        vp->vhp.drag_on         =   TRUE;
        vp->vhp.drag_start_pos  =   evb->y;
    }
    //  ........................................................................
    //  click on Horizontal ScrollBar
    if  (
            ( evb->x >= vp->hhp.x ) && ( evb->x <= vp->hhp.x + vp->hhp.w )  &&
            ( evb->y >= vp->hhp.y ) && ( evb->y <= vp->hhp.y + vp->hhp.h )
        )
    {
        //printf("HBar x=%f y=%f\n", evb->x, evb->y);
        vp->hhp.drag_on         =   TRUE;
        vp->hhp.drag_start_pos  =   evb->x;
    }

    //  false to propagate
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_SIGNAL_motion_notify_event()
//  ----------------------------------------------------------------------------
static  gboolean    gtk_fast_text_view_SIGNAL_motion_notify_event(
    GtkWidget       *   _w      ,
    GdkEvent        *   _evt    ,
    gpointer            _data   )
{
    GdkEventMotion          *   evm =   NULL;
    GtkFastTextView         *   v   =   NULL;
    GtkFastTextViewPrivate  *   vp  =   NULL;
    GwrFastTextBufferPrivate *   bp  =   NULL;
    //  ........................................................................
    //printf("VIEW:motion-notify\n");
    //  ........................................................................
    evm =   (GdkEventMotion*)_evt;
    v   =   (GtkFastTextView*)_data;
    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(v);
    bp  =   (GwrFastTextBufferPrivate*)(vp->buffer->priv);
    //  ........................................................................
    if ( vp->vhp.drag_on )
    {
        vp->vhp.gfx_l_start     =   vp->vhp.gfx_l_start + ( evm->y - vp->vhp.drag_start_pos );
        vp->vhp.gfx_l_start     =   floor( vp->vhp.gfx_l_start );               //  work with rounded pixels coords

        GWR_BOUND_LE( vp->vhp.gfx_l_start, vp->vhp.gfx_l_start_max );
        GWR_BOUND_GE( vp->vhp.gfx_l_start, vp->vhp.gfx_l_start_min );

        vp->vhp.drag_start_pos  =   evm->y;

        vp->vhp.offset          =   (gdouble)( bp->lines_card - vp->width_char_y )  *
                                    (
                                        (vp->vhp.gfx_l_start        - vp->vhp.gfx_l_start_min)
                                        /
                                        (vp->vhp.gfx_l_start_max    - vp->vhp.gfx_l_start_min)
                                    );

        //printf("VBar drag offset=%i\n", vp->vhp.offset);

        gtk_widget_queue_draw(GTK_WIDGET(_w));
    }
    //  ........................................................................
    if ( vp->hhp.drag_on )
    {
        vp->hhp.gfx_l_start     =   vp->hhp.gfx_l_start + ( evm->x - vp->hhp.drag_start_pos );
        vp->hhp.gfx_l_start     =   floor( vp->hhp.gfx_l_start );               //  work with rounded pixels coords

        GWR_BOUND_LE( vp->hhp.gfx_l_start, vp->hhp.gfx_l_start_max );
        GWR_BOUND_GE( vp->hhp.gfx_l_start, vp->hhp.gfx_l_start_min );

        vp->hhp.drag_start_pos  =   evm->x;

        vp->hhp.offset          =   (gdouble)( bp->lines_max_len - vp->width_char_x )  *
                                    (
                                        (vp->hhp.gfx_l_start        - vp->hhp.gfx_l_start_min)
                                        /
                                        (vp->hhp.gfx_l_start_max    - vp->hhp.gfx_l_start_min)
                                    );

        gtk_widget_queue_draw(GTK_WIDGET(_w));
        //printf("HBar drag %f\n", evm->x - vp->hhp.pos);
    }
    //  ........................................................................
    return FALSE;
}
//  ============================================================================
void                gtk_fast_text_view_set_buffer           (GtkFastTextView* _w, GwrFastTextBuffer* _b)
{
    GTK_FAST_TEXT_VIEW_PRIVATE(_w)->buffer     =   _b;
}
GwrFastTextBuffer*  gtk_fast_text_view_get_buffer           (GtkFastTextView* _w)
{
    return (GwrFastTextBuffer*)( GTK_FAST_TEXT_VIEW_PRIVATE(_w)->buffer );
}
void                gtk_fast_text_view_set_font_size        (GtkFastTextView* _w, guint32 _size)
{
    GTK_FAST_TEXT_VIEW_PRIVATE(_w)->font_size  =   _size;
}
//  ----------------------------------------------------------------------------
void                gtk_fast_text_view_refresh              (GtkFastTextView* _v)
{
    gtk_fast_text_view_recompute(_v);
    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
