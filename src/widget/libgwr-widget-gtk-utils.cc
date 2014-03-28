/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-gtk-utils.cc                                              *
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


//  ----------------------------------------------------------------------------
//  ----------------------------------------------------------------------------
static  gchar   p_va_buf[4096];

void    gtk_error_box(const gchar* format, ...)
{
    GtkWidget   *   dialog  = NULL;
    //  ........................................................................
    va_list arglist;
    va_start(arglist, format);
        vsprintf(p_va_buf, format,arglist);
    va_end(arglist);

    dialog = gtk_message_dialog_new(
        NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_CLOSE,
        p_va_buf);

    gtk_window_set_title( GTK_WINDOW(dialog), "gkconfig : error");
    /*
    gtk_message_dialog_set_image(
        GTK_MESSAGE_DIALOG(dialog),
        gtk_image_new_from_stock( GTK_STOCK_DIALOG_ERROR, GTK_ICON_SIZE_DIALOG) );
    */

    gtk_dialog_run(GTK_DIALOG (dialog));
    gtk_widget_destroy(dialog);
}

}       // namespace widget


namespace   gtk
{

//! \fn     vbox_new()
//!
//! \brief  Wrapper for deprecated gtk_vbox_new()
GtkWidget   *   vbox_new(gboolean _homogeneous, gint _spacing)
{
    GtkWidget   *   w   = NULL;
    //  ........................................................................
    w   =   gtk_box_new( GTK_ORIENTATION_VERTICAL, _spacing );
    gtk_box_set_homogeneous( GTK_BOX(w), _homogeneous );
    return w;
}
//! \fn     hbox_new()
//!
//! \brief  Wrapper for deprecated gtk_hbox_new()
GtkWidget   *   hbox_new(gboolean _homogeneous, gint _spacing)
{
    GtkWidget   *   w   = NULL;
    //  ........................................................................
    w   =   gtk_box_new( GTK_ORIENTATION_HORIZONTAL, _spacing );
    gtk_box_set_homogeneous( GTK_BOX(w), _homogeneous );
    return w;
}
//! \fn     vpaned_new()
//!
//! \brief  Wrapper for deprecated gtk_vpaned_new()
GtkWidget   *   vpaned_new()
{
    GtkWidget   *   w   = NULL;
    //  ........................................................................
    w   =   gtk_paned_new( GTK_ORIENTATION_VERTICAL );
    return w;
}
//! \fn     hpaned_new()
//!
//! \brief  Wrapper for deprecated gtk_hpaned_new()
GtkWidget   *   hpaned_new()
{
    GtkWidget   *   w   = NULL;
    //  ........................................................................
    w   =   gtk_paned_new( GTK_ORIENTATION_HORIZONTAL );
    return w;
}

void        paned_set_space_repartition(
    GtkPaned    *   _paned,
    guint           _first_child_percentage)
{
    GList           *   l   =   NULL;
    GtkWidget       *   w1  =   NULL;
    GtkWidget       *   w2  =   NULL;

    GtkOrientation      o;
    GtkAllocation       al, al1, al2;
    gint                *p, *p1, *p2;
    gint                s, s1, s2;
    //..........................................................................
    g_return_if_fail( _first_child_percentage >= 0      );
    g_return_if_fail( _first_child_percentage <= 100    );

    //  get infos
    l   =   gtk_container_get_children(GTK_CONTAINER(_paned));                  //  widgets
    w1  =   GTK_WIDGET(l->data);
    w2  =   GTK_WIDGET(l->next->data);

    gtk_widget_get_allocation(w1, &al1);                                        //  allocations
    gtk_widget_get_allocation(w2, &al2);
    gtk_widget_get_allocation(GTK_WIDGET(_paned), &al);

    //  from gtk 3.2, GtkHPaned & GtkVPaned are deprecated, and GTK_IS_HPANED
    //  is broken.
    if ( ( gtk_get_major_version() >= 3 ) && ( gtk_get_minor_version() >= 2 ) )
    {
        g_object_get( G_OBJECT(_paned), "orientation", &o, NULL );
    }
    //  before Gtk 3.2, GTK_IS_HPANED works well
    else
    {
        o   =   GTK_IS_HPANED(_paned)           ?
                    GTK_ORIENTATION_HORIZONTAL  :
                    GTK_ORIENTATION_VERTICAL    ;
    }

    if ( o == GTK_ORIENTATION_HORIZONTAL )                                      //  pointers on width or height, dependending on _paned is hpaned or vpaned
    {
        p   =   &( al.width);
        p1  =   &(al1.width);
        p2  =   &(al2.width);
    }
    else
    {
        p   =   &( al.height);
        p1  =   &(al1.height);
        p2  =   &(al2.height);
    }

    s1  = *p1;
    s2  = *p2;
    s   = s1 + s2;

    s1 =   s * _first_child_percentage / 100;

    gtk_paned_set_position(_paned, s1);
}


}   // namespace gtk




}       // namespace libgwr
