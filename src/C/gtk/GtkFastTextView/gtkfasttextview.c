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
                                                                                /*
--------------------------------------------------------------------------------
                            GENERAL REMARKS

*   We connect to "key-press-event" only to keep the focus, by returning
    TRUE from our handler

--------------------------------------------------------------------------------
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

typedef struct  _GftvGfxLine    GftvGfxLine;

struct  _GftvGfxLine
{
    guint8      a_has_data;
    guint8      a_has_url;

    guint16     a_url_x;
    guint16     a_url_y;
    guint16     a_url_w;
    guint16     a_url_h;
}  __attribute__((packed));


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
    GwrFastTextBuffer   *   buffer;                                             //!< The buffer we get text to display from

    GdkWindow           *   a_gdk_window;
    guint32                 a_cursor_current;

    GtkFastTextViewXdCallback   a_xd_callback;

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
    guint32                 font_uw;                                            //!< Withdrawal for html underline

    GwrCArrayEqualSimple*   d_gfx_lines;                                        //!< Memorized graphical lines infos

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

static  void        gtk_fast_text_view_dispose              (GObject* object);
static  void        gtk_fast_text_view_finalize             (GObject* object);
//  ****************************************************************************
//
//  Forward declarations - NON-GTK
//
//  ****************************************************************************
static  void        gtk_fast_text_view_recompute                    (GtkFastTextView*);
static  void        gtk_fast_text_view_compute_0_font_dimensions    (GtkFastTextViewPrivate*, cairo_t*);
static  void        gtk_fast_text_view_compute_1_chars_capacity     (GtkFastTextViewPrivate*);
static  void        gtk_fast_text_view_compute_2_check_offsets      (GtkFastTextViewPrivate*, GwrFastTextBufferPrivate*);
static  void        gtk_fast_text_view_compute_3_scrollbars         (GtkFastTextViewPrivate*, GwrFastTextBufferPrivate*);

static  void        gtk_fast_text_view_show_cursor_normal           (GtkWidget*, GtkFastTextViewPrivate*);
static  void        gtk_fast_text_view_show_cursor_data             (GtkWidget*, GtkFastTextViewPrivate*);
static  void        gtk_fast_text_view_show_cursor_url              (GtkWidget*, GtkFastTextViewPrivate*);

static  void        gtk_fast_text_view_scroll_vertical_up           (GtkFastTextView*, guint32 _ymove);
static  void        gtk_fast_text_view_scroll_vertical_down         (GtkFastTextView*, guint32 _ymove);
//  ----------------------------------------------------------------------------
static  void        gtk_fast_text_view_SIGNAL_map                   (GtkWidget*, gpointer                 );

static  gboolean    gtk_fast_text_view_SIGNAL_key_press_event       (GtkWidget*, GdkEventKey*   , gpointer);
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
    GdkDisplay      *   display     =   gdk_display_get_default();
    GError          *   ge          =   NULL;
    //  ........................................................................
    // Override GtkWidget methods
    GtkWidgetClass* widget_class        = GTK_WIDGET_CLASS(_klass);
    widget_class->get_preferred_width   = gtk_fast_text_view_get_preferred_width;  //!< Minimal & natural sizes
    widget_class->get_preferred_height  = gtk_fast_text_view_get_preferred_height; //!< Minimal & natural sizes
    widget_class->size_allocate         = gtk_fast_text_view_size_allocate;        //!< For computing gtk_fast_text_view_draw()'s parameters
    widget_class->draw                  = gtk_fast_text_view_draw;                 //!< For drawing the model's content
    widget_class->realize               = gtk_fast_text_view_realize;              //!< For creating a GdkWindow

    G_OBJECT_CLASS(_klass)->dispose     = gtk_fast_text_view_dispose;
    G_OBJECT_CLASS(_klass)->finalize    = gtk_fast_text_view_finalize;

    _klass->cursors.normal      =   gdk_cursor_new( GDK_LEFT_PTR );
    _klass->cursors.xd_pixbuf   =   gdk_pixbuf_new_from_file("raw/img/hand2.png", &ge );
    if ( ! ge )
    {
        _klass->cursors.url     =   gdk_cursor_new_from_pixbuf(display, _klass->cursors.xd_pixbuf,0,0);
    }
    else
    {
        _klass->cursors.url     =   gdk_cursor_new( GDK_HAND2 );
    }
    _klass->cursors.data        =   gdk_cursor_new( GDK_CENTER_PTR );


    //  Override GtkContainer methods : empty

    //  Add private indirection member
	g_type_class_add_private(_klass, sizeof(GtkFastTextViewPrivate));

    //  Define a custom signal for extra data
    //sid =   g_signal_new(
    //            "gftw-extra-data-callback",
    //            G_TYPE_OBJECT, G_SIGNAL_RUN_FIRST,
    //            0, NULL, NULL,
    //            g_cclosure_marshal_VOID__POINTER,
    //            G_TYPE_NONE, 1, G_TYPE_POINTER);
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

    priv->vhp.drag_on       =   FALSE;
    priv->vhp.offset        =   0;

    priv->hhp.drag_on       =   FALSE;
    priv->hhp.offset        =   0;

    priv->a_cursor_current  =   0;

    priv->a_xd_callback     =   NULL;

    priv->d_gfx_lines       =   gwr_array_equal_simple_new(
                                    "GtkFastTextViewPrivate:url zones"  ,
                                    sizeof(GftvGfxLine)                 ,
                                    10                                  );

    gtk_widget_set_can_focus( GTK_WIDGET(_textview), TRUE );

    g_signal_connect( _textview, "map"                  , G_CALLBACK(gtk_fast_text_view_SIGNAL_map)                , (gpointer)_textview);
    g_signal_connect( _textview, "key-press-event"      , G_CALLBACK(gtk_fast_text_view_SIGNAL_key_press_event)    , (gpointer)_textview);
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
    if ( ! gtk_widget_get_has_window(_w) )                                      //  unuseful code
    {                                                                           //  since GtkFastTextView
        GTK_WIDGET_CLASS(gtk_fast_text_view_parent_class)->realize (_w);        //  has its own GdkWindow
    }                                                                           //  - keeped as sample code -
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

        GTK_FAST_TEXT_VIEW_PRIVATE( GTK_FAST_TEXT_VIEW(_w) )->a_gdk_window   =   window;
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
//  gtk_fast_text_view_dispose()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_dispose()
//!
//! \brief
void            gtk_fast_text_view_dispose(GObject* _obj)
{
    G_OBJECT_CLASS(gtk_fast_text_view_parent_class)->dispose(_obj);
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_finalize()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_finalize()
//!
//! \brief
void            gtk_fast_text_view_finalize(GObject* _obj)
{
    GtkFastTextView         *   v   =   NULL;
    GtkFastTextViewPrivate  *   vp  =   NULL;
    //  ........................................................................
    v   =   GTK_FAST_TEXT_VIEW(_obj);
    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(v);
    gwr_array_equal_simple_delete( vp->d_gfx_lines );

    G_OBJECT_CLASS(gtk_fast_text_view_parent_class)->finalize(_obj);
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

    static  GftvGfxLine                 gfx_line;
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

        gtk_fast_text_view_compute_0_font_dimensions(vp, cr);
        gtk_fast_text_view_compute_1_chars_capacity(vp);
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
    gwr_array_equal_simple_reset( vp->d_gfx_lines );

    for ( i = 0 ; i != vp->width_char_y ; i++ )
    {
        if ( ! gwr_fast_text_buffer_get_line_and_data( b, y0 + i, &line ) )     //  no more lines
            break;

        ll = line.a_str_len;

        if ( x0 > (guint32)( ll - 1 ) )                                         //  not enough chars in string to reach the cairo clip
            continue;

        w3  =   MIN( (guint32)( ll - x0 ), vp->width_char_x );                  //  # of chars to be displayed

        memcpy( text, line.a_str + x0, w3 );                                    //  copy text
        text[w3] = 0;                                                           //  add final 0

        //D guint8  rr  =     vp->cR[line.a_attr.a_fg];
        //D guint8  gg  =     vp->cG[line.a_attr.a_fg];
        //D guint8  bb  =     vp->cB[line.a_attr.a_fg];

        //  cairo rgb are between 0.0 and 1.0
        cairo_set_source_rgb(
            cr                                              ,
            (gdouble)vp->cR[ line.a_attr.a_fg ] / 256.0     ,
            (gdouble)vp->cG[ line.a_attr.a_fg ] / 256.0     ,
            (gdouble)vp->cB[ line.a_attr.a_fg ] / 256.0     );

        cairo_move_to           (cr, 1.0, vp->font_height + i * vp->font_height);
        cairo_show_text         (cr, text);
        //  ....................................................................
        //  gfx_line initialization : data ?
        gfx_line.a_has_data = 0;
        gfx_line.a_has_url  = 0;

        if ( ! gwr_fast_text_buffer_line_has_data( &line ) )
            goto lab_draw_add_gfx_line;

        gfx_line.a_has_data = 1;
        //  ....................................................................
        //  recolorize in html style ?
        if ( gwr_fast_text_buffer_line_has_html( &line ) )
        {
            guint16 h1  =   line.a_ho;
            guint16 h2  =   line.a_ho + line.a_hl - 1;
            guint16 x1,x2;

            //if ( ! ( ( h2 <  x0 ) || ( h1 >  x0 + w3 ) ) )
            if   (     ( h2 >= x0 ) && ( h1 <= x0 + w3 )   )                    //  html text intersect cairo clip
            {
                //D printf("html offset intersect:x0[%3i] h1[%3i] h2[%3i]\n", x0, h1, h2);

                x1  =   MAX( x0     , h1 );                                     //  compute intersection bounds
                x2  =   MIN( x0 + w3, h2 );
                //D printf("           clamped to:        x1[%3i] x2[%3i]\n", x1, x2);

                text[ x2 - x0 + 1 ] = 0;                                        //  just poke 0x00 at the good place, text is already copied
                //D printf("                 text:'%s'\n", text + x1 - x0);

                gfx_line.a_has_url  =   1;
                gfx_line.a_url_x    =   1.0 + vp->font_width * ( x1 - x0 );
                gfx_line.a_url_y    =   i * vp->font_height;
                gfx_line.a_url_w    =   vp->font_width * ( x2 - x1 + 1 );
                gfx_line.a_url_h    =   vp->font_height;

                cairo_set_source_rgb(                                           //  redraw text with html standard color
                    cr      ,
                    0.0     ,
                    0.0     ,
                    1.0     );
                cairo_move_to           (cr, 1.0 + vp->font_width * ( x1 - x0),vp->font_height + i * vp->font_height);
                cairo_show_text         (cr, text + x1 - x0);

                //D url zones
                //D cairo_set_dash          ( cr, NULL, 0, 0.0 );               //  draw the url zone
                //D cairo_rectangle         ( cr, ux, uy, uw, uh );
                //D cairo_stroke(cr);

                cairo_set_dash          ( cr, NULL, 0, 0.0 );                   //  draw the underline
                cairo_move_to           (
                    cr                                                          ,
                    1.0 + vp->font_width * ( x1 - x0    )                       ,
                    vp->font_height + i * vp->font_height + vp->font_uw + 0.5   );
                cairo_line_to           (
                    cr                                                          ,
                    1.0 + vp->font_width * ( x2 - x0 + 1 )                      ,
                    vp->font_height + i * vp->font_height + vp->font_uw + 0.5   );
                cairo_stroke(cr);
            }
        }

lab_draw_add_gfx_line:
        gwr_array_equal_simple_add_data( vp->d_gfx_lines, &gfx_line );
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
    gtk_fast_text_view_compute_1_chars_capacity(vp);
    //  ........................................................................
    //  check offset values
    gtk_fast_text_view_compute_2_check_offsets(vp, bp);
    //  ........................................................................
    //  scrollbars relative computations
    gtk_fast_text_view_compute_3_scrollbars(vp, bp);
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_compute_0_font_dimensions()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_compute_0_font_dimensions()
//!
//! \brief  Compute font dimensions.
static  void    gtk_fast_text_view_compute_0_font_dimensions(
    GtkFastTextViewPrivate  *   _vp    ,
    cairo_t                 *   _cr     )
{
    static  cairo_font_extents_t        extents;
    //  ........................................................................
    cairo_font_extents      ( _cr, &extents );

    _vp->font_height        =   (guint32)( extents.height          );
    _vp->font_width         =   (guint32)( extents.max_x_advance   );
    _vp->font_uw            =   (guint32)( extents.descent         );

    //printf("gtk_fast_text_view_compute_font_dimensions():fw=%i\n", _vp->font_width);
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_compute_1_chars_capacity()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_compute_1_chars_capacity()
//!
//! \brief  Compute the number of chars the widget can display from
//!         screen allocation.
static  void    gtk_fast_text_view_compute_1_chars_capacity(
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
//  gtk_fast_text_view_compute_2_check_offsets()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_compute_2_check_offsets()
//!
//! \brief  Correct offset following buffer & window size modifications.
static  void    gtk_fast_text_view_compute_2_check_offsets(
    GtkFastTextViewPrivate      *   _vp     ,
    GwrFastTextBufferPrivate    *   _bp     )
{
    static  GftvHandleProps         *   vhp     =   NULL;
    static  GftvHandleProps         *   hhp     =   NULL;
    //  ........................................................................
    vhp =   &( _vp->vhp );
    hhp =   &( _vp->hhp );
    //  ........................................................................
    //  vertical offset

    //  offset is after the buffer's text ! In this case, change the offset,
    //  trying to show _vp->width_char_y lines.
    if ( ( vhp->offset + 1 ) > _bp->lines_card )
    {
        //  new offset should be :
        //
        //      ( _bp->lines_card - 1 ) - _vp->width_char_y  + 1
        //  =   _bp->lines_card         - _vp->width_char_y
        //
        //  So we test if _bp->lines_card >= _vp->width_char_y                  (1)
        vhp->offset =   _bp->lines_card >= _vp->width_char_y    ?
                        _bp->lines_card -  _vp->width_char_y    :
                        0                                       ;
    }
    //  offset is in the the buffer's text
    else
    {
        //  if buffer's last line end is before last displayable line, decrease offset
        //
        //  ld  =   vhp->offset     +   _vp->width_char_y - 1                   ( last displayable line index )
        //  lb  =   _vp->lines_card -   1                                       ( last buffer line index )
        //
        //  So  we test  :
        //
        //      lb < ld
        //  <=> _vp->lines_card - 1 < vhp->offset + _vp->width_char_y - 1
        //  <=> _vp->lines_card     < vhp->offset + _vp->width_char_y
        if ( _bp->lines_card < ( vhp->offset + _vp->width_char_y ) )
        {
            vhp->offset =   _bp->lines_card >= _vp->width_char_y    ?           //  same as (1) above
                            _bp->lines_card -  _vp->width_char_y    :
                            0                                       ;
        }
    }
    //  ........................................................................
    //  horizontal offset same as above, so no comments

    if ( ( hhp->offset + 1 ) > _bp->lines_max_len )
    {
        hhp->offset =   _bp->lines_max_len >= _vp->width_char_x    ?
                        _bp->lines_max_len -  _vp->width_char_x    :
                        0;
    }
    else
    {
        if ( _bp->lines_max_len < ( hhp->offset + _vp->width_char_x ) )
        {
            hhp->offset =   _bp->lines_max_len >= _vp->width_char_x     ?
                            _bp->lines_max_len -  _vp->width_char_x     :
                            0                                           ;
        }
    }
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_compute_3_scrollbars()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_compute_3_scrollbars()
//!
//! \brief  Compute scrollbars lengths & start offsets.
static  void    gtk_fast_text_view_compute_3_scrollbars(
    GtkFastTextViewPrivate      *   _vp     ,
    GwrFastTextBufferPrivate    *   _bp     )
{
    gdouble                 r   =   0.0;
    GftvHandleProps     *   vhp =   &( _vp->vhp );
    GftvHandleProps     *   hhp =   &( _vp->hhp );
    //  ........................................................................
    //  vertical scrollbar
    {

    //  ........................................................................
    //  scrollbar allocation ( in pixels )
    vhp->gfx_l_space_before =   1.0;
    vhp->gfx_l_space_after  =   1.0;

    vhp->gfx_l_alloc    =   MAX(
        _vp->alloc_height
            - vhp->gfx_l_space_before                                           //  top space
            - vhp->gfx_l_space_after                                            //  bottom space
            - 1.0                                                               //  HScrollBar separator
            - GTK_FAST_TEXT_VIEW_HBAR_AREA_THICKNESS                            //  HScrollBar space
            - 1.0   ,                                                           //  gtk resizing keep last line for him
        0.0         );
    //  ........................................................................
    //  scrollbar size ( in pixels )
    r   =   _bp->lines_card                                         ?           //  ratio [0.0;1.0] of lines the widget can display
            (gdouble)_vp->width_char_y / (gdouble)_bp->lines_card   :
            1.0                                                     ;

    vhp->gfx_l_size =   r * (gdouble)vhp->gfx_l_alloc;                          //  apply ratio to get scrollbar size
    vhp->gfx_l_size =   floor( vhp->gfx_l_size );                               //  ( work with rounded pixels coords )

    GWR_BOUND_LE( vhp->gfx_l_size, (gdouble)vhp->gfx_l_alloc            );      //  scrollbar size limitation 1 ( unuseful  ! )
    GWR_BOUND_GE( vhp->gfx_l_size, GTK_FAST_TEXT_VIEW_HANDLE_LENGTH_MIN );      //  scrollbar size limitation 2
    //  ........................................................................
    //  determine scrollbar start position

    //  0 <= r <= 1
    r   =   ( _bp->lines_card > 1 )                                     ?
            (gdouble)vhp->offset / ( (gdouble)_bp->lines_card /*- 1.0*/ )   :
            0.0                                                         ;

    vhp->gfx_l_start_min    =   vhp->gfx_l_space_before;
    vhp->gfx_l_start_max    =   vhp->gfx_l_alloc - vhp->gfx_l_size + vhp->gfx_l_space_before;

    vhp->gfx_l_start        =   ( vhp->gfx_l_alloc - vhp->gfx_l_size ) * r;
    vhp->gfx_l_start        =   floor( vhp->gfx_l_start );                  //  work with rounded pixels coords
    vhp->gfx_l_start        =   vhp->gfx_l_start + 1.0;
    }
    //printf("compute scrollbars:V:pos=%f size=%f step=%f\n", _vp->vhp.gfx_l_start, _vp->vhp.gfx_l_size, _vp->vhp.inc_step);
    //  ........................................................................
    //  horizontal scrollbar
    {

    //  ........................................................................
    //  scrollbar allocation ( in pixels )
    hhp->gfx_l_space_before =   1.0;
    hhp->gfx_l_space_after  =   1.0;

    hhp->gfx_l_alloc    =   MAX(
        _vp->alloc_width
            - hhp->gfx_l_space_before                                           //  left space
            - hhp->gfx_l_space_after                                            //  right space
            - 1.0                                                               //  VScrollBar separator
            - GTK_FAST_TEXT_VIEW_VBAR_AREA_THICKNESS                            //  VScrollBar space
            - 1.0   ,                                                           //  gtk resizing keep last line for him
        0.0         );

    //  ........................................................................
    //  scrollbar size ( in pixels )
    if ( _bp->lines_card )
    {
        r   =   (gdouble)_vp->width_char_x / (gdouble)_bp->lines_max_len;

        hhp->gfx_l_size =   r * (gdouble)hhp->gfx_l_alloc;
        hhp->gfx_l_size =   floor( hhp->gfx_l_size );                           //  work with rounded pixels coords

        GWR_BOUND_LE( hhp->gfx_l_size, (gdouble)hhp->gfx_l_alloc            );
        GWR_BOUND_GE( hhp->gfx_l_size, GTK_FAST_TEXT_VIEW_HANDLE_LENGTH_MIN );

        r   =   ( _bp->lines_max_len > 1 )                              ?
                (gdouble)hhp->offset / ( (gdouble)_bp->lines_max_len )  :
                0.0                                                     ;

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
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_get_line_index_at_pointer()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_get_line_index_at_pointer()
//!
//! \brief  Get GwrFastTextBuffer's line index from pointer y coord.
//!
//! \return TRUE if a line exists at that index in the GwrFastTextBuffer ;
//!     FALSE else.
inline static
guint32         gtk_fast_text_view_get_line_index_at_pointer(
    GtkFastTextViewPrivate      *   _vp     ,
    gdouble                         _y      )
{
    guint32 index   =   ( (guint32)_y ) / _vp->font_height;
            index   =   index +   _vp->vhp.offset;

    //D printf("Line index at pointer:[%3i]\n", index);

    return index;
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_show_cursor_normal()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_show_cursor_normal()
//!
//! \brief  Guess.
static  void        gtk_fast_text_view_show_cursor_normal(GtkWidget* _w, GtkFastTextViewPrivate* _vp)
{
    GtkFastTextViewClass    *   klass;
    //  ........................................................................
    if ( _vp->a_cursor_current == 0 )
        return;

    klass   =   G_TYPE_INSTANCE_GET_CLASS(
                    _w                              ,
                    gtk_fast_text_view_get_type()   ,
                    GtkFastTextViewClass            );

    gdk_window_set_cursor( _vp->a_gdk_window, klass->cursors.normal );

    _vp->a_cursor_current = 0;
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_show_cursor_url()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_show_cursor_url()
//!
//! \brief  Guess.
static  void        gtk_fast_text_view_show_cursor_url(GtkWidget* _w, GtkFastTextViewPrivate* _vp)
{
    GtkFastTextViewClass    *   klass;
    //  ........................................................................
    if ( _vp->a_cursor_current == 1 )
        return;

    klass   =   G_TYPE_INSTANCE_GET_CLASS(
                    _w                              ,
                    gtk_fast_text_view_get_type()   ,
                    GtkFastTextViewClass            );

    gdk_window_set_cursor( _vp->a_gdk_window, klass->cursors.url );

    _vp->a_cursor_current = 1;
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_show_cursor_data()
//  ----------------------------------------------------------------------------
//! \fn     gtk_fast_text_view_show_cursor_data()
//!
//! \brief  Guess.
static  void        gtk_fast_text_view_show_cursor_data(GtkWidget* _w, GtkFastTextViewPrivate* _vp)
{
    GtkFastTextViewClass    *   klass;
    //  ........................................................................
    if ( _vp->a_cursor_current == 2 )
        return;

    klass   =   G_TYPE_INSTANCE_GET_CLASS(
                    _w                              ,
                    gtk_fast_text_view_get_type()   ,
                    GtkFastTextViewClass            );

    gdk_window_set_cursor( _vp->a_gdk_window, klass->cursors.data );

    _vp->a_cursor_current = 2;
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

    gtk_fast_text_view_compute_3_scrollbars(vp, (GwrFastTextBufferPrivate*)vp->buffer->priv);
    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
void            gtk_fast_text_view_scroll_vertical_down (GtkFastTextView* _v, guint32 _ymove)
{
    GtkFastTextViewPrivate      *   vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(_v);
    GwrFastTextBuffer           *   b   =   vp->buffer;
    GwrFastTextBufferPrivate    *   bp  =   (GwrFastTextBufferPrivate*)(b->priv);
    //  ........................................................................
    if ( ( vp->vhp.offset  + _ymove ) > ( bp->lines_card - vp->width_char_y ) )
        vp->vhp.offset  =   bp->lines_card - vp->width_char_y;
    else
        vp->vhp.offset  =   vp->vhp.offset + _ymove;

    gtk_fast_text_view_compute_3_scrollbars(vp, (GwrFastTextBufferPrivate*)vp->buffer->priv);
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

    gtk_fast_text_view_compute_0_font_dimensions(vp, c);
    gtk_fast_text_view_compute_1_chars_capacity(vp);

    cairo_destroy(c);
    cairo_surface_destroy(s);

    gtk_fast_text_view_recompute( v );
}
//  ----------------------------------------------------------------------------
//  gtk_fast_text_view_SIGNAL_key_release_event()
//  ----------------------------------------------------------------------------
static gboolean     gtk_fast_text_view_SIGNAL_key_press_event(
    GtkWidget       *   _w      ,
    GdkEventKey     *   _evt    ,
    gpointer            _data   )
{
    return TRUE;
}
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
    //D printf("key release\n");

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
    GdkEventButton              *   evb =   NULL;
    GtkFastTextView             *   v   =   NULL;
    GtkFastTextViewPrivate      *   vp  =   NULL;
    GwrFastTextBufferPrivate    *   bp  =   NULL;
    GwrFastTextBufferLine           line;
    guint32                         lix =   0;
    //  ........................................................................
    //printf("VIEW:button-press\n");
    //  ........................................................................
    evb =   (GdkEventButton*)_evt;
    v   =   (GtkFastTextView*)_data;
    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(v);
    bp  =   (GwrFastTextBufferPrivate*)(vp->buffer->priv);
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

        return  FALSE;
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

        return  FALSE;
    }
    //  ........................................................................
    //  click somewhere else
    if ( vp->a_cursor_current == 1 )
    {
        printf("callback\n");

        lix = gtk_fast_text_view_get_line_index_at_pointer( vp, evb->y );

        if  (   gwr_fast_text_buffer_line_exist_and_has_data(
                    vp->buffer  ,
                    lix         )
            )
        {
            gwr_fast_text_buffer_get_line_and_data(
                vp->buffer  ,
                lix         ,
                &line       );
            //D printf("Extra data click:[%p][%3i][", line.a_extra_data, line.a_extra_data_len);
            //D for ( guint32 i = 0 ; i != line.a_extra_data_len ; i++ )
            //D {
            //D     printf("%c", *((gchar*)(line.a_extra_data + i)) );
            //D }
            //D printf("]\n");
            if ( vp->a_xd_callback )
            {
                vp->a_xd_callback( line.a_extra_data, line.a_extra_data_len, line.a_ho, line.a_hl );
            }
            else
            {
                g_warning( "Extra Data click but:no callback defined");
            }
        }
        else
        {
            g_warning( "Extra Data click but:no line / no line with extra data");
        }
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
    GdkEventMotion              *   evm =   NULL;
    GtkFastTextView             *   v   =   NULL;
    GtkFastTextViewPrivate      *   vp  =   NULL;
    GwrFastTextBufferPrivate    *   bp  =   NULL;

    guint32                         evx;
    guint32                         evy;
    GftvGfxLine                 *   gfx_line;
    guint32                         gix;
    //  ........................................................................
    //printf("VIEW:motion-notify\n");
    //  ........................................................................
    evm =   (GdkEventMotion*)_evt;
    v   =   (GtkFastTextView*)_data;
    vp  =   GTK_FAST_TEXT_VIEW_PRIVATE(v);
    bp  =   (GwrFastTextBufferPrivate*)(vp->buffer->priv);
    //  ........................................................................
    //  we are dragging vertical scrollbar
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

        return FALSE;
    }
    //  ........................................................................
    //  we are dragging horizontal scrollbar
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

        return FALSE;
    }
    //  ........................................................................
    //  we are dragging nothing, test for xtra data
    evx =   (guint32)( evm->x );
    evy =   (guint32)( evm->y );

    if  (   ( evx <= vp->text_area_width    )  &&                               //  in text area ?
            ( evy <= vp->text_area_height   )   )
    {
        gix = evy / vp->font_height;                                            //  graphical line index

        //D printf("line gfx index:%i\n", gix);

        if ( gix < vp->d_gfx_lines->a_slots_used )
        {
            gfx_line = gwr_array_equal_simple_get_data( vp->d_gfx_lines, gix );

            if ( gfx_line->a_has_url )                                          //  test for url first
            {
                if  (
                        (   (guint32)(evm->x) >=   gfx_line->a_url_x                     )   &&
                        (   (guint32)(evm->x) <= ( gfx_line->a_url_x + gfx_line->a_url_w )    )
                    )
                {
                    //D printf("Graphical line index:[%3i] - URL -\n", gix);
                    gtk_fast_text_view_show_cursor_url(_w, vp);
                    goto    lab_mne_cursor_done;
                }
                else
                {
                    //D printf("Graphical line index:[%3i] - DATA (url) -\n", gix);
                    gtk_fast_text_view_show_cursor_data(_w, vp);
                    goto    lab_mne_cursor_done;
                }
            }
            if ( gfx_line->a_has_data )                                         //  now test for data
            {
                //D printf("Graphical line index:[%3i] - DATA -\n", gix);
                gtk_fast_text_view_show_cursor_data(_w, vp);
                goto    lab_mne_cursor_done;
            }
        }
    }
    gtk_fast_text_view_show_cursor_normal(_w, vp);
    //  ........................................................................
lab_mne_cursor_done:
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
void                gtk_fast_text_view_set_xd_callback      (GtkFastTextView* _w, GtkFastTextViewXdCallback _cb)
{
    GTK_FAST_TEXT_VIEW_PRIVATE(_w)->a_xd_callback = _cb;
}
//  ----------------------------------------------------------------------------
void                gtk_fast_text_view_refresh              (GtkFastTextView* _v)
{
    gtk_fast_text_view_recompute(_v);
    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
