#include <gtk/gtk.h>
#include "gwrgtk-textview.h"
#include "gwrgtk-textbuffer-private.h"

#include    <string.h>
#include    <X11/Xlib.h>

typedef struct  _libgwrC_Color  libgwrC_Color;

struct  _libgwrC_Color
{
    guint8  r;
    guint8  g;
    guint8  b;
}   __attribute__(( aligned(8) ));

extern  libgwrC_Color   libgwrC_Colors[ 256 ];
//  ============================================================================
/* Private class member */
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

    int                     alloc_width;                                        //!< Widht  available for drawing
    int                     alloc_height;                                       //!< Height available for drawing

	gdouble                 scroll_pos_x;                                       //!< Position ( 0.0 -> 1.0 ) of horizontal scrollbar
	gdouble                 scroll_pos_y;                                       //!< Position ( 0.0 -> 1.0 ) of vertical   scrollbar

    gboolean                font_size_changed;
    guint32                 font_size;
    guint32                 font_width;
    guint32                 font_height;
};
//  ============================================================================
/* Forward declarations */
static  void        gwrgtk_textview_get_preferred_width (GtkWidget *widget, int *minimal, int *natural);
static  void        gwrgtk_textview_get_preferred_height(GtkWidget *widget, int *minimal, int *natural);

static  gboolean    gwrgtk_textview_draw                (GtkWidget *widget, cairo_t *cr);
static  void        gwrgtk_textview_size_allocate       (GtkWidget* _w, GtkAllocation* _allocation);
//static void gwrgtk_textview_size_allocate       (GtkWidget *widget,	GtkAllocation *allocation);

//  ============================================================================

//static GType gwrgtk_textview_child_type(GtkContainer *container);
//static void gwrgtk_textview_add(GtkContainer *container, GtkWidget *widget);
//static void gwrgtk_textview_remove(GtkContainer *container, GtkWidget *widget);
//static void gwrgtk_textview_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data);

/* Define the GwrGtkTextView type and inherit from GtkContainer */
G_DEFINE_TYPE(GwrGtkTextView, gwrgtk_textview, GTK_TYPE_WIDGET);

/* Initialize the GwrGtkTextView class */
static void
gwrgtk_textview_class_init(GwrGtkTextViewClass* _klass)
{
    // Override GtkWidget methods
    GtkWidgetClass* widget_class        = GTK_WIDGET_CLASS(_klass);
    widget_class->get_preferred_width   = gwrgtk_textview_get_preferred_width;
    widget_class->get_preferred_height  = gwrgtk_textview_get_preferred_height;
    widget_class->size_allocate         = gwrgtk_textview_size_allocate;
    widget_class->draw                  = gwrgtk_textview_draw;
    //  Override GtkContainer methods */
    //  empty

    //  Add private indirection member
	g_type_class_add_private(_klass, sizeof(GwrGtkTextViewPrivate));
}

/* Initialize a new GwrGtkTextView instance */
static void
gwrgtk_textview_init(GwrGtkTextView *_textview)
{
    // This means that GwrGtkTextView doesn't supply its own GdkWindow
	gtk_widget_set_has_window(GTK_WIDGET(_textview), FALSE);

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
}

/* Return a new GwrGtkTextView cast to a GtkWidget */
GtkWidget *
gwrgtk_textview_new()
{
	return GTK_WIDGET(g_object_new(gwrgtk_textview_get_type(), NULL));
}

/* Get the width of the container */
static void
gwrgtk_textview_get_preferred_width(GtkWidget *widget, int *minimal, int *natural)
{
	g_return_if_fail(widget != NULL);
	g_return_if_fail(GWRGTK_IS_TEXTVIEW(widget));

	//get_size(GWRGTK_TEXTVIEW(widget), GTK_ORIENTATION_HORIZONTAL, minimal, natural);
    *minimal    = 100;
    *natural    = 300;
}

