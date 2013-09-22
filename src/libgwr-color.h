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

class Color
{
    friend class LibGwrStatic;

    private:

    public:
    //--------------------------------------------------------------------------
    private:
    gchar   *   d_name;
    gchar   *   d_html;

    public:
            Color(const gchar* _name, const gchar* _html)
            {
                d_name  = g_strdup(_name);
                d_html  = g_strdup(_html);
            }
    virtual ~Color()
    {
        g_free( d_name );
        g_free( d_html);
    }

    inline  const gchar *   name()  { return d_name; }
    inline  const gchar *   html()  { return d_html; }

};

//  ............................................................................
GWR_NAMESPACE_START(color)

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

extern  const gchar *   Name(guint32 _index);
extern  const gchar *   Html(guint32 _index);

//  ............................................................................
GWR_NAMESPACE_START(console)

extern  const   gchar   *       Fg[1+8];
extern  const   gchar   *       Bg[1+8];

GWR_NAMESPACE_END(console)
//  ............................................................................
inline  gchar   *           G_console_add_fg(gchar* p, guint32 _index)
{
    if ( _index <= 8 )
    {
        *(p++) = libgwr::color::console::Fg[_index][0];
        *(p++) = libgwr::color::console::Fg[_index][1];
        return p;
    }

    *(p++) = libgwr::color::console::Fg[libgwr::color::Std][0];
    *(p++) = libgwr::color::console::Fg[libgwr::color::Std][1];
    return p;
}
inline  gchar   *           G_console_add_bg(gchar* p, guint32 _index)
{
    if ( _index <= 8 )
    {
        *(p++) = libgwr::color::console::Bg[_index][0];
        *(p++) = libgwr::color::console::Bg[_index][1];
        return p;
    }

    *(p++) = libgwr::color::console::Bg[libgwr::color::Std][0];
    *(p++) = libgwr::color::console::Bg[libgwr::color::Std][1];
    return p;
}
GWR_NAMESPACE_END(color)
GWR_NAMESPACE_END(libgwr)

#endif
