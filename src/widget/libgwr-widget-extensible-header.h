/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-extensible-header.h                                       *
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
    *   Class   : ExtensibleHeader                                              *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_WIDGET_EXTENSIBLE_HEADER_H__
#define     __LIBGWR_WIDGET_EXTENSIBLE_HEADER_H__

//  ............................................................................
#include    "../libgwr.h"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)

/// ****************************************************************************
//!
//! \namespace  libgwr::widget
//! \class      IExtensibleHeader
//!
//! \brief      Interface for extensible headers
//!
/// ****************************************************************************
class   IExtensibleHeader
{
    public:
    virtual     GtkWidget       *   widget()                                = 0;
    virtual     void                show()                                  = 0;
    virtual     void                hide()                                  = 0;
    virtual     gboolean            pack(GtkWidget*)                        = 0;

    //  ------------------------------------------------------------------------
    //  header bg color modification
    protected:
    GSList                      *   d_slist_widget;
    GdkColor                    *   d_bg_color_if_shown_orig;
    GdkColor                    *   d_bg_color_if_shown;

    protected:
                void                bg_color_if_shown_add_widget(GtkWidget*);
    public:
                void                bg_color_if_shown_set(const gchar* _html_color);
    //  ------------------------------------------------------------------------
    //  ()~()
    protected:
             IExtensibleHeader();
    virtual ~IExtensibleHeader();

};
/// ****************************************************************************
//!
//! \namespace  libgwr::widget
//! \class      ExtensibleHeader
//!
//! \brief      Header that can show / hide an unique child widget vertically
//!
/// ****************************************************************************
class   ExtensibleHeader    : public IExtensibleHeader
{
    private:
    GtkWidget               *           d_vbox;
    GtkWidget               *               d_hbox;
    GtkWidget               *                   d_evbox1;                       //!< For changing label's background color
    GtkWidget               *                       d_label;                    //!< Header title
    GtkWidget               *                   d_evbox2;                       //!< For changing label's background color
    GtkWidget               *                       d_dummy_label;              //!< Horizontal separator for good positionning
    GtkWidget               *                   d_button;                       //!< Show / Hide button
    GtkWidget               *               a_child;                            //!< Child to show / hide

    gboolean                            a_shown;

    private:
    static  void                Sgn_button_clicked(GtkButton*, gpointer);

    public:
    virtual GtkWidget       *   widget()    { return d_vbox; }
    virtual void                show();
    virtual void                hide();
    virtual gboolean            pack(GtkWidget*);

    public:
             ExtensibleHeader(const gchar* _name);
    virtual ~ExtensibleHeader();


};
/// ****************************************************************************
//!
//! \namespace  libgwr::widget
//! \class      ExtensibleHeaderF
//!
//! \brief      Header that can show / hide an unique child widget vertically
//!
/// ****************************************************************************
class   ExtensibleHeaderF   : public IExtensibleHeader
{
    private:
    GtkWidget               *           d_vbox;
    GtkWidget               *               d_frame;
    GtkWidget               *                   d_hbox;
    GtkWidget               *                       d_button;                   //!< Show / Hide button
    GtkWidget               *                       d_label;                    //!< Header title
    GtkWidget               *                   a_child;                        //!< Child to show / hide

    gboolean                            a_shown;

    gchar                   *           d_name;

    private:
    static  void                Sgn_button_clicked(GtkButton*, gpointer);

    public:
    virtual GtkWidget       *   widget()    { return d_vbox; }
    virtual void                show();
    virtual void                hide();
    virtual gboolean            pack(GtkWidget*);

    public:
             ExtensibleHeaderF(const gchar* _name);
    virtual ~ExtensibleHeaderF();
};







GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)

#endif                                                                          // #define __LIBGWR_WIDGET_EXTENSIBLE_HEADER_H__