/* Get the height of the container */
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
void    gwrgtk_textview_size_allocate(GtkWidget* _w, GtkAllocation* _allocation)
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

    //printf("size_allocate():%i %i %i %i\n", _allocation->x, _allocation->y, _allocation->width, _allocation->height);
}
gboolean
gwrgtk_textview_draw(GtkWidget *widget, cairo_t *cr)
{
    static  gchar                       text    [4096];

    static  GwrGtkTextViewPrivate   *   vp      =   NULL;
    static  GwrGtkTextBuffer        *   b       =   NULL;
    static  GwrGtkTextBufferPrivate *   bp      =   NULL;

    static  cairo_surface_t         *   surface;
    static  cairo_font_extents_t        extents;

    GwrGtkTextBufferLine                line;
    guint16                             ll;

    static  double                      x1,y1,x2,y2;
    static  guint32                     w2, w1, x0, w3;
    static  guint32                     h2, h1, y0;
    static  guint32                     i       =   0;
    //  ........................................................................
    vp  =   GWRGTK_TEXTVIEW_PRIVATE(widget);
    printf("draw():%f %f\n", vp->scroll_pos_x, vp->scroll_pos_y);

    b   =   vp->buffer;
    if ( ! b )
        return TRUE;

    surface =   cairo_get_target( cr );
    //  ........................................................................
    //  font sizes
    cairo_select_font_face  (cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    if ( vp->font_size_changed )
    {
        vp->font_size_changed   =   FALSE;

        cairo_set_font_size     (cr, vp->font_size);
        cairo_font_extents      (cr, &extents);

        vp->font_height         =   (guint32)( extents.height          );
        vp->font_width          =   (guint32)( extents.max_x_advance   );
    }
    //  ........................................................................
    //  text window setup
    cairo_clip_extents(cr,&x1,&y1,&x2,&y2);                                     //  get window dimensions ( pixels )
    bp  =   (GwrGtkTextBufferPrivate*)b->priv;

    //  determine w2 = # of chars the window can display
    w2  =   ( x2 - x1 + 1 ) / vp->font_width;                                   //  nb of available pixels / font width
    //  determine w1 = interval where the first char displayed will be
    if ( w2 < bp->lines_max_len )
        w1  =   bp->lines_max_len - w2;
    else
        w1  =   0;                                                              //  widget width > max text len  of text buffer
    //  determine x0 = first char to be displayed
    x0  =   (guint32)( (gdouble)w1 * vp->scroll_pos_x );

    //  determine h2 = # of lines the window can display
    h2  =   ( y2 - y1 + 1 ) / vp->font_height;                                  //  nb of available pixels / font height
    //  determine h1 = interval where the first line displayed will be
    if ( h2 < bp->lines_card )
        h1  =   bp->lines_card - h2;
    else                                                                        //  widget height can display all text buffer lines
        h1  =   0;
    //  determine y0 = first line to be displayed
    y0  =   (guint32)( (gdouble)h1 * vp->scroll_pos_y );

    //printf("draw():x0=%i w1=%i w2=%i\n", x0, w1, w2);
    //printf("draw():y0=%i h1=%i h2=%i\n", y0, h1, h2);

    for ( i = 0 ; i != h2 ; i++ )
    {
        if ( ! gwrgtk_text_buffer_get_line( b, y0 + i, &line ) )                //  no more lines
            break;

        ll = line.a_len;

        if ( x0 > (guint32)( ll - 1 ) )                                         //  not enough chars in string to reach the cairo clip
            continue;

        w3  =   MIN( (guint32)( ll - x0 ), w2 );                                //  # of chars to be displayed

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
//  ============================================================================
void    gwrgtk_textview_set_buffer(GwrGtkTextView* _w, GwrGtkTextBuffer* _b)
{
    GWRGTK_TEXTVIEW_PRIVATE(_w)->buffer     =   _b;
}
void    gwrgtk_textview_set_font_size       (GwrGtkTextView* _w, guint32 _size)
{
    GWRGTK_TEXTVIEW_PRIVATE(_w)->font_size  =   _size;
}
void    gwrgtk_textview_set_scroll_pos_x    (GwrGtkTextView* _w, gdouble _xpos)
{
    //printf("%i\n", _xoff);
    GWRGTK_TEXTVIEW_PRIVATE(_w)->scroll_pos_x   =   _xpos;

    gtk_widget_queue_draw(GTK_WIDGET(_w));
}
void    gwrgtk_textview_set_scroll_pos_y    (GwrGtkTextView* _w, gdouble _ypos)
{
    //printf("%i\n", _xoff);
    GWRGTK_TEXTVIEW_PRIVATE(_w)->scroll_pos_y   =   _ypos;

    gtk_widget_queue_draw(GTK_WIDGET(_w));
}
void    gwrgtk_textview_set_available_width (GwrGtkTextView* _w, guint32 _width)
{
}
void    gwrgtk_textview_set_available_height(GwrGtkTextView* _w, guint32 _height)
{
}
