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
#include    "libgwr.hh"
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
    d_tag_htm       =   gtk_text_buffer_create_tag(a_text_buffer, NULL,
                            "foreground"    ,   "#0000ff"                       ,
                            "underline"     ,    PANGO_UNDERLINE_SINGLE         , NULL);

    d_tags_color_fg =   g_array_new(FALSE, TRUE, sizeof(GtkTextTag*));
    d_tags_color_bg =   g_array_new(FALSE, TRUE, sizeof(GtkTextTag*));

    // color array : not used for now
    d_colors        = NULL;

    // color tags
    for ( i = 0 ; i < LIBGWR_COLOR_DEFINED_CARD ; i++ )
    {
        temp = gtk_text_buffer_create_tag(a_text_buffer, NULL, "foreground", color::Colors[i]->html(), NULL);
        g_array_append_val(d_tags_color_fg, temp);
        temp = gtk_text_buffer_create_tag(a_text_buffer, NULL, "background", color::Colors[i]->html(), NULL);
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

    if ( _flags & libgwr::text::attributes::Und  )   slist = g_slist_append(slist, d_tag_und);
    if ( _flags & libgwr::text::attributes::Bld  )   slist = g_slist_append(slist, d_tag_bld);
    if ( _flags & libgwr::text::attributes::Ita  )   slist = g_slist_append(slist, d_tag_ita);
    if ( _flags & libgwr::text::attributes::Stk  )   slist = g_slist_append(slist, d_tag_stk);
    if ( _flags & libgwr::text::attributes::Htm  )   slist = g_slist_append(slist, d_tag_htm);

    if ( _flags & libgwr::text::attributes::FgColor )
    {
        slist = g_slist_append(slist, (gpointer)get_tag_color_fg( (_flags & 0xFF000000) >> 24 ) );
    }

    if ( _flags & libgwr::text::attributes::BgColor )
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
    d_gtk_text_buffer       =   gtk_text_buffer_new(gtk_text_tag_table_new());
    d_text_tag_helper       =   GWR_NEW( TextTagHelper, d_gtk_text_buffer );
    a_cursor_roves_initial_offset_memorized = FALSE;
}
GwrTextView::Model::~Model()
{
}

gchar*
GwrTextView::Model::get_text()
{
    GtkTextIter         iter_start, iter_end;
    //..........................................................................
    gtk_text_buffer_get_start_iter  ( textbuffer(), &iter_start );
    gtk_text_buffer_get_end_iter    ( textbuffer(), &iter_end   );

    return gtk_text_buffer_get_text( textbuffer(), &iter_start, &iter_end, FALSE );
}
void
GwrTextView::Model::set_text(const gchar* _txt)
{
    gtk_text_buffer_set_text( textbuffer(), _txt, -1 );
}

