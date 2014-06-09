/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-gtk-fasttextview.cc                                       *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libgwr                                                          *
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
    *   Class   : libgwr::widget::gtk::FastTextView                             *
    *                                                                           *
    *   Purpose : It is in the name                                             *
    *                                                                           *
    *****************************************************************************
*/

//  ............................................................................
//  libgwr.h will also include the two C components :
//      - GwrGtkTextView
//      - GwrGtkTextBuffer
#include "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)
GWR_NAMESPACE_START(gtk)

gboolean
FastTextView::VScrollBar_change_value(
    GtkRange     *  _range  ,
    GtkScrollType   _scroll ,
    gdouble         _value  ,
    gpointer        _data   )
{
    FastTextView    *   THIS    =   (FastTextView*)_data;
    //  ........................................................................
    gwrgtk_textview_set_scroll_pos_y( GWRGTK_TEXTVIEW(THIS->textview()), _value );
    return FALSE;                                                               //  propagate
}
gboolean
FastTextView::HScrollBar_change_value(
    GtkRange     *  _range  ,
    GtkScrollType   _scroll ,
    gdouble         _value  ,
    gpointer        _data   )
{
    FastTextView    *   THIS    =   (FastTextView*)_data;
    //  ........................................................................
    gwrgtk_textview_set_scroll_pos_x( GWRGTK_TEXTVIEW(THIS->textview()), _value );
    return FALSE;                                                               //  propagate
}

FastTextView::FastTextView(
    guint32     _text_block_size            ,
    guint32     _text_block_store_realloc   ,
    guint32     _text_desc_store_realloc    ,
    guint32     _desc_block_size            ,
    guint32     _desc_block_store_realloc   ,
    guint32     _desc_desc_store_realloc    )
{
    d_gwrgtk_textview   =   gwrgtk_textview_new();
    d_gwrgtk_textbuffer =   gwrgtk_text_buffer_new(
                                _text_block_size, _text_block_store_realloc, _text_desc_store_realloc   ,
                                _desc_block_size, _desc_block_store_realloc, _desc_desc_store_realloc   );
    gwrgtk_textview_set_buffer( GWRGTK_TEXTVIEW(textview()), textbuffer() );

    d_vscrollbar        =   gtk_scrollbar_new( GTK_ORIENTATION_VERTICAL     , gwrgtk_textview_get_vadjustment(textview()) );
    d_hscrollbar        =   gtk_scrollbar_new( GTK_ORIENTATION_HORIZONTAL   , gwrgtk_textview_get_hadjustment(textview()) );

    d_hbox              =   gtk_box_new( GTK_ORIENTATION_HORIZONTAL , 0);
    d_vbox              =   gtk_box_new( GTK_ORIENTATION_VERTICAL   , 0);

    gtk_box_pack_start( GTK_BOX(vbox()), textview_w()   , TRUE , TRUE , 0 );
    gtk_box_pack_start( GTK_BOX(vbox()), hscroll()      , FALSE, FALSE, 0 );
    gtk_box_pack_start( GTK_BOX(hbox()), vbox()         , TRUE , TRUE , 0 );
    gtk_box_pack_start( GTK_BOX(hbox()), vscroll()      , FALSE, FALSE, 0 );

    g_signal_connect( vscroll(), "change-value", G_CALLBACK(VScrollBar_change_value), this );
    g_signal_connect( hscroll(), "change-value", G_CALLBACK(HScrollBar_change_value), this );

    //gtk_widget_set_events( GTK_WIDGET(d_gwrgtk_textview), GDK_ALL_EVENTS_MASK );
}

void
FastTextView::print_internal_stats()
{
    gchar                   stat[1024];
    GwrGtkTextBufferStat    s;
    //  ........................................................................
    gwrgtk_text_buffer_get_stats_alloc( textbuffer(), &s );

    sprintf(stat, "GwrArrayDataMulti lines DATA (text) stats:");
    gwrgtk_text_buffer_add_line( textbuffer(), 0, stat );
    sprintf(stat, "  #:%6i size:%8i", s.lt.a_data_card, s.lt.a_data_size );
    gwrgtk_text_buffer_add_line( textbuffer(), 0, stat );
    sprintf(stat, "  blocks:%6i (used:%6i) allocations since last reset:%6i",
        s.lt.a_block_card   , s.lt.a_block_used , s.lt.a_block_alloc    );
    gwrgtk_text_buffer_add_line( textbuffer(), 0, stat );
    sprintf(stat, "  descs :%6i (used:%6i) allocations since last reset:%6i",
        s.lt.a_desc_card    , s.lt.a_desc_used  , s.lt.a_desc_alloc     );
    gwrgtk_text_buffer_add_line( textbuffer(), 0, stat );

    sprintf(stat, "GwrArrayDataMulti lines METADADATA stats:");
    gwrgtk_text_buffer_add_line( textbuffer(), 0, stat );
    sprintf(stat, "  #:%6i size:%8i", s.ld.a_data_card, s.ld.a_data_size );
    gwrgtk_text_buffer_add_line( textbuffer(), 0, stat );
    sprintf(stat, "  blocks:%6i (used:%6i) allocations since last reset:%6i",
        s.ld.a_block_card   , s.ld.a_block_used , s.ld.a_block_alloc    );
    gwrgtk_text_buffer_add_line( textbuffer(), 0, stat );
    sprintf(stat, "  descs :%6i (used:%6i) allocations since last reset:%6i",
        s.ld.a_desc_card    , s.ld.a_desc_used  , s.ld.a_desc_alloc     );
    gwrgtk_text_buffer_add_line( textbuffer(), 0, stat );
}


GWR_NAMESPACE_END(gtk)
GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)

