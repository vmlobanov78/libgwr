/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-gwrgtk-textview.c                                               *
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

#include "libgwrc.h"
#include "C/widget/libgwrc-gwrgtk-textbuffer-private.h"
//  ****************************************************************************
//
//  Forward declarations - GTK
//
//  ****************************************************************************
//  Private class member
#define GWRGTK_TEXTVIEW_PRIVATE(obj)                                            \
        G_TYPE_INSTANCE_GET_PRIVATE                                             \
        (                                                                       \
            (obj)                   ,                                           \
            GWRGTK_TEXTVIEW_TYPE    ,                                           \
            GwrGtkTextViewPrivate                                               \
        )

typedef struct _GwrGtkTextViewPrivate GwrGtkTextViewPrivate;

struct _GwrGtkTextViewPrivate
{
    GwrGtkTextBuffer    *   buffer;

    GtkAdjustment       *   hadjust;
    GtkAdjustment       *   vadjust;

    int                     alloc_width;                                        //!< Widht  available for drawing
    int                     alloc_height;                                       //!< Height available for drawing

    gboolean                font_size_changed;
    guint32                 font_size;
    guint32                 font_width;
    guint32                 font_height;

    guint16                 width_char_x;                                       //!< # of chars the widget can display
    guint16                 width_char_y;                                       //!< # of lines the widget can display

	gdouble                 scroll_pos_x;                                       //!< Position ( 0.0 -> 1.0 ) of horizontal scrollbar
	gdouble                 scroll_pos_y;                                       //!< Position ( 0.0 -> 1.0 ) of vertical   scrollbar
};

static  void        gwrgtk_textview_get_preferred_width (GtkWidget *widget, int *minimal, int *natural);
static  void        gwrgtk_textview_get_preferred_height(GtkWidget *widget, int *minimal, int *natural);

