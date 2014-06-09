/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-gwrgtk-textview.h                                               *
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
    *   Purpose : Simple and fast gtk comptable textview widget                 *
    *                                                                           *
    *****************************************************************************
*/
#ifndef __LIBGWRC_GWRGTK_TEXT_VIEW_H__
#define __LIBGWRC_GWRGTK_TEXT_VIEW_H__

//  ............................................................................
#include <glib-object.h>
#include <gtk/gtk.h>
//  ............................................................................
#include "C/libgwrc-common.h"
#include "C/widget/libgwrc-gwrgtk-textbuffer.h"
//  ............................................................................
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
GtkWidget   *   gwrgtk_textview_new         ();

G_END_DECLS

#if ( __cplusplus )
extern "C" {
#endif

typedef struct  _GwrGtkTextBuffer           GwrGtkTextBuffer;

extern  void                gwrgtk_textview_set_buffer          (GwrGtkTextView*, GwrGtkTextBuffer*);
extern  void                gwrgtk_textview_set_font_size       (GwrGtkTextView*, guint32);
extern  GtkAdjustment   *   gwrgtk_textview_get_vadjustment     (GwrGtkTextView*);
extern  GtkAdjustment   *   gwrgtk_textview_get_hadjustment     (GwrGtkTextView*);
extern  void                gwrgtk_textview_set_scroll_pos_x    (GwrGtkTextView*, gdouble _xpos);
extern  void                gwrgtk_textview_set_scroll_pos_y    (GwrGtkTextView*, gdouble _ypos);

extern  void                gwrgtk_textview_one_line_up         (GwrGtkTextView*);
extern  void                gwrgtk_textview_one_line_down       (GwrGtkTextView*);
extern  void                gwrgtk_textview_one_page_up         (GwrGtkTextView*);
extern  void                gwrgtk_textview_one_page_down       (GwrGtkTextView*);

extern  void                gwrgtk_textview_refresh             (GwrGtkTextView*);

#if ( __cplusplus )
}
#endif






#endif                                                                          // __GWRGTK_TEXT_VIEW_H__
