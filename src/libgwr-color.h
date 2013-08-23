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
//  ...........................................................................
namespace libgwr
{

class   Color;

namespace color
{
/*
	Console text format : <ESC>[{attr};{fg};{bg}m

    * {attr} needs to be one of the following :
        ...

    * {fg} needs to be one of the following :
        30 Black
        31 Red
        32 Green
        33 Yellow
        34 Blue
        35 Magenta
        36 Cyan
        37 White

    * {bg} needs to be one of the following:
        40 Black
        41 Red
        42 Green
        43 Yellow
        44 Blue
        45 Magenta
        46 Cyan
        47 White
*/

enum
{
    Std     =       0   ,
    Blk     =       1   ,
    Red     =       2   ,
    Grn     =       3   ,
    Yel     =       4   ,
    Blu     =       5   ,
    Mag     =       6   ,
    Cya     =       7   ,
    Whi     =       8   ,
    Ora     =       9   ,
    Grn1    =       10  ,
    Greyd   =       11  ,
    Grey9   =       12  ,
    Greyc   =       13  ,
    Blu1    =       14  ,
    Turq1   =       15  ,
    Grey6   =       16
};

extern  const   gchar   *   g_console_colors_fg[LIBGWR_COLOR_DEFINED_CARD];
extern  const   gchar   *   g_console_colors_bg[LIBGWR_COLOR_DEFINED_CARD];

extern  Color       *   Colors[LIBGWR_COLOR_CARD_MAX];
extern  guint32         Card;

}   // namespace color

using namespace color;

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

namespace color
{

inline  const gchar *   Name(guint32 _index)
{
    g_return_val_if_fail( _index < Card, Colors[Red]->name() );
    return Colors[_index]->name();
}
inline  const gchar *   Html(guint32 _index)
{
    g_return_val_if_fail( _index < Card, Colors[Red]->html() );
    return Colors[_index]->html();
}
inline  gchar   *       g_add_console_fg_color(gchar* p, guint32 _index)
{
    g_return_val_if_fail( _index < LIBGWR_COLOR_DEFINED_CARD, p );
    *(p++) = g_console_colors_fg[_index][0];
    *(p++) = g_console_colors_fg[_index][1];
    *(p++) = ';';
    return p;
}
inline  gchar   *       g_add_console_bg_color(gchar* p, guint32 _index)
{
    g_return_val_if_fail( _index < LIBGWR_COLOR_DEFINED_CARD, p );
    *(p++) = g_console_colors_bg[_index][0];
    *(p++) = g_console_colors_bg[_index][1];
    *(p++) = ';';
    return p;
}

inline guint32 New(const gchar* _name, const gchar* _html)
{
    g_return_val_if_fail( Card < LIBGWR_COLOR_CARD_MAX, Red );

    Colors[Card] = GWR_NEW_CAST( Color, _name, _html );

    return Card++;
}

}   // namespace color



}   // libgwr

#endif
