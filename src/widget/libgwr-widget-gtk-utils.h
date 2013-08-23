/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-gtk-utils.h                                               *
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


#ifndef __LIBGWR_WIDGET_GTK_UTILS_H__
#define __LIBGWR_WIDGET_GTK_UTILS_H__

//  ............................................................................
namespace libgwr
{
namespace widget
{

extern  void    gtk_paned_set_space_repartition(GtkPaned* _paned, guint _first_child_percentage);

extern  void    gtk_error_box(const gchar*, ...);

}       // namespace widget
}       // namespace libgwr

#endif //__LIBGWR_WIDGET_GTK_UTILS_H__
