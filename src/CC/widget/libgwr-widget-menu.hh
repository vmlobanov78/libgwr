/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-menu.h                                                    *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Helper for GtkMenu widgets                                              *
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
    *   Class   : Menu                                                          *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/


#ifndef __LIBGWR_WIDGET_MENU_H__
#define __LIBGWR_WIDGET_MENU_H__

//  ............................................................................
namespace libgwr
{
namespace widget
{

class   IMenuShell;
class   Menu;
class   MenuItem;
class   CheckMenuItem;
/// ****************************************************************************
//!
//! \class MenuItemCBData
//!
//! \brief Convenience class for passing infos on "activated" signal callback
//!
//! It contains
//!   - an utf8 id string, for identifying the widget in the callback
//!   - a GCallback
//!   - a classical gpointer, the users data
//!
/// ****************************************************************************
class MenuItemCBData
{
    LIBGWR_MEMBER_HSTRING   ( str_id            );                              //!< HString id of the MenuItem
    LIBGWR_MEMBER_AUTO_TYPE ( gpointer  , udata );                              //!< User data given to the MenuItem's Parent
    //  ------------------------------------------------------------------------
    //  ()~()
    //  ------------------------------------------------------------------------
    public:
    MenuItemCBData(const gchar* _str_id, gpointer _udata)
    {
        g_return_if_fail( _str_id  );

        set_str_id      ( _str_id       );
        set_udata       ( _udata        );
    };
    virtual ~MenuItemCBData()
    {
    }
};
/// ****************************************************************************
//!
//! \class IMenu
//!
//! \brief Base class for putting some things in common
//!
/// ****************************************************************************
class   IMenu
{
    LIBGWR_MEMBER_HSTRING   ( str_id );                                         //!< HString id
    //  ------------------------------------------------------------------------
    //  ()~()
    //  ------------------------------------------------------------------------
    public:
            IMenu(const gchar* _str_id)
            {
                set_str_id( _str_id );
            }
    virtual ~IMenu()    {}
};
/// ****************************************************************************
//!
//! \class Menu
//!
//! \brief Convenience class for building GTK+ menus
//!
/// ****************************************************************************
class Menu  : public IMenu
{
    LIBGWR_MEMBER_AUTO_TYPE ( GCallback , gcallback );                          //!< GCallback for MenuItems children
    LIBGWR_MEMBER_AUTO_TYPE ( gpointer  , udata     );                          //!< User data for MenuItems children

    private:
    GtkMenu         *   d_gtk_menu;                                             //!< The GtkMenu...
    GtkMenuBar      *   d_gtk_menu_bar;                                         //!< ...or the GtkMenuBar

    GList           *   d_menu_item_list;                                       //!< List of MenuItems
    //  ------------------------------------------------------------------------
    //  ()~()
    //  ------------------------------------------------------------------------
    public:
             Menu(gchar* _str_id, GCallback, gpointer, GtkMenuBar* = NULL);     //!< If GtkMenuBar is NULL, a GtkMenu is created
    virtual ~Menu();

    public:
    inline  GtkWidget   *   widget()
                            {
                                return d_gtk_menu           ?
                                    GTK_WIDGET(d_gtk_menu)  :
                                    GTK_WIDGET(d_gtk_menu_bar);
                            }

    MenuItem        *   menu_item_add       (gchar* _label              ,       //!< Add a MenuItem
                                             gchar* _callback_string    );
    MenuItem        *   menu_item_add       (...);                              //!< Add many MenuItems in  one function call
    MenuItem        *   menu_item_get       (gchar* id);                        //!< Retrive a MenuItem

    CheckMenuItem   *   check_menu_item_add (gchar*     _label              ,   //!< Add a CheckMenuItem
                                             gchar*     _str_id             ,
                                             gboolean   _radio = FALSE      );

    //  ------------------------------------------------------------------------
    private:
    void                p0_menu_item_set_active (const gchar* _id_str, gboolean);

    public:
    void                menu_item_activate      (const gchar* _id_str);
    void                menu_item_desactivate   (const gchar* _id_str);
};

/// ****************************************************************************
//!
//! \class MenuItem
//!
//! \brief Convenience class for building GTK+ menus
//!
//! Retrieving a menu / menu item is slow, because we parse a GNode-based tree ;
//! so don't abuse of get_menu and get_menu_item methods.
//!
/// ****************************************************************************
class MenuItem  : public IMenu
{
    friend class    Menu;
    friend class    libgwr::Object<MenuItem>;

    LIBGWR_MEMBER_STRING    ( label                         );                  //!< Title in the GUI
    LIBGWR_MEMBER_AUTO_TYPE ( Menu*     , parent            );                  //!< Parent Menu of this MenuItem
    LIBGWR_MEMBER_AUTO_TYPE ( Menu*     , child             );                  //!< Child Menu of this MenuItem

    LIBGWR_MEMBER_AUTO_TYPE ( gulong            , signal_handle     );          //!< Gtk+ handle for the signal
    LIBGWR_MEMBER_DYNAMIC   ( MenuItemCBData    , mdata             );          //!< gpointer passed in callback

protected:
    GtkMenuItem     *   d_gtk_menu_item;                                        //!< The GtkMenuItem
    //  ------------------------------------------------------------------------
    //  ()~()
    //  ------------------------------------------------------------------------
    public:
    MenuItem(
        const   gchar   *   _label          ,                                   //!< MenuItem label
                Menu    *   _menu_parent    ,
        const   gchar   *   _str_id         );

    protected:
    MenuItem(const gchar* _str_id) : IMenu(_str_id)  {};                        //!< Especially for CheckMenuItem

    public:
    virtual ~MenuItem();

    public:
    inline  GtkWidget   *   widget()    { return GTK_WIDGET(d_gtk_menu_item);   }

    protected:
    void    signal_connect();
    void    signal_disconnect();

    public:
    void    set_active(gboolean);

    public:
    Menu    *   submenu_add(gchar* _id, GCallback, gpointer, GtkMenuBar* =NULL);//!< Add the sub Menu ( and disconnect MenuItem signal )
};

/// ****************************************************************************
//!
//! \class CheckMenuItem
//!
//! \brief Convenience class for building GTK+ menus
//!
/// ****************************************************************************
class CheckMenuItem : public MenuItem
{
    friend class    Menu;
    friend class    libgwr::Object<CheckMenuItem>;

    //  ------------------------------------------------------------------------
    //  ()~()
    //  ------------------------------------------------------------------------
    private:
    CheckMenuItem(
        gchar       *   _label          ,                                       //!< MenuItem label
        Menu        *   _parent         ,                                       //!< Parent Menu
        gchar       *   _str_id         ,                                       //!< Signal : id string
        gboolean        _radio = FALSE  );                                      //!< Wether the check box will be a radio button
    public:
    virtual ~CheckMenuItem();
    //  ------------------------------------------------------------------------
    //  check
    //  ------------------------------------------------------------------------
    private:
    gboolean            a_checked;
    public:
    void                check();
    void                uncheck();

    public:
    //virtual     Menu    *   submenu_add(gchar* _id, GCallback, gpointer, GtkMenuBar* =NULL);//!< This method fail for this class
    //virtual     gboolean    submenu_del();                                                  //!< This method fail for this class
};


}       // namespace widget
}       // namespace libgwr

#endif //__LIBGWR_WIDGET_MENU_H__
