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
void    gtk_paned_set_space_repartition(
    GtkPaned    *   _paned,
    guint           _first_child_percentage)
{
    GList           *   l   =   NULL;
    GtkWidget       *   w1  =   NULL;
    GtkWidget       *   w2  =   NULL;

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
    gtk_widget_get_allocation(w1, &al2);
    gtk_widget_get_allocation(GTK_WIDGET(_paned), &al);

    if ( GTK_IS_HPANED(_paned) )                                                //  pointers on width or height, dependending on _paned is hpaned or vpaned
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



    /*
    s   = *p1 + *p2;                                                            //  total amount of pixels available for children

    //  compute new allocations
    *p1 =   s * _first_child_percentage / 100;
    *p2 =   s - *p1;

    //  re-allocate, we act like we were the GtkPaned
    gtk_widget_size_allocate(w1, &al1);
    gtk_widget_size_allocate(w2, &al2);
    */
}

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
}       // namespace libgwr
