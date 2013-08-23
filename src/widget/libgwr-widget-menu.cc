/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-menu.cc                                                   *
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
    *   Class   : Menu, MenuItem, MenuItemData                                  *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/


//  ............................................................................
#include    "../libgwr.h"
//  ............................................................................
namespace libgwr
{
namespace widget
{

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
    gchar       *   _id,
    GCallback       _gcallback,
    gpointer        _udata,
    GtkMenuBar  *   _gtk_menu_bar)
{
    g_return_if_fail( _id );
    g_return_if_fail( _gcallback );

    d_id                = NULL;
    d_gtk_menu          = NULL;
    d_gtk_menu_bar      = NULL;
    d_menu_item_list    = NULL;

    d_id                = g_strdup( _id );

    a_callback          = _gcallback;
    a_udata             = _udata;

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
    gchar   *   _label,
    gchar   *   _callback_string)
{
    MenuItem    *   mi  = NULL;
    //..........................................................................
    g_return_val_if_fail( _label            , NULL);
    g_return_val_if_fail( _callback_string  , NULL );
    //..........................................................................

    mi = GWR_NEW_CAST( MenuItem, _label, gcallback(), _callback_string, udata(), this);

    d_menu_item_list = g_list_append(d_menu_item_list, (gpointer)mi);

    gtk_menu_shell_append( GTK_MENU_SHELL(widget()), mi->widget() );

    return mi;
}
gboolean
Menu::menu_item_del(
    gchar* _label)
{
    return FALSE;
}

MenuItem*
Menu::menu_item_get(
    gchar   *   id)
{
    GList   *   list    = NULL;
    //..........................................................................
    list = g_list_first(d_menu_item_list);

    while ( list )
    {
        if ( g_strcmp0( id, ((MenuItem*)list->data)->id() ) == 0 )
        {
            return (MenuItem*)list->data;
        }
        list = g_list_next( list );
    }

    return NULL;
}

CheckMenuItem*
Menu::check_menu_item_add(
    gchar       *   _label,
    gchar       *   _callback_string,
    gboolean        _radio)
{
    CheckMenuItem   *   cmi  = NULL;
    //..........................................................................
    g_return_val_if_fail( _label            , NULL);
    g_return_val_if_fail( _callback_string  , NULL );
    //..........................................................................

    cmi = GWR_NEW_CAST( CheckMenuItem, _label, gcallback(), _callback_string, udata(), this);

    d_menu_item_list = g_list_append(d_menu_item_list, (gpointer)cmi);

    gtk_menu_shell_append( GTK_MENU_SHELL(widget()), cmi->widget() );

    return cmi;
}

//  ############################################################################
//
//  MenuItem
//
//  ############################################################################
MenuItem::MenuItem(
    gchar       *   _label,
    GCallback       _gcallback,
    gchar       *   _callback_string,
    gpointer        _udata,
    Menu        *   _menu_parent)
{
    g_return_if_fail( _label );
    g_return_if_fail( _gcallback );
    g_return_if_fail( _callback_string );
    g_return_if_fail( _menu_parent );
    //..........................................................................
    d_utf8_label        = g_strdup( _label );

    d_gtk_menu_item     = GTK_MENU_ITEM( gtk_menu_item_new_with_label( _label ) );

    d_menu_item_data    = GWR_NEW( MenuItemData, _callback_string, _udata);

    a_menu_parent       = _menu_parent;
    d_submenu           = NULL;

    a_signal_handle     = g_signal_connect(
                            d_gtk_menu_item,
                            "activate",
                            _gcallback,
                            (gpointer)d_menu_item_data);
}
MenuItem::~MenuItem()
{
    g_free( d_utf8_label );
    delete d_menu_item_data;
}

gboolean
MenuItem::disconnect_signal()
{
    g_return_val_if_fail( a_signal_handle, FALSE );
    //..........................................................................
    g_signal_handler_disconnect((gpointer)widget(), a_signal_handle);

    return TRUE;
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

    // this item will now have a submenu, so disconnect the "activate" signal
    disconnect_signal();

    d_submenu = GWR_NEW( Menu, _id, _gcallback, _udata );

    gtk_menu_item_set_submenu(d_gtk_menu_item, d_submenu->widget() );

    return d_submenu;
}

gboolean
MenuItem::submenu_del()
{
    return TRUE;
}

//  ############################################################################
//
//  CheckMenuItem
//
//  ############################################################################
CheckMenuItem::CheckMenuItem(
    gchar       *   _label,
    GCallback       _gcallback,
    gchar       *   _callback_string,
    gpointer        _udata,
    Menu        *   _menu_parent,
    gboolean        _radio)
{
    g_return_if_fail( _label );
    g_return_if_fail( _gcallback );
    g_return_if_fail( _callback_string );
    g_return_if_fail( _menu_parent );
    //..........................................................................
    d_utf8_label        = g_strdup( _label );

    d_gtk_menu_item     = GTK_MENU_ITEM( gtk_check_menu_item_new_with_label( _label ) );
    gtk_check_menu_item_set_draw_as_radio(
        GTK_CHECK_MENU_ITEM(d_gtk_menu_item)    ,
        _radio);
    gtk_check_menu_item_set_active(
        GTK_CHECK_MENU_ITEM(d_gtk_menu_item)    ,
        FALSE);
    a_checked = FALSE;

    d_menu_item_data    = GWR_NEW( MenuItemData, _callback_string, _udata);

    a_menu_parent       = _menu_parent;
    d_submenu           = NULL;

    a_signal_handle     = g_signal_connect(
                            d_gtk_menu_item,
                            "activate",
                            _gcallback,
                            (gpointer)d_menu_item_data);
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

Menu*
CheckMenuItem::submenu_add(
    gchar       *   _id,
    GCallback       _gcallback,
    gpointer        _udata,
    GtkMenuBar  *)
{
    return NULL;
}

gboolean
CheckMenuItem::submenu_del()
{
    return FALSE;
}


}       // namespace widget
}       // namespace libgwr