static  void        gwrgtk_textview_realize             (GtkWidget*);
static  void        gwrgtk_textview_size_allocate       (GtkWidget*, GtkAllocation*);
static  gboolean    gwrgtk_textview_draw                (GtkWidget*, cairo_t*);
//static void gwrgtk_textview_size_allocate       (GtkWidget *widget,	GtkAllocation *allocation);
//static GType gwrgtk_textview_child_type(GtkContainer *container);
//static void gwrgtk_textview_add(GtkContainer *container, GtkWidget *widget);
//static void gwrgtk_textview_remove(GtkContainer *container, GtkWidget *widget);
//static void gwrgtk_textview_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data);
//  ****************************************************************************
//
//  Forward declarations - NON-GTK
//
//  ****************************************************************************
static  void        gwrgtk_textview_recompute_adjustments       (GwrGtkTextView*);
static  void        gwrgtk_textview_compute_font_dimensions     (GwrGtkTextViewPrivate*, cairo_t*);
static  void        gwrgtk_textview_compute_chars_capacity      (GwrGtkTextViewPrivate*);
static  void        gwrgtk_textview_SIGNAL_map                  (GtkWidget*, gpointer                 );
static  gboolean    gwrgtk_textview_SIGNAL_key_press_event      (GtkWidget*, GdkEventKey*   , gpointer);
static  gboolean    gwrgtk_textview_SIGNAL_key_release_event    (GtkWidget*, GdkEventKey*   , gpointer);
static  gboolean    gwrgtk_textview_SIGNAL_button_press_event   (GtkWidget*, GdkEvent*      , gpointer);
//  ****************************************************************************
//
//  GTK stuff
//
//  ****************************************************************************
//  Define the GwrGtkTextView type and inherit from GtkContainer
G_DEFINE_TYPE(GwrGtkTextView, gwrgtk_textview, GTK_TYPE_WIDGET);
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_class_init()
//  ----------------------------------------------------------------------------
static void
gwrgtk_textview_class_init(GwrGtkTextViewClass* _klass)
{
    // Override GtkWidget methods
    GtkWidgetClass* widget_class        = GTK_WIDGET_CLASS(_klass);
    widget_class->get_preferred_width   = gwrgtk_textview_get_preferred_width;  //!< Minimal & natural sizes
    widget_class->get_preferred_height  = gwrgtk_textview_get_preferred_height; //!< Minimal & natural sizes
    widget_class->size_allocate         = gwrgtk_textview_size_allocate;        //!< For computing gwrgtk_textview_draw()'s parameters
    widget_class->draw                  = gwrgtk_textview_draw;                 //!< For drawing the model's content
    widget_class->realize               = gwrgtk_textview_realize;              //!< For creating a GdkWindow

    //  Override GtkContainer methods : empty

    //  Add private indirection member
	g_type_class_add_private(_klass, sizeof(GwrGtkTextViewPrivate));
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_init()
//  ----------------------------------------------------------------------------
static void
gwrgtk_textview_init(GwrGtkTextView *_textview)
{
    // This means that GwrGtkTextView doesn't supply its own GdkWindow
	gtk_widget_set_has_window(GTK_WIDGET(_textview), TRUE);

	/* Set redraw on allocate to FALSE if the top left corner of your widget
	 * doesn't change when it's resized; this saves time */
	/*gtk_widget_set_redraw_on_allocate(GTK_WIDGET(square), FALSE);*/

	/* Initialize private members */
	GwrGtkTextViewPrivate *priv = GWRGTK_TEXTVIEW_PRIVATE(_textview);
	priv->scroll_pos_x  =   0.0;
	priv->scroll_pos_y  =   0.0;

    priv->font_size_changed =   TRUE;
    priv->font_size         =   8;
    priv->font_width        =   0.0;
    priv->font_height       =   0.0;

    priv->hadjust           =   gtk_adjustment_new( 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 );
    priv->vadjust           =   gtk_adjustment_new( 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 );

    gtk_widget_set_can_focus( GTK_WIDGET(_textview), TRUE );

    gtk_widget_add_events( GTK_WIDGET(_textview), GDK_KEY_RELEASE_MASK );

    g_signal_connect( _textview, "map"                  , G_CALLBACK(gwrgtk_textview_SIGNAL_map)                , (gpointer)_textview);
    g_signal_connect( _textview, "key-release-event"    , G_CALLBACK(gwrgtk_textview_SIGNAL_key_release_event)  , (gpointer)_textview);
    g_signal_connect( _textview, "key-press-event"      , G_CALLBACK(gwrgtk_textview_SIGNAL_key_press_event)    , (gpointer)_textview);
    //g_signal_connect( _textview, "button-press-event"   , G_CALLBACK(gwrgtk_textview_SIGNAL_button_press_event) , (gpointer)_textview);
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_new()
//  ----------------------------------------------------------------------------
GtkWidget *
gwrgtk_textview_new()
{
    return GTK_WIDGET( g_object_new(gwrgtk_textview_get_type(), NULL) );
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_get_preferred_width()
//  ----------------------------------------------------------------------------
static void
gwrgtk_textview_get_preferred_width(GtkWidget *widget, int *minimal, int *natural)
{
	g_return_if_fail(widget != NULL);
	g_return_if_fail(GWRGTK_IS_TEXTVIEW(widget));

	//get_size(GWRGTK_TEXTVIEW(widget), GTK_ORIENTATION_HORIZONTAL, minimal, natural);
    *minimal    = 100;
    *natural    = 300;
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_get_preferred_height()
//  ----------------------------------------------------------------------------
static void
gwrgtk_textview_get_preferred_height(GtkWidget *widget, int *minimal, int *natural)
{
	g_return_if_fail(widget != NULL);
	g_return_if_fail(GWRGTK_IS_TEXTVIEW(widget));

	//get_size(GWRGTK_TEXTVIEW(widget), GTK_ORIENTATION_VERTICAL, minimal, natural);
    *minimal    = 100;
    *natural    = 300;
}
//  ============================================================================
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_realize()
//  ----------------------------------------------------------------------------
//! \fn     gwrgtk_textview_realize()
//!
//! \brief  Here we create a GdkWindow.
void            gwrgtk_textview_realize(GtkWidget* _w)
{
    GtkAllocation       allocation;
    GdkWindow       *   window;
    GdkWindowAttr       attributes;
    gint                attributes_mask;
    //  ........................................................................
    if ( ! gtk_widget_get_has_window (_w) )
    {
        GTK_WIDGET_CLASS (gwrgtk_textview_parent_class)->realize (_w);
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
        attributes.event_mask   = gtk_widget_get_events (_w) |
                                    GDK_EXPOSURE_MASK | GDK_KEY_RELEASE_MASK;

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
//  gwrgtk_textview_size_allocate()
//  ----------------------------------------------------------------------------
//! \fn     gwrgtk_textview_size_allocate()
//!
//! \brief  GTK calls us when our allocation changes.
void            gwrgtk_textview_size_allocate(GtkWidget* _w, GtkAllocation* _allocation)
{
    GwrGtkTextView          *   tv  =   NULL;
    GwrGtkTextViewPrivate   *   tvp =   NULL;
    //  ........................................................................
    g_return_if_fail( GWRGTK_IS_TEXTVIEW(_w) );
    //  ........................................................................
	gtk_widget_set_allocation(_w, _allocation);                                 //  mandatory
    //  ........................................................................
    tv  =   GWRGTK_TEXTVIEW         ( _w );
    tvp =   GWRGTK_TEXTVIEW_PRIVATE ( _w );

    tvp->alloc_width    =   _allocation->width;
    tvp->alloc_height   =   _allocation->height;

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
            //  gwrgtk_textview_recompute_adjustments() below. It's faster.
            //
            //  gtk_drawing_area_send_configure (GTK_DRAWING_AREA (widget));
        }
    }

    gwrgtk_textview_recompute_adjustments(tv);

    //printf("gwrgtk_textview_size_allocate():%i %i %i %i\n", _allocation->x, _allocation->y, _allocation->width, _allocation->height);
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_draw()
//  ----------------------------------------------------------------------------
//! \fn     gwrgtk_textview_draw()
//!
//! \brief  GTK calls us for drawing our stuff.
gboolean        gwrgtk_textview_draw(GtkWidget *widget, cairo_t *cr)
{
    static  gchar                       text    [4096];

    static  GwrGtkTextViewPrivate   *   vp      =   NULL;
    static  GwrGtkTextBuffer        *   b       =   NULL;
    static  GwrGtkTextBufferPrivate *   bp      =   NULL;

    static  cairo_surface_t         *   surface;

    GwrGtkTextBufferLine                line;
    guint16                             ll;

    static  double                      x1,y1,x2,y2;
    static  guint32                     x0, w3;
    static  guint32                     y0;
    static  guint32                     i       =   0;
    //  ........................................................................
    vp  =   GWRGTK_TEXTVIEW_PRIVATE(widget);
    //printf("gwrgtk_textview_draw():sx=%f sy=%f\n", vp->scroll_pos_x, vp->scroll_pos_y);

    b   =   vp->buffer;
    if ( ! b )
        return TRUE;

    surface =   cairo_get_target( cr );
    //  ........................................................................
    //  font sizes
    cairo_select_font_face  (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size     (cr, vp->font_size);

    //  if font size changed, recompute extents and associated values
    if ( vp->font_size_changed )
    {
        //printf("gwrgtk_textview_draw():font size changed\n");

        vp->font_size_changed   =   FALSE;

        gwrgtk_textview_compute_font_dimensions(vp, cr);
        gwrgtk_textview_compute_chars_capacity(vp);
    }
    //printf("gwrgtk_textview_draw():fx=%i wx=%i\n", vp->font_width, vp->width_char_x);
    //  ........................................................................
    //  text window setup
    cairo_clip_extents(cr,&x1,&y1,&x2,&y2);                                     //  get window dimensions ( pixels )
    bp  =   (GwrGtkTextBufferPrivate*)b->priv;

    x0  =   (guint32)vp->scroll_pos_x;                                          //  compute x0 = first char ( in each line ) to be displayed
    y0  =   (guint32)vp->scroll_pos_y;                                          //  compute y0 = first line to be displayed

    for ( i = 0 ; i != vp->width_char_y ; i++ )
    {
        if ( ! gwrgtk_text_buffer_get_line( b, y0 + i, &line ) )                //  no more lines
            break;

        ll = line.a_len;

        if ( x0 > (guint32)( ll - 1 ) )                                         //  not enough chars in string to reach the cairo clip
            continue;

        w3  =   MIN( (guint32)( ll - x0 ), vp->width_char_x );                  //  # of chars to be displayed

        strncpy( text, line.a_str + x0, w3 );
        text[w3] = 0;

        if ( line.a_fg )
            cairo_set_source_rgb(
                cr                                              ,
                (gdouble)libgwrC_Colors[ line.a_fg ].r / 256.0  ,
                (gdouble)libgwrC_Colors[ line.a_fg ].g / 256.0  ,
                (gdouble)libgwrC_Colors[ line.a_fg ].b / 256.0  );
        else
            cairo_set_source_rgb    (cr, 0.0, 0.0, 0.0);

        cairo_move_to           (cr, 0, 8 + i * vp->font_height);
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
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_compute_font_dimensions()
//  ----------------------------------------------------------------------------
//! \fn     gwrgtk_textview_compute_font_dimensions()
//!
//! \brief  Compute font dimensions
static  void    gwrgtk_textview_compute_font_dimensions(
    GwrGtkTextViewPrivate   *   _vp     ,
    cairo_t                 *   _cr     )
{
    static  cairo_font_extents_t        extents;
    //  ........................................................................
    cairo_font_extents      ( _cr, &extents );

    _vp->font_height        =   (guint32)( extents.height          );
    _vp->font_width         =   (guint32)( extents.max_x_advance   );

    //printf("gwrgtk_textview_compute_font_dimensions():fw=%i\n", _vp->font_width);
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_compute_chars_capacity()
//  ----------------------------------------------------------------------------
//! \fn     gwrgtk_textview_compute_chars_capacity()
//!
//! \brief  Compute chars capacities from screen allocation.
static  void    gwrgtk_textview_compute_chars_capacity(
    GwrGtkTextViewPrivate   *   _vp     )
{
    _vp->width_char_x       =   _vp->alloc_width     / _vp->font_width;
    _vp->width_char_y       =   _vp->alloc_height    / _vp->font_height;
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_recompute_adjustments()
//  ----------------------------------------------------------------------------
//! \fn     gwrgtk_textview_recompute_adjustments()
//!
//! \brief  Compute adjustments from chars capacities.
static  void    gwrgtk_textview_recompute_adjustments(
    GwrGtkTextView          *   _v      )
{
    GwrGtkTextViewPrivate   *   vp  =   NULL;
    GwrGtkTextBufferPrivate *   bp  =   NULL;

    gdouble                     psx =   0.0;
    gdouble                     psy =   0.0;
    gdouble                     upx =   0.0;
    gdouble                     upy =   0.0;
    //  ........................................................................
    g_return_if_fail( GWRGTK_IS_TEXTVIEW(_v) );

    vp  =   GWRGTK_TEXTVIEW_PRIVATE     ( _v                    );
    bp  =   (GwrGtkTextBufferPrivate*)  ( vp->buffer->priv      );

    //  we may be called before realization ( ex by gwrgtk_textview_refresh()
    //  when the widget has not been yet displayed but some text was added
    //  to the buffer )
    if ( ! gtk_widget_get_realized( GTK_WIDGET( _v ) ) )
        return;

    if ( ! bp )
        return;

    gwrgtk_textview_compute_chars_capacity(vp);

    psx =   (gdouble)vp->width_char_x;
    psy =   (gdouble)vp->width_char_y;

    gtk_adjustment_set_page_size( vp->hadjust, psx );
    gtk_adjustment_set_page_size( vp->vadjust, psy );

    upx =   psx < (gdouble)bp->lines_max_len        ?
                (gdouble)bp->lines_max_len - psx    :
                0.1;

    upy =   psy < (gdouble)bp->lines_card           ?
                (gdouble)bp->lines_card - psy       :
                0.1;

    gtk_adjustment_set_upper    ( vp->hadjust, upx );
    gtk_adjustment_set_upper    ( vp->vadjust, upy );

    //gtk_adjustment_changed(vp->hadjust);                                      //  unuseful
    //gtk_adjustment_changed(vp->vadjust);

    //printf("gwrgtk_textview_recompute_adjustments():\n");
    //printf("  vp=%p\n", vp);
    //printf("  fx=%i wx=%i\n", vp->font_width, vp->width_char_x);
    //printf("  line_max_len=%i lines_card=%i\n", bp->lines_max_len, bp->lines_card);
    //printf("  psx=%f psy=%f upx=%f upy=%f\n", psx, psy, upx, upy);
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_SIGNAL_map()
//  ----------------------------------------------------------------------------
static  void        gwrgtk_textview_SIGNAL_map(
    GtkWidget       *   _w      ,
    gpointer            _data   )
{
    GwrGtkTextView          *   v   =   NULL;
    GwrGtkTextViewPrivate   *   vp  =   NULL;

    GdkWindow               *   w   =   NULL;
    cairo_surface_t         *   s   =   NULL;
    cairo_t                 *   c   =   NULL;
    //  ........................................................................
    //  "map" signal is connected because gwrgtk_textview_draw() can be called
    //  before gwrgtk_textview_recompute_adjustments() or gwrgtk_textview_compute_chars_capacity().
    //  So at first display of widget, we do a whole computation

    //printf("gwrgtk_textview_SIGNAL_map()\n");

    g_return_if_fail( GWRGTK_IS_TEXTVIEW(_w) );

    v   =   GWRGTK_TEXTVIEW( _w );
    vp  =   GWRGTK_TEXTVIEW_PRIVATE( v );

    w   =   gtk_widget_get_window (_w);
    s   =   gdk_window_create_similar_surface(w, CAIRO_CONTENT_COLOR, 10, 10);
    c   =   cairo_create(s);

    //  select font else cairo with use default font, sizes will be different
    //  from our settings
    cairo_select_font_face  (c, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size     (c, vp->font_size);

    gwrgtk_textview_compute_font_dimensions(vp, c);
    gwrgtk_textview_compute_chars_capacity(vp);

    cairo_destroy(c);
    cairo_surface_destroy(s);

    gwrgtk_textview_recompute_adjustments( v );
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_SIGNAL_key_press_event()
//  ----------------------------------------------------------------------------
static gboolean     gwrgtk_textview_SIGNAL_key_press_event(
    GtkWidget       *   _w      ,
    GdkEventKey     *   _evt    ,
    gpointer            _data   )
{
    //  ........................................................................
    //  grab the focus so parent widget cant intercept our keys before we
    //  get them
    gtk_widget_grab_focus( _w );
    return TRUE;
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_SIGNAL_key_release_event()
//  ----------------------------------------------------------------------------
static gboolean     gwrgtk_textview_SIGNAL_key_release_event(
    GtkWidget       *   _w      ,
    GdkEventKey     *   _evt    ,
    gpointer            _data   )
{
    GwrGtkTextView          *   v   =   NULL;
    GwrGtkTextViewPrivate   *   vp  =   NULL;
    GwrGtkTextBuffer        *   b   =   NULL;
    gboolean                    r   =   TRUE;
    //  ........................................................................
    v   =   GWRGTK_TEXTVIEW( _data );
    vp  =   GWRGTK_TEXTVIEW_PRIVATE( v );
    b   =   vp->buffer;

    if ( !b )
        return TRUE;

    if ( _evt->keyval == GDK_KEY_Up )
    {
        //printf("gwrgtk_textview_SIGNAL_key_release_event():UP\n");
        gwrgtk_textview_one_line_up(v);
        return r;
    }
    if ( _evt->keyval == GDK_KEY_Down )
    {
        //printf("gwrgtk_textview_SIGNAL_key_release_event():DOWN\n");
        gwrgtk_textview_one_line_down(v);
        return r;
    }
    if ( _evt->keyval == GDK_KEY_Page_Up )
    {
        //printf("gwrgtk_textview_SIGNAL_key_release_event():PAGEUP\n");
        gwrgtk_textview_one_page_up(v);
        return r;
    }
    if ( _evt->keyval == GDK_KEY_Page_Down )
    {
        //printf("gwrgtk_textview_SIGNAL_key_release_event():PAGEDOWN\n");
        gwrgtk_textview_one_page_down(v);
        return r;
    }

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  gwrgtk_textview_SIGNAL_button_press_event()
//  ----------------------------------------------------------------------------
static  gboolean    gwrgtk_textview_SIGNAL_button_press_event(
    GtkWidget       *   _w      ,
    GdkEvent        *   _evt    ,
    gpointer        _data       )
{
    printf("VIEW:EVENT-BUTTON\n");

    //  false to propagate
    return FALSE;
}
//  ============================================================================
void                gwrgtk_textview_set_buffer          (GwrGtkTextView* _w, GwrGtkTextBuffer* _b)
{
    GWRGTK_TEXTVIEW_PRIVATE(_w)->buffer     =   _b;
}
void                gwrgtk_textview_set_font_size       (GwrGtkTextView* _w, guint32 _size)
{
    GWRGTK_TEXTVIEW_PRIVATE(_w)->font_size  =   _size;
}
GtkAdjustment   *   gwrgtk_textview_get_vadjustment     (GwrGtkTextView* _v)
{
    return  GWRGTK_TEXTVIEW_PRIVATE(_v)->vadjust;
}
GtkAdjustment   *   gwrgtk_textview_get_hadjustment     (GwrGtkTextView* _v)
{
    return  GWRGTK_TEXTVIEW_PRIVATE(_v)->hadjust;
}
void                gwrgtk_textview_set_scroll_pos_x    (GwrGtkTextView* _v, gdouble _xpos)
{
    GwrGtkTextViewPrivate   *   vp  =   GWRGTK_TEXTVIEW_PRIVATE(_v);
    gdouble                     up  =   gtk_adjustment_get_upper( vp->hadjust );
    //  ........................................................................
    if ( _xpos < 0.1 )
    {
        vp->scroll_pos_x    =   0.1;
        goto lab_end;
    }

    if ( _xpos > up )
    {
        vp->scroll_pos_x    =   up;
        goto lab_end;
    }

    vp->scroll_pos_x        =   _xpos;

lab_end:
    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
void                gwrgtk_textview_set_scroll_pos_y    (GwrGtkTextView* _v, gdouble _ypos)
{
    GwrGtkTextViewPrivate   *   vp  =   GWRGTK_TEXTVIEW_PRIVATE(_v);
    gdouble                     up  =   gtk_adjustment_get_upper( vp->vadjust );
    //  ........................................................................
    if ( _ypos < 0.1 )
    {
        vp->scroll_pos_y    =   0.1;
        goto lab_end;
    }

    if ( _ypos > up )
    {
        vp->scroll_pos_y    =   up;
        goto lab_end;
    }

    vp->scroll_pos_y        =   _ypos;

lab_end:
    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
//  ----------------------------------------------------------------------------
void                gwrgtk_textview_one_line_up         (GwrGtkTextView* _v)
{
    GwrGtkTextViewPrivate   *   vp  =   GWRGTK_TEXTVIEW_PRIVATE(_v);
    gdouble                     v   =   vp->scroll_pos_y;
    //  ........................................................................
    v   =   v   -   1.0;

    gwrgtk_textview_set_scroll_pos_y( _v, v );

    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
void                gwrgtk_textview_one_line_down       (GwrGtkTextView* _v)
{
    GwrGtkTextViewPrivate   *   vp  =   GWRGTK_TEXTVIEW_PRIVATE(_v);
    gdouble                     v   =   vp->scroll_pos_y;
    //  ........................................................................
    v   =   v   +   1.0;

    gwrgtk_textview_set_scroll_pos_y( _v, v );

    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
void                gwrgtk_textview_one_page_up         (GwrGtkTextView* _v)
{
    GwrGtkTextViewPrivate   *   vp  =   GWRGTK_TEXTVIEW_PRIVATE(_v);
    gdouble                     v   =   vp->scroll_pos_y;
    //  ........................................................................
    v   =   v   -   ( 1.0 * (gdouble)vp->width_char_y );

    gwrgtk_textview_set_scroll_pos_y( _v, v );

    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
void                gwrgtk_textview_one_page_down       (GwrGtkTextView* _v)
{
    GwrGtkTextViewPrivate   *   vp  =   GWRGTK_TEXTVIEW_PRIVATE(_v);
    gdouble                     v   =   vp->scroll_pos_y;
    //  ........................................................................
    v   =   v   +   ( 1.0 * (gdouble)vp->width_char_y );

    gwrgtk_textview_set_scroll_pos_y( _v, v );

    gtk_widget_queue_draw(GTK_WIDGET(_v));
}
//  ----------------------------------------------------------------------------
void                gwrgtk_textview_refresh             (GwrGtkTextView* _v)
{
    gwrgtk_textview_recompute_adjustments(_v);
    gtk_widget_queue_draw(GTK_WIDGET(_v));
}