gboolean            GwrTextView::Model::cursor_roves_initial_offset_memorized()
{
    return a_cursor_roves_initial_offset_memorized;
}
void                GwrTextView::Model::cursor_roves_initial_offset_memorize()
{
    GtkTextMark     *   ins_mark    =   NULL;
    GtkTextIter         ins_iter;
    //  ........................................................................
    g_return_if_fail( ! cursor_roves_initial_offset_memorized() );

    ins_mark                =   gtk_text_buffer_get_insert( textbuffer() );     //  cursor = "insert" mark
    gtk_text_buffer_get_iter_at_mark( textbuffer(), &ins_iter, ins_mark );      //  get iter
    a_cursor_roves_initial_offset   =   gtk_text_iter_get_offset( &ins_iter );  //  get offset

    a_cursor_roves_initial_offset_memorized = TRUE;

    LIBGWR_INF("Memorized offset %i", a_cursor_roves_initial_offset);
}
void                GwrTextView::Model::cursor_roves_initial_offset_recall()
{
    GtkTextIter         ins_iter;
    //  ........................................................................
    if ( ! cursor_roves_initial_offset_memorized() )
        return;

    LIBGWR_INF("Recalling offset %i", a_cursor_roves_initial_offset);

    gtk_text_buffer_get_iter_at_offset(                                         //  get iter from memorized offset in buffer
        textbuffer()                    ,
        &ins_iter                       ,
        a_cursor_roves_initial_offset   );

    //  calling this make a selection, because the "insert" mark moves, but not
    //  the "selection_bound" mark !
    //gtk_text_buffer_move_mark(
    //    textbuffer()            ,
    //    ins_mark                ,
    //    &ins_iter               );

    gtk_text_buffer_place_cursor(                                               //  set cursor position
        textbuffer()        ,
        &ins_iter           );

    a_cursor_roves_initial_offset_memorized = FALSE;
}
void
GwrTextView::Model::cursor_print()
{
    GtkTextMark     *   ins_mark    =   NULL;
    GtkTextIter         ins_iter;
    gint                v           =   0;
    //  ........................................................................
    ins_mark                =   gtk_text_buffer_get_insert( textbuffer() );     //  cursor = "insert" mark
    gtk_text_buffer_get_iter_at_mark( textbuffer(), &ins_iter, ins_mark );      //  get iter
    v                       =   gtk_text_iter_get_offset( &ins_iter );          //  get offset

    LIBGWR_INF("Offset %i", v);
}

