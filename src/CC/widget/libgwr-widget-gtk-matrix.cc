/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-gtk-matrix.cc                                             *
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
    *   Purpose : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)
GWR_NAMESPACE_START(gtk)

Matrix::Matrix(guint32 _n_rows, guint32 _n_columns, gboolean _homogeneous)
{
    a_nrow  =   _n_rows;
    a_ncol  =   _n_columns;
    //  ........................................................................
    #if GWR_GTK_VERSION_GE(3,4)

        d_widget    =   gtk_grid_new();
        gtk_grid_set_column_homogeneous( GTK_GRID(d_widget), FALSE );

    #else

        d_widget    =    gtk_table_new( _n_rows, _n_columns, _homogeneous );

    #endif
}
Matrix::~Matrix()
{
}
void
Matrix::attach(GtkWidget* _child, guint32 _left, guint32 _top, GtkAttachOptions _xopt, GtkAttachOptions _yopt, guint _xpadding, guint _ypadding)
{
    #if GWR_GTK_VERSION_GE(3,4)

        GtkWidget   *   w       =   _child;
        GtkWidget   *   hbox    =   NULL;
        GtkWidget   *   vbox    =   NULL;

        if ( ( _xopt == GTK_SHRINK ) && ( _yopt != GTK_SHRINK ) )
        {
            gtk_widget_set_hexpand( _child, FALSE );
            hbox = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, FALSE );
            gtk_box_pack_start( GTK_BOX(hbox), _child, FALSE, FALSE, 0 );
            w   =   hbox;
        }

        if ( ( _xopt != GTK_SHRINK ) && ( _yopt == GTK_SHRINK ) )
        {
            gtk_widget_set_vexpand( _child, FALSE );
            vbox = gtk_box_new( GTK_ORIENTATION_VERTICAL, FALSE );
            gtk_box_pack_start( GTK_BOX(vbox), _child, FALSE, FALSE, 0 );
            w   =   vbox;
        }

        if ( ( _xopt == GTK_SHRINK ) && ( _yopt == GTK_SHRINK ) )
        {
            gtk_widget_set_hexpand( _child, FALSE );
            gtk_widget_set_vexpand( _child, FALSE );

            hbox = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, FALSE );
            vbox = gtk_box_new( GTK_ORIENTATION_VERTICAL, FALSE );

            gtk_box_pack_start( GTK_BOX(hbox), _child, FALSE, FALSE, 0 );
            gtk_box_pack_start( GTK_BOX(vbox), hbox, FALSE, FALSE, 0 );

            w   =   vbox;
        }

        gtk_grid_attach( GTK_GRID(widget()), w, _left, _top, 1, 1 );

    #else

        gtk_table_attach( GTK_TABLE(widget()), _child, (guint)_left, (guint)_left + 1, (guint)_top, (guint)_top + 1, _xopt, _yopt, _xpadding, _ypadding );

    #endif
}


GWR_NAMESPACE_END(gtk)
GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)
