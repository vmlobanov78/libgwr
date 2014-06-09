#ifndef __GWRGTK_TEXT_VIEW_H__
#define __GWRGTK_TEXT_VIEW_H__

#include <glib-object.h>
#include <gtk/gtk.h>

#include "gwrgtk-textbuffer.h"

G_BEGIN_DECLS

#define GWRGTK_TEXTVIEW_TYPE            ( gwrgtk_textview_get_type  ()                                                          )
#define GWRGTK_TEXTVIEW(obj)            ( G_TYPE_CHECK_INSTANCE_CAST( (obj)     , GWRGTK_TEXTVIEW_TYPE, GwrGtkTextView      )   )
#define GWRGTK_TEXTVIEW_CLASS(klass)    ( G_TYPE_CHECK_CLASS_CAST   ( (klass)   , GWRGTK_TEXTVIEW_TYPE, GwrGtkTextViewClass )   )
#define GWRGTK_IS_TEXTVIEW(obj)         ( G_TYPE_CHECK_INSTANCE_TYPE( (obj)     , GWRGTK_TEXTVIEW_TYPE                      )   )
#define GWRGTK_IS_TEXTVIEW_CLASS(klass) ( G_TYPE_CHECK_CLASS_TYPE   ( (klass)   , GWRGTK_TEXTVIEW_TYPE                      )   )

typedef struct _GwrGtkTextView          GwrGtkTextView;
typedef struct _GwrGtkTextViewClass     GwrGtkTextViewClass;

struct _GwrGtkTextView
{
    GtkWidgetClass  parent_instance;
};

struct _GwrGtkTextViewClass
{
    GtkWidgetClass  parent_class;
};

GType           gwrgtk_textview_get_type    (void) G_GNUC_CONST;
GtkWidget   *   gwrgtk_textview_new         (void);

G_END_DECLS

#if ( __cplusplus )
extern "C" {
#endif

extern  void    gwrgtk_textview_set_buffer          (GwrGtkTextView*, GwrGtkTextBuffer*);

extern  void    gwrgtk_textview_set_font_size       (GwrGtkTextView*, guint32);

extern  void    gwrgtk_textview_set_scroll_pos_x    (GwrGtkTextView*, gdouble _xpos);
extern  void    gwrgtk_textview_set_scroll_pos_y    (GwrGtkTextView*, gdouble _ypos);
extern  void    gwrgtk_textview_set_available_width (GwrGtkTextView*, guint32 _width);
extern  void    gwrgtk_textview_set_available_height(GwrGtkTextView*, guint32 _height);

#if ( __cplusplus )
}
#endif






#endif                                                                          // __GWRGTK_TEXT_VIEW_H__