void
GwrTextView::Model::clear()
{
    GtkTextIter         iter_start, iter_end;
    //..........................................................................
    gtk_text_buffer_get_start_iter  (textbuffer(), &iter_start);                // get start of buffer
    gtk_text_buffer_get_end_iter    (textbuffer(), &iter_end);                  // get end of buffer

    gtk_text_buffer_delete(textbuffer(), &iter_start, &iter_end);
}
void
GwrTextView::Model::append(
    const   gchar     *   _txt,
            guint32       _tag)
{
    GtkTextIter         iter_start, iter_end;
    gint                offset_start;
    GSList          *   slist       = NULL;
    //..........................................................................
    // build GtkTextTag* list
    d_text_tag_helper->build_tag_list(&slist, _tag);

    gtk_text_buffer_get_end_iter(textbuffer(), &iter_end);                      // get end of buffer iter   = start of our text
    offset_start = gtk_text_iter_get_offset(&iter_end);                         // get end of buffer offset = start of our text

    gtk_text_buffer_insert(textbuffer(), &iter_end, _txt, -1);                  // insert text

    gtk_text_buffer_get_end_iter(textbuffer(), &iter_end);                      // iter_end has changed, have to recompute it

    gtk_text_buffer_get_iter_at_offset(textbuffer(), &iter_start, offset_start);// iter_start

    slist = g_slist_nth(slist, 0);
    while( slist )
    {
        gtk_text_buffer_apply_tag(
            textbuffer(), (GtkTextTag*)slist->data  ,
            &iter_start, &iter_end                  );

        slist = g_slist_next(slist);
    }
}
void
GwrTextView::Model::append_html(
    const   gchar     *   _txt)
{
    GtkTextIter         iter_start, iter_end;
    gint                offset_start;
    //..........................................................................
    gtk_text_buffer_get_end_iter(textbuffer(), &iter_end);                      // get end of buffer iter   = start of our text
    offset_start = gtk_text_iter_get_offset(&iter_end);                         // get end of buffer offset = start of our text

    gtk_text_buffer_insert(textbuffer(), &iter_end, _txt, -1);                  // insert text

    gtk_text_buffer_get_end_iter(textbuffer(), &iter_end);                      // iter_end has changed, have to recompute it

    gtk_text_buffer_get_iter_at_offset(textbuffer(), &iter_start, offset_start);// iter_start

    gtk_text_buffer_apply_tag(
        textbuffer()                        ,
        d_text_tag_helper->get_tag_htm()    ,
        &iter_start, &iter_end              );
}
//  ############################################################################
//
//  GwrTextView::View
//
//  ############################################################################
GwrTextView::View::View(
    GwrTextView *   _control,
    gboolean        _editable)
{
    //  ........................................................................
    g_return_if_fail( _control );

    Static_init();

    a_control               = _control;
    a_text_buffer           = control()->model()->textbuffer();

    d_gtk_text_view         =   gtk_text_view_new_with_buffer(a_text_buffer);
    a_buffer_connected      =   TRUE;

    gtk_text_view_set_editable(GTK_TEXT_VIEW(d_gtk_text_view), _editable);
    d_gtk_scrolled_window   =   gtk_scrolled_window_new( NULL, NULL );

    d_gtk_lab1              =   gtk_label_new("Find : ");
    d_gtk_entry_find        =   gtk_entry_new();
    d_gtk_hbox              =   gwrgtk_hbox_new(FALSE, 0);

    d_gtk_vbox              =   gwrgtk_vbox_new(FALSE, 0);

    g_signal_connect(d_gtk_text_view    , "key-release-event"   , G_CALLBACK(View::Sgn_key_release_event), this);
    g_signal_connect(d_gtk_text_view    , "button-press-event"  , G_CALLBACK(View::Sgn_button_press_event), this);
    g_signal_connect(d_gtk_entry_find   , "key-release-event"   , G_CALLBACK(View::Sgn_entry_find_key_release_event), this);

    //  add the event for urls
    gtk_widget_add_events( d_gtk_text_view, GDK_POINTER_MOTION_HINT_MASK );
    g_signal_connect(d_gtk_text_view    , "motion-notify-event" , G_CALLBACK(View::Sgn_motion_notify_event), this);

    gtk_container_add(GTK_CONTAINER(d_gtk_scrolled_window)  , d_gtk_text_view);

    gtk_box_pack_start(GTK_BOX(d_gtk_hbox), d_gtk_lab1              , FALSE , TRUE, 0);
    gtk_box_pack_start(GTK_BOX(d_gtk_hbox), d_gtk_entry_find        ,  TRUE , TRUE, 0);

    gtk_box_pack_start(GTK_BOX(d_gtk_vbox), d_gtk_scrolled_window   ,  TRUE , TRUE, 0);
    gtk_box_pack_start(GTK_BOX(d_gtk_vbox), d_gtk_hbox              , FALSE , TRUE, 0);

    a_is_shown              =   TRUE;

    d_find_text             =   NULL;
    d_find_text_ck          =   NULL;

    d_mark_find_start       =   NULL;
}
GwrTextView::View::~View()
{
    //g_object_unref( d_scroll_mark );
}
//  ----------------------------------------------------------------------------
//  View - static init
//  ----------------------------------------------------------------------------
gboolean        GwrTextView::View::s_initialized    =   FALSE;
void
GwrTextView::View::Static_init()
{
    if ( s_initialized )
        return;

    s_cursor_arrow      =   gdk_cursor_new(GDK_HAND2);
    s_cursor_default    =   gdk_cursor_new(GDK_XTERM);
}
//  ----------------------------------------------------------------------------
void            GwrTextView::View::buffer_disconnect()
{
    if ( a_buffer_connected )
    {
        g_object_ref( (gpointer)textbuffer() );
        gtk_text_view_set_buffer( textview(), NULL );
        a_buffer_connected = FALSE;
    }
}
void            GwrTextView::View::buffer_reconnect()
{
    if ( ! a_buffer_connected )
    {
        gtk_text_view_set_buffer( textview(), textbuffer() );
        g_object_unref( (gpointer)textbuffer() );
        a_buffer_connected = TRUE;
    }
}
//  ----------------------------------------------------------------------------
//  View - cursor
//  ----------------------------------------------------------------------------
GdkCursor   *   GwrTextView::View::s_cursor_arrow   =   NULL;
GdkCursor   *   GwrTextView::View::s_cursor_default =   NULL;

