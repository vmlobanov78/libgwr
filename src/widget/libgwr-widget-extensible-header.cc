/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-extensible-header.h                                       *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libkconfig                                                      *
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
    *   Class   :                                                               *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *   Purpose : Widget for fine-controlling logs                              *
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
//  static
//
//  ############################################################################

//  ############################################################################
//
//  IExtensibleHeader
//
//  ############################################################################
IExtensibleHeader::IExtensibleHeader()
{
    d_slist_widget                  = NULL;
    d_bg_color_if_shown_orig        = NULL;
    d_bg_color_if_shown             = NULL;

}
IExtensibleHeader::~IExtensibleHeader()
{
    g_free_safe( d_bg_color_if_shown_orig   );
    g_free_safe( d_bg_color_if_shown        );

    g_slist_free( d_slist_widget );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
IExtensibleHeader::show()
{
    GSList      *   sl  = NULL;
    //  ........................................................................
    if ( ! d_bg_color_if_shown )
        return;

    sl = d_slist_widget ; while ( sl )
    {
        gtk_widget_modify_bg( (GtkWidget*)( sl->data ), GTK_STATE_NORMAL, d_bg_color_if_shown );

        sl = g_slist_next( sl );
    }
}
void
IExtensibleHeader::hide()
{
    GSList      *   sl  = NULL;
    //  ........................................................................
    if ( ! d_bg_color_if_shown )
        return;

    sl = d_slist_widget ; while ( sl )
    {
        gtk_widget_modify_bg( (GtkWidget*)( sl->data ), GTK_STATE_NORMAL, d_bg_color_if_shown_orig );

        sl = g_slist_next( sl );
    }
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
IExtensibleHeader::bg_color_if_shown_add_widget(GtkWidget* _w)
{
    d_slist_widget = g_slist_append( d_slist_widget, (gpointer)_w );
}
void
IExtensibleHeader::bg_color_if_shown_set(const gchar* _html_color )
{
    GtkRcStyle  *   rcs = NULL;
    GtkStyle    *   st  = NULL;
    GtkWidget   *   wf  = NULL;
    //  ........................................................................
    g_return_if_fail( d_slist_widget );
    wf = (GtkWidget*)( d_slist_widget->data );

    //  free colors ( if successive call to this method )
    g_free_safe( d_bg_color_if_shown_orig   );
    g_free_safe( d_bg_color_if_shown        );

    //  memorize current bg color
    rcs                         = gtk_widget_get_modifier_style ( wf );
    st                          = gtk_widget_get_style          ( wf );
    d_bg_color_if_shown_orig    = GWR_NEW_CAST( GdkColor );
    *d_bg_color_if_shown_orig   = st->bg[GTK_STATE_PRELIGHT];

    //  create new bg color
    d_bg_color_if_shown         = GWR_NEW_CAST( GdkColor );
    gdk_color_parse( _html_color, d_bg_color_if_shown );
}
//  ############################################################################
//
//  ExtensibleHeader
//
//  ############################################################################
ExtensibleHeader::ExtensibleHeader(
    const gchar     *   _name)
    : IExtensibleHeader()
{
    a_child                         = NULL;
    //  ........................................................................
    d_vbox                          = libgwr::gtk::vbox_new(FALSE, 0);
        d_hbox                      = libgwr::gtk::hbox_new(FALSE, 0);
            d_evbox1                = gtk_event_box_new();
                d_label             = gtk_label_new(_name);
            d_evbox2                = gtk_event_box_new();
                d_dummy_label       = gtk_label_new("");
            d_button                = gtk_button_new_with_label("?");

    gtk_widget_set_size_request( d_button    , 20, 15 );                        // reduce minimal size of button

    gtk_container_add   ( GTK_CONTAINER(d_evbox1), d_label          );
    gtk_container_add   ( GTK_CONTAINER(d_evbox2), d_dummy_label    );

    gtk_box_pack_start  ( GTK_BOX(d_hbox), d_evbox1     , FALSE, FALSE, 0 );
    gtk_box_pack_start  ( GTK_BOX(d_hbox), d_evbox2     , TRUE , TRUE , 0 );
    gtk_box_pack_start  ( GTK_BOX(d_hbox), d_button     , FALSE, FALSE, 0 );

    gtk_box_pack_start  ( GTK_BOX(d_vbox), d_hbox   ,   FALSE, TRUE , 0 );

    gtk_widget_show_all( d_vbox );                                              // show everything

    IExtensibleHeader::bg_color_if_shown_add_widget( d_evbox1 );
    IExtensibleHeader::bg_color_if_shown_add_widget( d_evbox2 );
    //  ........................................................................
    //  simulate shown state, then hide
    a_shown = TRUE;
    // now hide
    hide();

    g_signal_connect( d_button, "clicked", (GCallback)libgwr::widget::ExtensibleHeader::Sgn_button_clicked, (gpointer)this );
}
ExtensibleHeader::~ExtensibleHeader()
{
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean    ExtensibleHeader::pack(GtkWidget * _child)
{
    g_return_val_if_fail( ! a_child, FALSE );

    a_child = _child;

    if ( a_shown )
    {
        gtk_box_pack_start( GTK_BOX(d_vbox), a_child, TRUE, TRUE, 0 );
        gtk_widget_show( a_child );
    }
    else
    {
        g_object_ref( a_child );
    }

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        ExtensibleHeader::show()
{
    g_return_if_fail( ! a_shown );

    if ( a_child )
    {
        IExtensibleHeader::show();

        gtk_box_pack_start( GTK_BOX(d_vbox), a_child, TRUE, TRUE, 0 );
        g_object_unref( a_child );
        gtk_widget_show_all( d_vbox );
    }

    gtk_button_set_label( GTK_BUTTON(d_button), "~" );

    a_shown = TRUE;
}
void        ExtensibleHeader::hide()
{
    g_return_if_fail( a_shown );

    if ( a_child )
    {
        IExtensibleHeader::hide();

        g_object_ref( a_child );
        gtk_container_remove( GTK_CONTAINER(d_vbox), a_child );
    }

    gtk_button_set_label( GTK_BUTTON(d_button), "+" );

    a_shown = FALSE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        ExtensibleHeader::Sgn_button_clicked(GtkButton * _button, gpointer _data)
{
    ExtensibleHeader    *   THIS    = NULL;
    //  ........................................................................
    THIS = static_cast< ExtensibleHeader * > ( _data );

    if ( THIS->a_shown )
        THIS->hide();
    else
        THIS->show();
}
//  ############################################################################
//
//  ExtensibleHeaderF
//
//  ############################################################################
ExtensibleHeaderF::ExtensibleHeaderF(
    const gchar     *   _name)
    : IExtensibleHeader()
{
    a_child                         = NULL;
    //  ........................................................................
    d_vbox                      = libgwr::gtk::vbox_new(FALSE,0);
        d_frame                 = gtk_frame_new( "" );
            d_hbox              = libgwr::gtk::hbox_new(FALSE,3);
                d_button        = gtk_button_new_with_label("?");
                d_label         = gtk_label_new( _name);
            a_child             = NULL;

    gtk_container_set_border_width( GTK_CONTAINER(d_frame), 5);
    gtk_widget_set_size_request( d_button    , 20, 15 );                        // reduce minimal size of button

    gtk_box_pack_start  ( GTK_BOX(d_hbox), d_button     , FALSE, FALSE, 0 );
    gtk_box_pack_start  ( GTK_BOX(d_hbox), d_label      , FALSE, FALSE, 0 );

    IExtensibleHeader::bg_color_if_shown_add_widget( d_button );
    //  ........................................................................
    //  simulate shown state, then hide
    //  setup frame : set hbox as label
    gtk_frame_set_label_widget( GTK_FRAME(d_frame), d_hbox    );
    //  add frame to vbox
    gtk_box_pack_start( GTK_BOX(d_vbox), d_frame , TRUE, TRUE, 0);
    //  show / hide rest
    gtk_widget_show_all( d_vbox );
    a_shown = TRUE;
    // now hide
    hide();

    g_signal_connect( d_button, "clicked", (GCallback)libgwr::widget::ExtensibleHeaderF::Sgn_button_clicked, (gpointer)this );
}
ExtensibleHeaderF::~ExtensibleHeaderF()
{
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean    ExtensibleHeaderF::pack(GtkWidget * _child)
{
    g_return_val_if_fail( ! a_child, FALSE );

    a_child = _child;

    gtk_container_add( GTK_CONTAINER(d_frame), a_child   );

    gtk_widget_show_all( d_frame );

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        ExtensibleHeaderF::show()
{
    g_return_if_fail( ! a_shown );

    gtk_button_set_label( GTK_BUTTON(d_button), "~" );

    //  remove hbox from vbox
    g_object_ref( d_hbox    );
    gtk_container_remove( GTK_CONTAINER(d_vbox), d_hbox );

    //  setup frame : set hbox as label
    gtk_frame_set_label_widget( GTK_FRAME(d_frame), d_hbox    );
    g_object_unref( d_hbox );

    //  add frame to vbox
    gtk_box_pack_start( GTK_BOX(d_vbox), d_frame , TRUE, TRUE, 0);
    g_object_unref( d_frame   );

    gtk_widget_show_all( d_frame );
    a_shown = TRUE;
}
void        ExtensibleHeaderF::hide()
{
    g_return_if_fail( a_shown );

    gtk_button_set_label( GTK_BUTTON(d_button), "+" );

    //  frame : remove hbox
    g_object_ref( d_hbox );
    gtk_frame_set_label_widget( GTK_FRAME(d_frame), NULL    );

    //  remove frame from vbox
    g_object_ref( d_frame    );
    gtk_container_remove( GTK_CONTAINER(d_vbox), d_frame );

    //  add hbox to vbox
    gtk_box_pack_start( GTK_BOX(d_vbox), d_hbox , TRUE, TRUE, 0);
    g_object_unref( d_hbox   );

    a_shown = FALSE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void        ExtensibleHeaderF::Sgn_button_clicked(GtkButton * _button, gpointer _data)
{
    ExtensibleHeaderF   *   THIS    = NULL;
    //  ........................................................................
    THIS = static_cast< ExtensibleHeaderF * > ( _data );

    if ( THIS->a_shown )
        THIS->hide();
    else
        THIS->show();
}


GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)
