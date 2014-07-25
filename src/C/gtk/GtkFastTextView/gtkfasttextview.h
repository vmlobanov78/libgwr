/*
    *****************************************************************************
    *                                                                           *
    *   gtkfasttextview.h                                                       *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Copyright (C) 2011-2014 Guillaume Wardavoir                             *
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
    *   Purpose : Simple and fast gtk compatible textview widget                *
    *                                                                           *
    *****************************************************************************
*/
#ifndef __GTK_FAST_TEXT_VIEW_H__
#define __GTK_FAST_TEXT_VIEW_H__

//  ............................................................................
#include <glib-object.h>
#include <gtk/gtk.h>

#include "C/buffers/libgwrc-fast-text-buffer.h"
//  ............................................................................
G_BEGIN_DECLS

#define GTK_FAST_TEXT_VIEW_TYPE             ( gtk_fast_text_view_get_type   ()                                                          )
#define GTK_FAST_TEXT_VIEW(obj)             ( G_TYPE_CHECK_INSTANCE_CAST    ( (obj)     , GTK_FAST_TEXT_VIEW_TYPE, GtkFastTextView      )   )
#define GTK_FAST_TEXT_VIEW_CLASS(klass)     ( G_TYPE_CHECK_CLASS_CAST       ( (klass)   , GTK_FAST_TEXT_VIEW_TYPE, GtkFastTextViewClass )   )
#define GTK_IS_FAST_TEXT_VIEW(obj)          ( G_TYPE_CHECK_INSTANCE_TYPE    ( (obj)     , GTK_FAST_TEXT_VIEW_TYPE                       )   )
#define GTK_IS_FAST_TEXT_VIEW_CLASS(klass)  ( G_TYPE_CHECK_CLASS_TYPE       ( (klass)   , GTK_FAST_TEXT_VIEW_TYPE                       )   )

typedef struct _GtkFastTextView          GtkFastTextView;
typedef struct _GtkFastTextViewClass     GtkFastTextViewClass;

struct _GtkFastTextView
{
    GtkWidgetClass  parent_instance;
};

struct _GtkFastTextViewClass
{
    GtkWidgetClass  parent_class;
};

GType           gtk_fast_text_view_get_type (void) G_GNUC_CONST;
GtkWidget   *   gtk_fast_text_view_new      (GwrFastTextBuffer*);

G_END_DECLS

#if ( __cplusplus )
extern "C" {
#endif

typedef struct  _GwrFastTextBuffer           GwrFastTextBuffer;

extern  void                gtk_fast_text_view_set_buffer           (GtkFastTextView*, GwrFastTextBuffer*);
extern  GwrFastTextBuffer*   gtk_fast_text_view_get_buffer           (GtkFastTextView*);

extern  void                gtk_fast_text_view_set_font_size        (GtkFastTextView*, guint32);
extern  void                gtk_fast_text_view_set_color            (GtkFastTextView*, guint32 _color_index, guint8 _r, guint8 _g, guint8 _b);

extern  void                gtk_fast_text_view_refresh              (GtkFastTextView*);

#if ( __cplusplus )
}
#endif






#endif                                                                          // __GWRGTK_TEXT_VIEW_H__