void
GwrTextView::View::set_html_cursor(gboolean _on)
{
            GdkWindow           *   w       =   NULL;
            GdkCursor           *   cold    =   NULL;
            GdkCursor           *   cnew    =   NULL;
    //  ........................................................................
    w       =   gtk_text_view_get_window(
                    textview()              ,
                    GTK_TEXT_WINDOW_TEXT    );
    g_return_if_fail( w );

    cold                    =   gdk_window_get_cursor( w );

    if ( _on )
        cnew                =   View::s_cursor_arrow;
    else
        cnew                =   View::s_cursor_default;

    if ( cnew == cold )
        return;

    gdk_window_set_cursor( w, cnew );
}
//  ----------------------------------------------------------------------------
//  View - scolling
//  ----------------------------------------------------------------------------
void
GwrTextView::View::p0_scroll(GtkTextIter* _iter)
{
    gtk_text_view_scroll_to_iter(
        GTK_TEXT_VIEW(d_gtk_text_view),
        _iter,
        (gdouble)0.0,
        TRUE,
        (gdouble)0.0,
        (gdouble)0.5);
}
//  ----------------------------------------------------------------------------
void
GwrTextView::View::p0_scroll_to_line()
{
    GtkTextIter     iter;
    //  ........................................................................
    gtk_text_buffer_get_iter_at_line(
        textbuffer()            ,
        &iter                   ,
        a_scroll_to_line_lineno );

    p0_scroll( &iter );
}
//  ----------------------------------------------------------------------------
gboolean
GwrTextView::View::Scroll_to_line_timeout_func(gpointer    _data)
{
            GwrTextView::View   *   THIS    =   NULL;
    //  ........................................................................
    THIS    =   (GwrTextView::View*)_data;

    THIS->p0_scroll_to_line();

    THIS->a_scroll_to_line_lineno         = 0;
    THIS->a_scroll_to_line_timeout_id     = 0;

    return FALSE;                                                               //  cancel the timeout
}
//  ----------------------------------------------------------------------------
void
GwrTextView::View::scroll_to_line(
    guint32     _lineno     ,
    gboolean    _delayed    )
{
    gint            lineno  =   (gint)_lineno;
    //  ........................................................................
    if ( ! _delayed )
    {
        a_scroll_to_line_lineno =   lineno;
        p0_scroll_to_line();
        return;
    }

    a_scroll_to_line_lineno     =   lineno;
    a_scroll_to_line_timeout_id =   g_timeout_add(
        300                                             ,
        GwrTextView::View::Scroll_to_line_timeout_func  ,
        (gpointer)this                                  );
}
//  ----------------------------------------------------------------------------
void
GwrTextView::View::scroll_to_end()
{
}
//  ----------------------------------------------------------------------------
//  View - appearance
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
//  View - find
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
//  ----------------------------------------------------------------------------
void
GwrTextView::View::clear()
{
    _find_text_set_mark_find_start_at_buffer_top();
}
//  ----------------------------------------------------------------------------
gboolean
GwrTextView::View::gtk_text_iter_from_window_coords(gint _wx, gint _wy, GtkTextIter* _iter)
{
            GtkTextView         *   tv      =   NULL;
            GtkTextWindowType       wt;
            gint                    xbuf    =   0;
            gint                    ybuf    =   0;
    //  ........................................................................
    tv  =   textview();

    wt  =   gtk_text_view_get_window_type(
                tv                                      ,
                gtk_widget_get_window( GTK_WIDGET(tv) ) );

    gtk_text_view_window_to_buffer_coords(
        tv      ,
        wt      ,
        _wx     ,                                                               //  xin
        _wy     ,                                                               //  yin
        &xbuf   ,                                                               //  x out
        &ybuf   );                                                              //  y out

    gtk_text_view_get_iter_at_location(
        tv          ,
        _iter       ,
        xbuf        ,
        ybuf        );

    return TRUE;
}
//  ----------------------------------------------------------------------------
void
GwrTextView::View::_find_text_set_mark_find_start_at_buffer_top()
{
    GtkTextIter iter;
    //..........................................................................
    if ( d_mark_find_start )
    {
        gtk_text_buffer_delete_mark( textbuffer(), d_mark_find_start );
    }

    gtk_text_buffer_get_start_iter(
        textbuffer()                ,
        &iter                       );

    d_mark_find_start   =   gtk_text_buffer_create_mark(
                                textbuffer()            ,
                                "find-start"            ,
                                &iter                   ,
                                TRUE                    );
}
gboolean
GwrTextView::View::_find_text_and_select()
{
    gboolean        b                   = FALSE;
    GtkTextIter     iter;
    GtkTextIter     iter_match_start;
    GtkTextIter     iter_match_end;
    //  ........................................................................
    //  compute the iter from which to search
    g_return_val_if_fail( d_mark_find_start, FALSE );

    gtk_text_buffer_get_iter_at_mark(
        textbuffer()            ,
        &iter                   ,
        d_mark_find_start);

    b = gtk_text_iter_forward_search(
            &iter                               ,
            d_find_text                         ,
            (GtkTextSearchFlags)0               ,
            &iter_match_start                   ,
            &iter_match_end                     ,
            NULL                                );                              // search until end of buffer

    if ( ! b )
        return FALSE;

    p0_scroll( &iter_match_start );

    gtk_text_buffer_select_range(
        a_text_buffer               ,
        &iter_match_start           ,
        &iter_match_end             );

    gtk_text_buffer_delete_mark( textbuffer(), d_mark_find_start );
    d_mark_find_start   =   gtk_text_buffer_create_mark(
                                textbuffer()            ,
                                "find-start"            ,
                                &iter_match_end         ,
                                TRUE                    );

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
            {
                delete ck;
                goto lab_find_text_next_occurence;
            }
            delete ck;
            goto lab_find_text_first_occurence;
        }
        else
        {
            goto lab_find_text_first_occurence;
        }
    }
    else
    {
        if ( d_find_text_ck )
            goto lab_find_text_next_occurence;

        return;
    }
    //  ........................................................................
    //  start search from top of buffer
