/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-gtk-fasttextview.h                                        *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libgwr                                                          *
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
    *   Class   : libgwr::widget::gtk::FastTextView                             *
    *                                                                           *
    *   Purpose : It is in the name                                             *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_WIDGET_GTK_FASTTEXTVIEW_H__
#define     __LIBGWR_WIDGET_GTK_FASTTEXTVIEW_H__

//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)
GWR_NAMESPACE_START(gtk)

/// ****************************************************************************
//!
//! \class  FastTextView
//!
//! \brief  The purpose of this class is to have a GtkWidget for displaying text,
//!     but as fast as possible, unlike GtkTextView.
//!
/// ****************************************************************************
class FastTextView
{
    //  ========================================================================
    private:
    GtkAdjustment       *   d_vadjust;
    GtkAdjustment       *   d_hadjust;

    GtkWidget           *   d_vscrollbar;
    GtkWidget           *   d_hscrollbar;

    GtkWidget           *   d_hbox;
    GtkWidget           *   d_vbox;

    GtkWidget           *   d_gwrgtk_textview;
    GwrGtkTextBuffer    *   d_gwrgtk_textbuffer;
    //  ------------------------------------------------------------------------
    private:
    GtkWidget           *   vscroll()       { return d_vscrollbar;          }
    GtkWidget           *   hscroll()       { return d_hscrollbar;          }
    GtkWidget           *   vbox()          { return d_vbox;                }
    GtkWidget           *   hbox()          { return d_hbox;                }

    public:
    GtkWidget           *   textview_w()    { return d_gwrgtk_textview;                     }
    GtkFastTextView     *   textview()      { return GTK_FAST_TEXT_VIEW(d_gwrgtk_textview); }
    GwrGtkTextBuffer    *   textbuffer()    { return d_gwrgtk_textbuffer;                   }

    GtkWidget           *   widget()        { return hbox();                }
    //  ========================================================================
    public:
    void                    sync();
    //  ========================================================================
    public:
    void                    print_internal_stats();
    //  ========================================================================
    static  gboolean    VScrollBar_change_value(GtkRange*, GtkScrollType, gdouble, gpointer);
    static  gboolean    HScrollBar_change_value(GtkRange*, GtkScrollType, gdouble, gpointer);

    //  ========================================================================
    //  ()~()
    //  ========================================================================
    public:
             FastTextView(
                guint32     _text_block_size            ,
                guint32     _text_block_store_realloc   ,
                guint32     _text_desc_store_realloc    ,
                guint32     _desc_realloc               );

    virtual ~FastTextView() {}
};

GWR_NAMESPACE_END(gtk)
GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)

#endif                                                                          // #define __LIBGWR_WIDGET_GTK_FASTTEXTVIEW_H__
