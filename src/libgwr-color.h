/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-common.h                                                         *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
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
    *   Nspace  : color                                                         *
    *                                                                           *
    *   Class   : color::Color                                                  *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_COLOR_H__
#define     __LIBGWR_COLOR_H__
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(color)

class Color
{
    friend class LibGwrStatic;

private:

public:
    //--------------------------------------------------------------------------
private:
    gchar   *   d_name;
    gchar   *   d_html;
    gchar   *   d_console_fg;
    gchar   *   d_console_bg;

public:
    void        set_html(const gchar* _html)
    {
        g_free_safe( d_html );
        d_html = g_strdup( _html );
    }


public:
    Color(const gchar* _name, const gchar* _html, const gchar* _console_fg, const gchar* _console_bg)
    {
        d_name          = g_strdup( _name       );
        d_html          = g_strdup( _html       );
        d_console_fg    = g_strdup( _console_fg );
        d_console_bg    = g_strdup( _console_bg );
    }
    virtual ~Color()
    {
        g_free( d_name );
        g_free( d_html);
    }

    inline  const gchar *   name()
    {
        return d_name;
    }
    inline  const gchar *   html()
    {
        return d_html;
    }
    inline  const gchar *   cfg()
    {
        return d_console_fg;
    }
    inline  const gchar *   cbg()
    {
        return d_console_bg;
    }

};

enum
{
    Std     =        0  ,

    Blk     =        1  ,                                                       //  -+
    Red     =        2  ,                                                       //   |
    Grn     =        3  ,                                                       //   |
    Yel     =        4  ,                                                       //   |--> terminal colors
    Blu     =        5  ,                                                       //   |
    Mag     =        6  ,                                                       //   |
    Cya     =        7  ,                                                       //   |
    Whi     =        8  ,                                                       //  -+

    Ora     =        9  ,
    Grn1    =       10  ,
    Greyd   =       11  ,
    Grey9   =       12  ,
    Greyc   =       13  ,
    Blu1    =       14  ,
    Turq1   =       15  ,
    Grey6   =       16
};

extern      Color       *   Colors[LIBGWR_COLOR_CARD_MAX];
//  ............................................................................
inline  gchar   *           Console_add_fg(gchar* p, guint32 _index)
{
    g_return_val_if_fail( _index < LIBGWR_COLOR_DEFINED_CARD , NULL );

    *(p++) = libgwr::color::Colors[_index]->cfg()[0];
    *(p++) = libgwr::color::Colors[_index]->cfg()[1];
    return p;
}
inline  gchar   *           Console_add_bg(gchar* p, guint32 _index)
{
    g_return_val_if_fail( _index < LIBGWR_COLOR_DEFINED_CARD , NULL );

    *(p++) = libgwr::color::Colors[_index]->cbg()[0];
    *(p++) = libgwr::color::Colors[_index]->cbg()[1];
    return p;
}

GWR_NAMESPACE_END(color)
GWR_NAMESPACE_END(libgwr)

#endif
