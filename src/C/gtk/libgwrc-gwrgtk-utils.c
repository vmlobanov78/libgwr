/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-gwrgtk-utils.c                                                   *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libgwr                                                          *
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


//  ............................................................................
#include    "C/libgwrc-macro.h"
#include    "libgwrc-gwrgtk-utils.h"
//  ............................................................................
static  gchar   p_va_buf[4096];

void    gwrgtk_error_box(const gchar* format, ...)
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

GtkWidget   *   gwrgtk_hbox_new(gboolean _homogeneous, gint _spacing)
{
    GtkWidget   *   w   = NULL;
    //  ........................................................................
    #if GWR_GTK_VERSION_GE(3,2)
    w   =   gtk_box_new( GTK_ORIENTATION_HORIZONTAL, _spacing );
    gtk_box_set_homogeneous( GTK_BOX(w), _homogeneous );
    #else
        w   =   gtk_hbox_new( _homogeneous, _spacing );
    #endif
    return w;
}
GtkWidget   *   gwrgtk_vbox_new(gboolean _homogeneous, gint _spacing)
{
    GtkWidget   *   w   = NULL;
    //  ........................................................................
    #if GWR_GTK_VERSION_GE(3,2)
        w   =   gtk_box_new( GTK_ORIENTATION_VERTICAL, _spacing );
        gtk_box_set_homogeneous( GTK_BOX(w), _homogeneous );
    #else
        w   =   gtk_vbox_new( _homogeneous, _spacing );
    #endif
    return w;
}

GtkWidget   *   gwrgtk_hpaned_new()
{
    GtkWidget   *   w   = NULL;
    //  ........................................................................
    #if GWR_GTK_VERSION_GE(3,2)
        w   =   gtk_paned_new( GTK_ORIENTATION_HORIZONTAL );
    #else
        w   =   gtk_hpaned_new();
    #endif
    return w;
}
GtkWidget   *   gwrgtk_vpaned_new()
{
    GtkWidget   *   w   = NULL;
    //  ........................................................................
    #if GWR_GTK_VERSION_GE(3,2)
        w   =   gtk_paned_new( GTK_ORIENTATION_VERTICAL );
    #else
        w   =   gtk_vpaned_new();
    #endif
    return w;
}

void        gwrgtk_paned_set_space_repartition(
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

    //  Orientation : from gtk 3.2, GtkHPaned & GtkVPaned are deprecated,
    //  and GTK_IS_HPANED is broken.
    #if GWR_GTK_VERSION_GE(3,2)
        g_object_get( G_OBJECT(_paned), "orientation", &o, NULL );
    #else
    {
        o   =   GTK_IS_HPANED(_paned)           ?
                    GTK_ORIENTATION_HORIZONTAL  :
                    GTK_ORIENTATION_VERTICAL    ;
    }
    #endif

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


GtkWidget*  gwrgtk_hseparator_new()
{
    //  GTK >= 3.0
    #if GWR_GTK_VERSION_GE(3,0)
    {
        return  gtk_separator_new( GTK_ORIENTATION_HORIZONTAL );
    }
    #else
    {
        return  gtk_hseparator_new();
    }
    #endif
}


//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void            gwrgtk_modify_fg_color   (GtkWidget* _w, GtkStateType _st, guint16 _r, guint16 _g, guint16 _b)
{
    //  GTK >= 3.0
    #if GWR_GTK_VERSION_GE(3,0)
    {
        GtkStateFlags   sf;
        GdkRGBA         color;

        sf      = (GtkStateFlags)
        (
            - ( _st == GTK_STATE_NORMAL ) * GTK_STATE_FLAG_NORMAL
        );
    /*
        - ( GTK_STATE_ACTIVE,
        - ( GTK_STATE_PRELIGHT,
        - ( GTK_STATE_SELECTED,
        - ( GTK_STATE_INSENSITIVE,
        - ( GTK_STATE_INCONSISTENT,
        - ( GTK_STATE_FOCUSED

 GTK_STATE_FLAG_NORMAL       = 0,
  GTK_STATE_FLAG_ACTIVE       = 1 << 0,
  GTK_STATE_FLAG_PRELIGHT     = 1 << 1,
  GTK_STATE_FLAG_SELECTED     = 1 << 2,
  GTK_STATE_FLAG_INSENSITIVE  = 1 << 3,
  GTK_STATE_FLAG_INCONSISTENT = 1 << 4,
  GTK_STATE_FLAG_FOCUSED      = 1 << 5,
  GTK_STATE_FLAG_BACKDROP     = 1 << 6
    */

        sf          =   (GtkStateFlags)0xff;

        color.red   =   ( (gdouble)_r / (gdouble)65535. );
        color.green =   ( (gdouble)_g / (gdouble)65535. );
        color.blue  =   ( (gdouble)_b / (gdouble)65535. );
        color.alpha =   (gdouble)1.;

        gtk_widget_override_color(_w, sf, &color);
    }
    #else
    //  GTK < 3.0
    {
        GdkColor    color;

        color.pixel =   0;
        color.red   =   _r;
        color.green =   _g;
        color.blue  =   _b;

        gtk_widget_modify_fg( _w, _st, &color );
    }
    #endif
}
void            gwrgtk_modify_bg_color   (GtkWidget* _w, GtkStateType _st, guint16 _r, guint16 _g, guint16 _b)
{
}

void            gwrgtk_button_set_label_font( GtkWidget* _button, const gchar* _fontname, PangoStyle _style, PangoWeight _weight, guint32 _absolute_size_in_device_units)
{
    GtkWidget               *   l   =   gtk_bin_get_child( GTK_BIN(_button) );

    PangoAttrList           *   pal =   pango_attr_list_new();

    PangoFontDescription    *   pfd =   pango_font_description_new();
    pango_font_description_set_family       (pfd, _fontname );
    pango_font_description_set_style        (pfd, _style );
    pango_font_description_set_weight       (pfd, _weight );
    pango_font_description_set_absolute_size(pfd, _absolute_size_in_device_units * PANGO_SCALE);

    PangoAttribute          *   pat =   pango_attr_font_desc_new( pfd );
    pango_attr_list_insert( pal, pat );

    gtk_label_set_attributes( GTK_LABEL(l), pal );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

