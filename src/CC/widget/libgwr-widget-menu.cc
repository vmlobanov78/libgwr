/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-menu.cc                                                   *
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
    *   Class   : Menu, MenuItem, MenuItemData                                  *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/


//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)

//  ############################################################################
//
//  MenuItemData
//
//  ############################################################################
//  ############################################################################
//
//  Menu
//
//  ############################################################################
Menu::Menu(
    gchar       *   _str_id         ,
    GCallback       _gcallback      ,
    gpointer        _udata          ,
    GtkMenuBar  *   _gtk_menu_bar   )
        : IMenu( _str_id )
{
    g_return_if_fail( _str_id       );
    g_return_if_fail( _gcallback    );
    //  ........................................................................
    set_gcallback   ( _gcallback    );
    set_udata       ( _udata        );

    d_gtk_menu          = NULL;
    d_gtk_menu_bar      = NULL;
    d_menu_item_list    = NULL;

    if ( _gtk_menu_bar )
        d_gtk_menu_bar  = _gtk_menu_bar;
    else
        d_gtk_menu      = GTK_MENU(gtk_menu_new());
}
Menu::~Menu()
{

}
//  ****************************************************************************
MenuItem*
Menu::menu_item_add(
    gchar   *   _label  ,
    gchar   *   _str_id )
{
    MenuItem    *   mit =   NULL;
    //..........................................................................
    g_return_val_if_fail( _label    , NULL);
    g_return_val_if_fail( _str_id   , NULL );
    //..........................................................................
    mit = GWR_NEW_CAST( MenuItem, _label, this, _str_id );

    d_menu_item_list = g_list_append(d_menu_item_list, (gpointer)mit);

    gtk_menu_shell_append( GTK_MENU_SHELL(widget()), mit->widget() );

    return mit;
}

CheckMenuItem*
Menu::check_menu_item_add(
    gchar       *   _label      ,
    gchar       *   _str_id     ,
    gboolean        _radio      )
{
    CheckMenuItem   *   cmi  = NULL;
    //..........................................................................
    g_return_val_if_fail( _label    , NULL);
    g_return_val_if_fail( _str_id   , NULL );
    //..........................................................................
    cmi = GWR_NEW_CAST( CheckMenuItem, _label, this, _str_id );

    d_menu_item_list = g_list_append(d_menu_item_list, (gpointer)cmi);

    gtk_menu_shell_append( GTK_MENU_SHELL(widget()), cmi->widget() );

    return cmi;
}


MenuItem*
Menu::menu_item_get(
    gchar   *   _str_id)
{
    GList   *   list    = NULL;
    //..........................................................................
    list = g_list_first(d_menu_item_list);

    while ( list )
    {
        if ( g_strcmp0( _str_id, ((MenuItem*)list->data)->str_id() ) == 0 )
        {
            return (MenuItem*)list->data;
        }
        list = g_list_next( list );
    }

    return NULL;
}


void    Menu::p0_menu_item_set_active (const gchar* _id_str, gboolean _b)
{
    MenuItem    *   mit =   NULL;
    GList       *   l   =   g_list_first( d_menu_item_list );
    //  ........................................................................
    while ( l )
    {
        mit =   (MenuItem*)( l->data );

        if ( ! g_strcmp0( mit->str_id(), _id_str ) )
        {
            gtk_widget_set_sensitive( mit->widget(), _b );
            return;
        }

        l   =   g_list_next(l);
    }
}
void    Menu::menu_item_activate      (const gchar* _id_str)
{
    p0_menu_item_set_active( _id_str, TRUE );
}
void    Menu::menu_item_desactivate   (const gchar* _id_str)
{
    p0_menu_item_set_active( _id_str, FALSE );
}
//  ############################################################################
//
//  MenuItem
//
//  ############################################################################
MenuItem::MenuItem(
    const   gchar   *   _label          ,
            Menu    *   _menu_parent    ,
    const   gchar   *   _str_id         )
        : IMenu( _str_id )
{
    g_return_if_fail( _label            );
    g_return_if_fail( _menu_parent      );
    //..........................................................................
    set_label   ( _label        );
    set_parent  ( _menu_parent  );
    set_child   ( NULL          );

    set_mdata   ( GWR_NEW_CAST( MenuItemCBData, str_id(), parent()->udata() ) );

    d_gtk_menu_item     =   GTK_MENU_ITEM( gtk_menu_item_new_with_label( _label ) );

    signal_connect();
}
MenuItem::~MenuItem()
{
}
//  ============================================================================
void
MenuItem::signal_connect()
{
    g_return_if_fail( a_signal_handle == 0 );
    //..........................................................................
    a_signal_handle     =   g_signal_connect(
                                d_gtk_menu_item             ,
                                "activate"                  ,
                                parent()->gcallback()       ,
                                mdata()                     );
}
void
MenuItem::signal_disconnect()
{
    g_return_if_fail( a_signal_handle != 0 );
    //..........................................................................
    g_signal_handler_disconnect((gpointer)widget(), a_signal_handle);
    a_signal_handle =   0;
}

Menu*
MenuItem::submenu_add(
    gchar       *   _id,
    GCallback       _gcallback,
    gpointer        _udata,
    GtkMenuBar  *)
{
    g_return_val_if_fail( _id,          NULL );
    g_return_val_if_fail( _gcallback,   NULL );
    //..........................................................................
    set_child( GWR_NEW_CAST( Menu, _id, _gcallback, _udata ) );

    gtk_menu_item_set_submenu(d_gtk_menu_item, child()->widget() );

    return child();
}

void
MenuItem::set_active(gboolean _b)
{
    gtk_widget_set_sensitive( widget(), _b );
}
//  ############################################################################
//
//  CheckMenuItem
//
//  ############################################################################
CheckMenuItem::CheckMenuItem(
    gchar       *   _label          ,
    Menu        *   _menu_parent    ,
    gchar       *   _str_id         ,
    gboolean        _radio          )
    :   MenuItem( _str_id )
{
    g_return_if_fail( _label            );
    g_return_if_fail( _menu_parent      );
    //..........................................................................
    set_label   ( _label        );
    set_parent  ( _menu_parent  );
    set_child   ( NULL          );

    set_mdata   ( GWR_NEW_CAST( MenuItemCBData, str_id(), parent()->udata() ) );

    d_gtk_menu_item     = GTK_MENU_ITEM( gtk_check_menu_item_new_with_label( _label ) );

    gtk_check_menu_item_set_draw_as_radio(
        GTK_CHECK_MENU_ITEM(d_gtk_menu_item)    ,
        _radio);

    gtk_check_menu_item_set_active(
        GTK_CHECK_MENU_ITEM(d_gtk_menu_item)    ,
        FALSE);

    a_checked = FALSE;

    signal_connect();
}
CheckMenuItem::~CheckMenuItem()
{
}

void
CheckMenuItem::check()
{
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget()), TRUE);
}
void
CheckMenuItem::uncheck()
{
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget()), FALSE);
}

GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)
