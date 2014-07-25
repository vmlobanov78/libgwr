/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-gtk-matrix.hh                                             *
    *                                                                           *
    *   --------------------------------------------------------------------    *
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
    *   Class   : GwrTextView                                                   *
    *                                                                           *
    *   Purpose : Mix between GtkGrid & GtkNone                                 *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_WIDGET_GTK_MATRIX_HH__
#define     __LIBGWR_WIDGET_GTK_MATRIX_HH__

//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)
GWR_NAMESPACE_START(gtk)

//! \brief  Wrapper : GtkTable / GtkGrid wether we use Gtk2 or Gtk3
class   Matrix
{
    LIBGWR_MEMBER_AUTO_TYPE ( guint32    , nrow );
    LIBGWR_MEMBER_AUTO_TYPE ( guint32    , ncol );
    LIBGWR_MEMBER_DYNAMIC   ( GtkWidget  , widget );
    //  ........................................................................
    public:
    void            attach     (GtkWidget* _child, guint32 _left, guint32 _top, GtkAttachOptions _xopt, GtkAttachOptions _yopt, guint _xpadding, guint _ypadding);

             Matrix(guint32 _n_rows, guint32 _n_columns, gboolean _homogeneous);
    virtual ~Matrix();
};


GWR_NAMESPACE_END(gtk)
GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)

#endif                                                                          // #define __LIBGWR_WIDGET_GTK_MATRIX_HH__
