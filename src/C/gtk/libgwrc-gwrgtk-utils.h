/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-gwrgtk-utils.h                                                  *
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
    *   Purpose : GTK utils                                                     *
    *                                                                           *
    *****************************************************************************
*/


#ifndef __LIBGWRC_GWRGTK_UTILS_H__
#define __LIBGWRC_GWRGTK_UTILS_H__
//  ............................................................................
#include <gtk/gtk.h>
//  ............................................................................
#if ( __cplusplus )
extern "C" {
#endif

extern  void            gwrgtk_error_box(const gchar*, ...);


//! \brief  Wrapper : gtk_hbox_new() is deprecated from 3.2
extern  GtkWidget   *   gwrgtk_hbox_new    (gboolean _homogeneous, gint _spacing);
//! \brief  Wrapper : gtk_vbox_new() is deprecated from 3.2
extern  GtkWidget   *   gwrgtk_vbox_new    (gboolean _homogeneous, gint _spacing);

//! \brief  Wrapper : gtk_hpaned_new() is deprecated from 3.2
extern  GtkWidget   *   gwrgtk_hpaned_new  ();
//! \brief  Wrapper : gtk_vpaned_new() is deprecated from 3.2
extern  GtkWidget   *   gwrgtk_vpaned_new  ();
extern  void            gwrgtk_paned_set_space_repartition(GtkPaned* _paned, guint _first_child_percentage);

//! \brief  Wrapper : gtk_hseparator_new is deprecated from 3.0
extern  GtkWidget   *   gwrgtk_hseparator_new      ();

//! \brief  Wrapper : gtk_widget_modify_fg is deprecated from 3.0
extern  void            gwrgtk_modify_fg_color      (GtkWidget*, GtkStateType, guint16 _r, guint16 _g, guint16 _b);
//! \brief  Wrapper : gtk_widget_modify_bg is deprecated from 3.0
extern  void            gwrgtk_modify_bg_color      (GtkWidget*, GtkStateType, guint16 _r, guint16 _g, guint16 _b);

//! \brief  Wrapper : set font for a GtkButton's GtkLabel
extern  void            gwrgtk_button_set_label_font(GtkWidget* _button, const gchar* _fontname, PangoStyle, PangoWeight, guint32 _absolute_size_in_device_units);


#if ( __cplusplus )
}
#endif

#endif                                                                          //  __LIBGWRC_GWRGTK_UTILS_H__
