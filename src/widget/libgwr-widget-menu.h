/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-menu.h                                                    *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Helper for GtkMenu widgets                                              *
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

/**
  * ============================================================================
  *
  * \class MenuActivatedData
  *
  * \brief Convenience class for passing infos on "activated" signal callback
  *
  * It contains
  * - an utf8 id string, for identifying the widget in the callback
  * - a GCallback
  * - a classical gpointer, the users data
  *
  * ============================================================================
  */
class MenuItemData
{
    private:
    const gchar *   d_utf8_id;                                                  //!< Callback id string
    gpointer        a_data;                                                     //!< User's data

    public:
    MenuItemData(const gchar* _utf8_id, gpointer _data)
    {
        g_return_if_fail( _utf8_id  );

        d_utf8_id   =   g_strdup(_utf8_id);
        a_data      =   _data;
    };
    virtual ~MenuItemData()
    {
        g_free((gpointer)d_utf8_id);
    }

    public:
    inline  const gchar *   id()    { return d_utf8_id; }
    inline  gpointer        data()  { return a_data;    }
};

class   Menu;
class   MenuItem;
class   CheckMenuItem;

/**
  * ============================================================================
  *
  * \class Menu
  *
  * \brief Convenience class for building GTK+ menus
  *
  * ============================================================================
  */
class Menu
{
    private:
    GtkMenu         *   d_gtk_menu;                                             //!< The GtkMenu...
    GtkMenuBar      *   d_gtk_menu_bar;                                         //!< ...or the GtkMenuBar
    gchar           *   d_id;                                                   //!< Menu id
    GCallback           a_callback;                                             //!< Common GCallback for all MenuItems
    gpointer            a_udata;                                                //!< Common user data for all MenuItems

    GList           *   d_menu_item_list;                                       //!< List of MenuItems
    //..........................................................................
    public:
             Menu(gchar* _id, GCallback, gpointer, GtkMenuBar* = NULL);         //!< If GtkMenuBar is NULL, a GtkMenu is created
    virtual ~Menu();

    public:
    inline  GtkWidget   *   widget()
                            {
                                return d_gtk_menu           ?
                                    GTK_WIDGET(d_gtk_menu)  :
                                    GTK_WIDGET(d_gtk_menu_bar);
                            }
    inline  GCallback       gcallback() { return a_callback;                }
    inline  gpointer        udata()     { return a_udata;                   }

    MenuItem        *   menu_item_add(gchar* _label, gchar* _callback_string);      //!< Add a MenuItem
    MenuItem        *   menu_item_add(...);                                         //!< Add many MenuItems in  one function call
    gboolean            menu_item_del(gchar* _label);
    MenuItem        *   menu_item_get(gchar* id);

    CheckMenuItem   *   check_menu_item_add(gchar* _label, gchar* _callback_string, gboolean _radio = FALSE);   //!< Add a CheckMenuItem
};

/**
  * ============================================================================
  *
  * \class MenuItem
  *
  * \brief Convenience class for building GTK+ menus
  *
  * Retrieving a menu / menu item is slow, because we parse a GNode-based tree ;
  * so don't abuse of get_menu and get_menu_item methods.
  *
  * ============================================================================
  */
class MenuItem
{
    friend class    Menu;
    friend class    libgwr::Object<MenuItem>;

protected:
    gchar           *   d_utf8_label;                                           //!< MenuItem label ( used as id )
    GtkMenuItem     *   d_gtk_menu_item;                                        //!< The GtkMenuItem

    MenuItemData    *   d_menu_item_data;                                       //!< GCallback data
    gulong              a_signal_handle;                                        //!< Gtk+ handle for the signal

    Menu            *   a_menu_parent;                                          //!< Parent Menu of this MenuItem
    Menu            *   d_submenu;                                              //!< Sub-Menu of this MenuItem
    //..........................................................................
    private:
    MenuItem(
        gchar       *   _label,                                                 //!< MenuItem label
        GCallback       _gcallback,                                             //!< Signal : GCallback
        gchar       *   _callback_string,                                       //!< Signal : id string
        gpointer        _udata,                                                 //!< Signal : user data
        Menu        *   _menu_parent);                                          //!< Parent Menu
    protected:
    MenuItem() {}
    public:
    virtual ~MenuItem();

    public:
    inline  GtkWidget   *   widget()    { return GTK_WIDGET(d_gtk_menu_item);   }
    inline  gchar       *   label()     { return d_utf8_label;                  }
    inline  gchar       *   id()        { return label();                       }

    private:
    gboolean    disconnect_signal();

    public:
    Menu    *   submenu_add(gchar* _id, GCallback, gpointer, GtkMenuBar* =NULL);//!< Add the sub Menu ( and disconnect MenuItem signal )
    gboolean    submenu_del();                                                  //!< Del the sub Menu ( recusrive process )
};

/**
  * ============================================================================
  *
  * \class CheckMenuItem
  *
  * \brief Convenience class for building GTK+ menus
  *
  * ============================================================================
  */
class CheckMenuItem : public MenuItem
{
    friend class    Menu;
    friend class    libgwr::Object<CheckMenuItem>;

    //  ........................................................................
    //  check
    private:
    gboolean            a_checked;
    public:
    void                check();
    void                uncheck();
    //..........................................................................
    private:
    CheckMenuItem(
        gchar       *   _label,                                                 //!< MenuItem label
        GCallback       _gcallback,                                             //!< Signal : GCallback
        gchar       *   _callback_string,                                       //!< Signal : id string
        gpointer        _udata,                                                 //!< Signal : user data
        Menu        *   _menu_parent,                                           //!< Parent Menu
        gboolean        _radio = FALSE);                                        //!< Wether the check box will be a radio button
    public:
    virtual ~CheckMenuItem();

    private:
    gboolean    disconnect_signal();

    public:
    virtual     Menu    *   submenu_add(gchar* _id, GCallback, gpointer, GtkMenuBar* =NULL);//!< This method fail for this class
    virtual     gboolean    submenu_del();                                                  //!< This method fail for this class
};

}       // namespace widget
}       // namespace libgwr

#endif //__LIBGWR_WIDGET_MENU_H__
