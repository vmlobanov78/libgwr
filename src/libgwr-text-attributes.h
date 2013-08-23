/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-text-attributes.h                                                *
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
    *   Nspace  : txt                                                           *
    *                                                                           *
    *   Class   :                                                               *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_TEXT_ATTRIBUTES_H__
#define     __LIBGWR_TEXT_ATTRIBUTES_H__
//  ...........................................................................
namespace libgwr
{
/*!
 *  \brief  Quickly modify text attributes with this struct
 *
 *  01234567 012345637 01234567 012345637
 *  fg color  bg color <----  style ---->
 *
 */
struct TextAttributes
{
    private:
    guint32 a_att;

    public:
    TextAttributes(guint32 _style = ts::Std, guint32 _fg_color = color::Std, guint32 _bg_color = color::Std)
    {
        g_return_if_fail( _fg_color < 256 );
        g_return_if_fail( _bg_color < 256 );

        a_att = _style + ( _fg_color << 24 ) + ( _bg_color << 16 );
    }
    TextAttributes(guint32 _flags)
    {
        //guint32 _fg_color   =   ( _flags & 0xFF000000 ) >> 24;
        //guint32 _bg_color   =   ( _flags & 0x00FF0000 ) >> 16;

        //g_return_if_fail( _fg_color < 256 );
        //g_return_if_fail( _bg_color < 256 );

        a_att = _flags;
    }
    virtual ~TextAttributes()    {}

    inline  guint32 flags() { return a_att;                             }
    inline  guint32 style() { return a_att & 0x0000FFFF;                }
    inline  guint32 fgcol() { return ( a_att & 0xFF000000 ) >> 24;      }
    inline  guint32 bgcol() { return ( a_att & 0x00FF0000 ) >> 16;      }

    inline  operator    guint32()  { return a_att; }

};
typedef struct TextAttributes TA;


namespace ta
{

extern  gchar   g_console_attributes[32];

inline const gchar * g_add_console_attributes(gchar* p, TextAttributes _ta)
{
    if ( ! _ta.flags() )
        return p;

    // [1] From here we know that something will be added

    // Escape sequence
    *(p++) = 27     ;
    *(p++) = '['    ;

    // Styles
    p = ts::g_add_console_styles(p, _ta.style());

    // Colors
    if ( _ta.flags() & ts::Fg )
        p = g_add_console_fg_color(p, _ta.fgcol());
    if ( _ta.flags() & ts::Bg )
        p = g_add_console_bg_color(p, _ta.fgcol());

    // remove the trailing ';' ( cf [1] ), and put 'm' instead
    *( -- p) = 'm';

    return p;
}

inline  const gchar *   get_console_attributes(TextAttributes _ta)
{
    g_add_console_attributes(g_console_attributes, _ta);
    return g_console_attributes;
}



}   // namespace ta
}   // namespace libgwr

#endif  // __LIBGWR_TEXT_ATTRIBUTES_H__