lab_find_text_first_occurence:

    g_free_safe( d_find_text    );
    g_free_safe( d_find_text_ck );

    d_find_text       = g_strdup(_st);
    d_find_text_ck    = g_utf8_collate_key(d_find_text, -1);

    _find_text_set_mark_find_start_at_buffer_top();
    //  ........................................................................
    //  continue search from last search result
lab_find_text_next_occurence:

    if ( ! _find_text_and_select() )
    {
        _find_text_set_mark_find_start_at_buffer_top();
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
    //LIBGWR_INF("%s", "GwrTextView::View::Sgn_key_release_event()");
    THIS    =   (View*)_data;
    ek      =   (GdkEventKey*)_event;

    if ( ( ek->keyval == GDK_KEY_z ) && ( ek->state & GDK_CONTROL_MASK ) )
    {
        THIS->control()->clear();
        goto lab_end;
    }

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
    return LIBGWR_GTK_EVENT_KEY_PROPAGATE_YES;
}
//  ----------------------------------------------------------------------------
//  View::Sgn_key_release_event()
//  ----------------------------------------------------------------------------
gboolean
GwrTextView::View::Sgn_button_press_event(
    GtkWidget   *   _widget   ,
    GdkEvent    *   _event    ,
    gpointer        _data)
{
            View                *   THIS    = NULL;
            GdkEventButton      *   evb     =   NULL;
    //  ........................................................................
    THIS    =   (View*)_data;
    evb     =   (GdkEventButton*)_event;

    THIS->control()->sgn_button_press_event( evb );

    return LIBGWR_GTK_EVENT_KEY_PROPAGATE_YES;
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
//  ----------------------------------------------------------------------------
//  View::Sgn_motion_notify_event()
//  ----------------------------------------------------------------------------
gboolean
GwrTextView::View::Sgn_motion_notify_event(
    GtkWidget   *   _widget,
    GdkEvent    *   _event,
    gpointer        _data)
{
    static  guint32                         Count   =   0;
            View                        *   THIS    =   NULL;
            GdkEventMotion              *   evm     =   NULL;
    //..........................................................................
    THIS    =   (View*)_data;
    evm     =   (GdkEventMotion*)_event;

    //  filter 1 on 10 event
    Count   =   ( Count + 1 ) % 10;
    if ( ! Count )
        THIS->control()->sgn_motion_notify_event( evm );

    return LIBGWR_GTK_EVENT_KEY_PROPAGATE_YES;
}
//  ############################################################################
//
//  GwrTextView
//
//  ############################################################################
GwrTextView::GwrTextView(gboolean _editable)
{
    d_html_callback     =   NULL;
    a_html_inside       =   FALSE;

    d_model             =   GWR_NEW_CAST( Model );
    d_view              =   GWR_NEW_CAST( View, this , _editable );
}
GwrTextView::~GwrTextView()
{
    if ( html_callback() )
        delete d_html_callback;
}


void
GwrTextView::html_callback_set(HtmlCallback * _hcb)
{
    //  ........................................................................
    g_return_if_fail( _hcb );



    d_html_callback =   GWR_NEW_CAST( HtmlCallback );
    d_html_callback->set( _hcb );
}


void
GwrTextView::sgn_button_press_event(GdkEventButton* _evb)
{
            GtkTextTag          *   htag    =   NULL;
            GtkTextIter             iter;
            GtkTextIter             iter1;
            GtkTextIter             iter2;
            gchar               *   dstr    =   NULL;
    //  ........................................................................
    if ( ! model()->cursor_roves_initial_offset_memorized() )
        model()->cursor_roves_initial_offset_memorize();

    if ( ! html_callback() )
        goto lab_end;

    if ( ! view()->gtk_text_iter_from_window_coords( _evb->x, _evb->y, &iter ) )
    {
        goto lab_end;
    }

    htag    =   model()->text_tag_helper()->get_tag_htm();

    if ( ! gtk_text_iter_has_tag( &iter, htag ) )
        goto lab_end;

    iter1 = iter;
    while ( ! gtk_text_iter_begins_tag( &iter1, htag ) )
    {
        if ( ! gtk_text_iter_backward_char( &iter1 ) )
            goto lab_end;
    }

    iter2 = iter;
    while ( ! gtk_text_iter_ends_tag( &iter2, htag ) )
    {
        if ( ! gtk_text_iter_forward_char( &iter2 ) )
            goto lab_end;
    }

    dstr = gtk_text_buffer_get_slice(
        model()->textbuffer()   ,
        &iter1                  ,
        &iter2                  ,
        FALSE                   );

    if ( dstr )
    {
        html_callback()->func()( dstr, html_callback()->data() );
        g_free( dstr );
    }

    //  ........................................................................
lab_end:
    return;
}
void
GwrTextView::sgn_motion_notify_event(GdkEventMotion* _evm)
{
            GtkTextTag          *   htag    =   NULL;
            gboolean                b       =   FALSE;
            GtkTextIter             iter;
    //  ........................................................................
    //printf("motion event\n");

    if ( ! html_callback() )
        goto lab_end;

    if ( ! view()->gtk_text_iter_from_window_coords( _evm->x, _evm->y, &iter ) )
    {
        goto lab_end;
    }

    htag    =   model()->text_tag_helper()->get_tag_htm();

    b = gtk_text_iter_has_tag( &iter, htag );

    if ( b )
    {
        if ( ! a_html_inside )
        {
            view()->set_html_cursor(TRUE);
            a_html_inside = TRUE;
        }
    }
    else
    {
        if ( a_html_inside )
        {
            view()->set_html_cursor(FALSE);
            a_html_inside = FALSE;
        }
    }

    //  ........................................................................
lab_end:
    return;
}
//  ############################################################################
//
//  GwrFileView
//
//  ############################################################################
GwrFileView::GwrFileView()
    :   GwrTextView(FALSE)
{
    d_file_url                      =   NULL;

    d_vbox                          =   gwrgtk_vbox_new(FALSE , 0 );
        d_hbox                      =   gwrgtk_hbox_new(FALSE , 0 );
            d_label_location        =   gtk_label_new( "Location : " );
            d_label_location_text   =   gtk_label_new( "?" );
            d_button_open_ext       =   gtk_button_new_with_label( "Open");

    gtk_box_set_homogeneous( GTK_BOX(d_hbox), FALSE );
    gtk_box_pack_start( GTK_BOX(d_hbox), d_label_location       , FALSE, FALSE, 0 );
    gtk_box_pack_start( GTK_BOX(d_hbox), d_label_location_text  , TRUE , TRUE , 0 );
    gtk_box_pack_start( GTK_BOX(d_hbox), d_button_open_ext      , FALSE, FALSE, 0 );

    gtk_box_set_homogeneous( GTK_BOX(d_vbox), FALSE );
    gtk_box_pack_start( GTK_BOX(d_vbox), d_hbox                         , FALSE, FALSE, 0 );
    gtk_box_pack_start( GTK_BOX(d_vbox), GwrTextView::widget()          , TRUE , TRUE , 0 );

    g_signal_connect(d_button_open_ext    , "button-press-event"  , G_CALLBACK(GwrFileView::Sgn_button_open_ext_press_event), this);

}
GwrFileView::GwrFileView(
    GwrFileView::Url    *   _url)
    :   GwrFileView()
{
    file_set( _url );
}
GwrFileView::~GwrFileView()
{
    delete d_file_url;

}

gboolean
GwrFileView::file_set(
    GwrFileView::Url    *   _url)
{
    gchar               sline   [64];
    gchar               temp    [4096];
    guint32             nline   =   0;
    FILE            *   f       =   NULL;
    //  ........................................................................
    g_return_val_if_fail( _url                  , FALSE );
    g_return_val_if_fail( _url->file_location() , FALSE );
    //  ........................................................................
    //  init
    GWR_IF_FALSE( f = fopen( _url->file_location(), "r" ), f )
        goto lab_exit_error;

    d_file_url   =   GWR_NEW_CAST( Url, _url->file_location(), _url->file_line() );

    //  ........................................................................
    //  go !
    GwrTextView::clear();

    while ( ! feof(f) )
    {
        fgets(temp, 4096 - 2, f);

        nline++;
        sprintf(sline,"%5i ", nline);

        GwrTextView::append( sline, 0 );
        GwrTextView::append( temp, 0 );
    }

    //  if given lineno, define the mark & scroll
    if ( url()->file_line() )
    {
        GwrTextView::view()->scroll_to_line( url()->file_line(), TRUE );        //  delay because gtk bug !
    }
    //  ........................................................................
lab_exit_success:
    fclose( f );
    return TRUE;
    //  ........................................................................
lab_exit_error:
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  View::Sgn_key_release_event()
//  ----------------------------------------------------------------------------
gboolean
GwrFileView::Sgn_button_open_ext_press_event(
    GtkWidget   *   _widget   ,
    GdkEvent    *   _event    ,
    gpointer        _data)
{
    //        GwrFileView         *   THIS    =   NULL;
    //        GdkEventButton      *   evb     =   NULL;
    //  ........................................................................
    printf("Open ext click\n");

    return LIBGWR_GTK_EVENT_KEY_PROPAGATE_YES;
}


GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)
