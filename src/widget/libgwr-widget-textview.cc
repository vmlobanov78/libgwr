/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-textview.cc                                               *
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
    *   Class   : GwrTextView                                                   *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

//  ............................................................................
#include    "../libgwr.h"
#include    <gdk/gdkkeysyms.h>
#include     <gdk/gdkkeysyms-compat.h>                                          //  for GDK_KP_Enter
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)
//  ############################################################################
//
//  GwrTextView::TextTagHelper
//
//  ############################################################################
GwrTextView::TextTagHelper::TextTagHelper(
    GtkTextBuffer   *   _buffer)
{
    guint           i;
    GtkTextTag  *   temp;
    //..........................................................................
    g_return_if_fail( _buffer );

    a_text_buffer   =   _buffer;

    // style tags
    d_tag_und       =   gtk_text_buffer_create_tag(a_text_buffer, NULL, "underline",        PANGO_UNDERLINE_SINGLE, NULL);
    d_tag_bld       =   gtk_text_buffer_create_tag(a_text_buffer, NULL, "weight",           PANGO_WEIGHT_BOLD,      NULL);
    d_tag_ita       =   gtk_text_buffer_create_tag(a_text_buffer, NULL, "style",            PANGO_STYLE_ITALIC,     NULL);
    d_tag_stk       =   gtk_text_buffer_create_tag(a_text_buffer, NULL, "strikethrough",    TRUE,                   NULL);

    d_tags_color_fg =   g_array_new(FALSE, TRUE, sizeof(GtkTextTag*));
    d_tags_color_bg =   g_array_new(FALSE, TRUE, sizeof(GtkTextTag*));

    // color array : not used for now
    d_colors        = NULL;

    // color tags
    for ( i = 0 ; i < LIBGWR_COLOR_DEFINED_CARD ; i++ )
    {
        temp = gtk_text_buffer_create_tag(a_text_buffer, NULL, "foreground", color::Html(i), NULL);
        g_array_append_val(d_tags_color_fg, temp);
        temp = gtk_text_buffer_create_tag(a_text_buffer, NULL, "background", color::Html(i), NULL);
        g_array_append_val(d_tags_color_bg, temp);
    }
}

GwrTextView::TextTagHelper::~TextTagHelper()
{
    g_array_free(d_tags_color_fg, FALSE);                                       // GWR_TODO : delete GtkTextTags
    g_array_free(d_tags_color_bg, FALSE);
}

void
GwrTextView::TextTagHelper::build_tag_list(
    GSList  **  _slist,
    guint32     _flags)
{
    GSList  *   slist   = NULL;
    //..........................................................................
    g_return_if_fail( _slist );

    // get GSList value
    slist   =   *(_slist);

    if ( _flags & ts::Und )   slist = g_slist_append(slist, d_tag_und);
    if ( _flags & ts::Bld )   slist = g_slist_append(slist, d_tag_bld);
    if ( _flags & ts::Ita )   slist = g_slist_append(slist, d_tag_ita);
    if ( _flags & ts::Stk )   slist = g_slist_append(slist, d_tag_stk);

    if ( _flags & ts::Fg )
    {
        slist = g_slist_append(slist, (gpointer)get_tag_color_fg( (_flags & 0xFF000000) >> 24 ) );
    }

    if ( _flags & ts::Bg )
    {
        slist = g_slist_append(slist, (gpointer)get_tag_color_bg( (_flags & 0x00FF0000) >> 16 ) );
    }

    // set GSList modified value
    *(_slist)   = slist;
}


//  ############################################################################
//
//  GwrTextView::Model
//
//  ############################################################################
GwrTextView::Model::Model()
{
    d_gtk_text_buffer   =   gtk_text_buffer_new(gtk_text_tag_table_new());
    d_text_tag_helper   =   GWR_NEW( TextTagHelper, d_gtk_text_buffer );

    a_count             = 0;
}
GwrTextView::Model::~Model()
{
}
void
GwrTextView::Model::clear()
{
    GtkTextIter         iter_start, iter_end;
    //..........................................................................
    a_count = 0;
    gtk_text_buffer_get_start_iter  (buffer(), &iter_start);                    // get start of buffer
    gtk_text_buffer_get_end_iter    (buffer(), &iter_end);                      // get end of buffer

    gtk_text_buffer_delete(buffer(), &iter_start, &iter_end);
}
void
GwrTextView::Model::append(
    const   gchar     *   _txt,
            guint32       _tag)
{
    GtkTextMark     *   mark = NULL;
    GtkTextIter         iter_start, iter_end;
    gint                offset_start;
    GSList          *   slist       = NULL;
    //..........................................................................
    a_count++;

    // build GtkTextTag* list
    d_text_tag_helper->build_tag_list(&slist, _tag);

    gtk_text_buffer_get_end_iter(buffer(), &iter_end);                          // get end of buffer iter   = start of our text
    offset_start = gtk_text_iter_get_offset(&iter_end);                         // get end of buffer offset = start of our text

    gtk_text_buffer_insert(buffer(), &iter_end, _txt, -1);

    gtk_text_buffer_get_end_iter(buffer(), &iter_end);                          // get end of buffer iter = end of our text

    gtk_text_buffer_get_iter_at_offset(buffer(), &iter_start, offset_start);    // iters change, have to recompute the start iter

    slist = g_slist_nth(slist, 0);
    while( slist )
    {
        gtk_text_buffer_apply_tag(
            buffer(), (GtkTextTag*)slist->data,
            &iter_start, &iter_end);

        slist = g_slist_next(slist);
    }
}
//  ############################################################################
//
//  GwrTextView::View
//
//  ############################################################################
GwrTextView::View::View(
    Control     *   _control,
    gboolean        _editable)
{
    g_return_if_fail( _control );

    a_control               = _control;
    a_text_buffer           = control()->model()->buffer();

    d_gtk_text_view         =   gtk_text_view_new_with_buffer(a_text_buffer);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(d_gtk_text_view), _editable);
    d_gtk_scrolled_window   =   gtk_scrolled_window_new( NULL, NULL );

    d_gtk_lab1              =   gtk_label_new("Find : ");
    d_gtk_entry_find        =   gtk_entry_new();
    d_gtk_hbox              =   gtk_hbox_new(FALSE, 0);

    d_gtk_vbox              =   gtk_vbox_new(FALSE, 0);

    g_signal_connect(d_gtk_text_view    , "key-release-event", G_CALLBACK(View::Sgn_key_release_event), this);
    g_signal_connect(d_gtk_entry_find   , "key-release-event", G_CALLBACK(View::Sgn_entry_find_key_release_event), this);

    gtk_container_add(GTK_CONTAINER(d_gtk_scrolled_window)  , d_gtk_text_view);

    gtk_box_pack_start(GTK_BOX(d_gtk_hbox), d_gtk_lab1              , FALSE , TRUE, 0);
    gtk_box_pack_start(GTK_BOX(d_gtk_hbox), d_gtk_entry_find        ,  TRUE , TRUE, 0);

    gtk_box_pack_start(GTK_BOX(d_gtk_vbox), d_gtk_scrolled_window   ,  TRUE , TRUE, 0);
    gtk_box_pack_start(GTK_BOX(d_gtk_vbox), d_gtk_hbox              , FALSE , TRUE, 0);

    a_is_shown              = TRUE;

    d_find_text             = NULL;
    d_find_text_ck          = NULL;
}
GwrTextView::View::~View()
{
}
//  ----------------------------------------------------------------------------
void
GwrTextView::View::scroll(GtkTextIter* _iter)
{
    gtk_text_view_scroll_to_iter(
        GTK_TEXT_VIEW(d_gtk_text_view),
        _iter,
        (gdouble)0.0,
        FALSE,
        (gdouble)0.0,
        (gdouble)0.0);
}
//  ----------------------------------------------------------------------------
void
GwrTextView::View::scroll_to_end()
{
    GtkTextIter iter;
    /*
    //..........................................................................
    gtk_text_buffer_get_end_iter(a_text_buffer, &iter);

    gtk_text_view_scroll_to_iter(
        GTK_TEXT_VIEW(d_gtk_text_view),
        &iter,
        (gdouble)0.0,
        FALSE,
        (gdouble)0.0,
        (gdouble)0.0);
    */
}
//  ----------------------------------------------------------------------------
void
GwrTextView::View::set_font_monospace(
    guint32 _size)
{
    gchar   font_desc[32];
    //..........................................................................
    sprintf(font_desc, "Monospace %i", _size);

    gtk_widget_modify_font(
        d_gtk_text_view,
        pango_font_description_from_string (font_desc));
}
//  ----------------------------------------------------------------------------
void
GwrTextView::View::set_color_bg(
    gchar   *   _color)
{
    GdkColor color;
    //..........................................................................
    gdk_color_parse(_color, &color);

    gtk_widget_modify_base(d_gtk_text_view, GTK_STATE_NORMAL, &color);
}
//  ----------------------------------------------------------------------------
//  View::find
//  ----------------------------------------------------------------------------
void
GwrTextView::View::find_entry_show(
    gboolean    _bsh)
{
    _bsh                                ?
        gtk_widget_show( d_gtk_hbox )   :
        gtk_widget_hide( d_gtk_hbox )   ;

    a_is_shown = _bsh;

    if ( a_is_shown )
        gtk_widget_grab_focus(d_gtk_entry_find);
}
void
GwrTextView::View::find_entry_show_toggle()
{
    find_entry_show( a_is_shown ? FALSE : TRUE );
}
void
GwrTextView::View::_find_text_wrap_start_iter()
{
    gtk_text_buffer_get_start_iter(
        a_text_buffer           ,
        &a_find_iter_start    );
}
//  ----------------------------------------------------------------------------
gboolean
GwrTextView::View::_find_text_find()
{
    return gtk_text_iter_forward_search(
            &a_find_iter_start                  ,
            d_find_text                         ,
            (GtkTextSearchFlags)0               ,
            &a_find_iter_match_start            ,
            &a_find_iter_match_end              ,
            NULL                                );                              // search until end of buffer
}
gboolean
GwrTextView::View::_find_text_and_select()
{
    if ( ! _find_text_find() )
        return FALSE;

    scroll( &a_find_iter_match_start );

    gtk_text_buffer_select_range(
        a_text_buffer               ,
        &a_find_iter_match_start    ,
        &a_find_iter_match_end      );

    a_find_iter_start = a_find_iter_match_end;

    return TRUE;
}
void
GwrTextView::View::find_text(
    const gchar     *   _st)
{
    gchar           *   ck          = NULL;
    //..........................................................................
    if ( _st )
    {
        if ( d_find_text )
        {
            ck = g_utf8_collate_key(_st, -1);
            if ( strcmp( d_find_text_ck, ck ) == 0 )
                goto lab_find_next;

            goto lab_find_first;
        }
        else
        {
            goto lab_find_first;
        }
    }
    else
    {
        if ( d_find_text_ck )
            goto lab_find_next;

        return;
    }
    //  ........................................................................
lab_find_first:

    d_find_text       = g_strdup(_st);
    d_find_text_ck    = g_utf8_collate_key(d_find_text, -1);
    _find_text_wrap_start_iter();
    //  ........................................................................
lab_find_next:

    if ( ! _find_text_and_select() )
    {
        _find_text_wrap_start_iter();
        _find_text_and_select();
    }
}
//  ----------------------------------------------------------------------------
//  View::Sgn_key_release_event()
//  ----------------------------------------------------------------------------
gboolean
GwrTextView::View::Sgn_key_release_event(
    GtkWidget   *   _widget   ,
    GdkEvent    *   _event    ,
    gpointer        _data)
{
    static  gchar                   str[256];
            View                *   THIS    = NULL;
            GdkEventKey         *   ek      = NULL;
    //..........................................................................
    THIS    =   (View*)_data;
    ek      =   (GdkEventKey*)_event;

    if ( ( ek->keyval == GDK_KEY_f ) && ( ek->state & GDK_CONTROL_MASK ) )
    {
        THIS->find_entry_show_toggle();
        goto lab_end;
    }

    if ( ek->keyval == GDK_KEY_F3 )
    {
        THIS->find_text();
        goto lab_end;
    }

    if ( ek->keyval == GDK_KEY_m )
    {
        sprintf(str, "lines:%i\n", gtk_text_buffer_get_line_count(THIS->a_text_buffer));
        THIS->control()->model()->append( str, 0);
    }

    //  ........................................................................
lab_end:
    //  allow other handlers
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  View::Sgn_entry_find_key_release_event()
//  ----------------------------------------------------------------------------
gboolean
GwrTextView::View::Sgn_entry_find_key_release_event(
    GtkWidget   *   _widget   ,
    GdkEvent    *   _event    ,
    gpointer        _data)
{
    View                        *   THIS    = NULL;
    GdkEventKey                 *   ek      = NULL;
    const   gchar               *   txt     = NULL;
    //..........................................................................
    THIS    =   (View*)_data;
    ek      =   (GdkEventKey*)_event;

    if ( ( ek->keyval == GDK_KEY_f ) && ( ek->state & GDK_CONTROL_MASK ) )
    {
        THIS->find_entry_show_toggle();
        goto lab_end;
    }

    if  (
            ( ek->keyval != GDK_KP_Enter    )   &&
            ( ek->keyval != GDK_Return      )   &&
            ( ek->keyval != GDK_KEY_F3      )
        )
        goto lab_end;

    txt = gtk_entry_get_text( GTK_ENTRY(THIS->d_gtk_entry_find) );

    if ( txt )
        THIS->find_text( txt );

    //  ........................................................................
lab_end:
    //  allow other handlers
    return FALSE;
}
//  ############################################################################
//
//  GwrTextView::Control
//
//  ############################################################################
GwrTextView::Control::Control(gboolean _editable)
{
    d_model     = GWR_NEW( Model );
    d_view      = GWR_NEW( View, this , _editable );
}
GwrTextView::Control::~Control()
{
}
//  ############################################################################
//
//  GwrTextView
//
//  ############################################################################
GwrTextView::GwrTextView(gboolean _editable)
{
    d_control   = GWR_NEW( Control, _editable );
}

GwrTextView::~GwrTextView()
{
}


GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)
