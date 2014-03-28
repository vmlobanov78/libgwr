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
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
//  ............................................................................
GWR_NAMESPACE_START(text)
GWR_NAMESPACE_START(attributes)

//      01234567 012345637 01234567 012345637
//      fg color  bg color <----  style ---->

enum
{
    // attribute : styles ( console )
    Std     =   0x00000000  ,
    Rst     =   0x00000001  ,
    Bld     =   0x00000002  ,
    Dim     =   0x00000004  ,
    Und     =   0x00000008  ,
    Blk     =   0x00000010  ,
    Rev     =   0x00000020  ,
    Hid     =   0x00000040  ,

    // attributes : styles ( GTK+ )
    Ita     =   0x00000080  ,
    Stk     =   0x00000100  ,
    Htm     =   0x00000200  ,

    //  attributes : colors
    FgColor =   0x00008000  ,
    BgColor =   0x00004000
};

GWR_NAMESPACE_END(attributes)
GWR_NAMESPACE_END(text)
//  ............................................................................


//! \class   TextFmt
//!
//! \brief  Quickly modify text attributes with this class
//!
//!     01234567 012345637 01234567 012345637
//!     fg color  bg color <----  style ---->
//!
//!
class   TextAttributes
{
private:
    guint32 a_att;

public:
    TextAttributes(
        guint32     _style      =   libgwr::text::attributes::Std   ,
        guint32     _fg_color   =   libgwr::color::Std              ,
        guint32     _bg_color   =   libgwr::color::Std              )
    {
        g_return_if_fail( _fg_color < 256 );
        g_return_if_fail( _bg_color < 256 );

        a_att = _style + ( _fg_color << 24 ) + ( _bg_color << 16 );
    }
    TextAttributes(guint32 _flags)
    {
        a_att = _flags;
    }
    virtual ~TextAttributes()    {}

    inline  guint32 flags()
    {
        return a_att;
    }
    inline  guint32 style()
    {
        return a_att & 0x0000FFFF;
    }
    inline  guint32 fgcol()
    {
        return ( a_att & 0xFF000000 ) >> 24;
    }
    inline  guint32 bgcol()
    {
        return ( a_att & 0x00FF0000 ) >> 16;
    }

    //inline  operator    guint32()  { return a_att; }
};
//  ............................................................................
GWR_NAMESPACE_START(text)



inline gchar * Console_add_style(gchar* p, guint32 _style)
{
    if ( _style & libgwr::text::attributes::Rst )
    {
        *(p++) = '0';
        return p;
    };
    if ( _style & libgwr::text::attributes::Bld )
    {
        *(p++) = '1';
        return p;
    };
    if ( _style & libgwr::text::attributes::Dim )
    {
        *(p++) = '2';
        return p;
    };
    if ( _style & libgwr::text::attributes::Und )
    {
        *(p++) = '3';
        return p;
    };
    if ( _style & libgwr::text::attributes::Blk )
    {
        *(p++) = '5';
        return p;
    };
    if ( _style & libgwr::text::attributes::Rev )
    {
        *(p++) = '7';
        return p;
    };
    if ( _style & libgwr::text::attributes::Hid )
    {
        *(p++) = '9';
        return p;
    };

    *(p++) = '0';
    return p;
}


//! \fn     libgwr::text::attributes::console::G_add()
//!
//! \brief  Console text format : <ESC>[{attr};{fg};{bg}m
//:
inline const gchar *
G_console_add_attributes(
    gchar                   *   _p  ,
    libgwr::TextAttributes      _ta )
{
    // Escape sequence
    *(_p++) = 27     ;
    *(_p++) = '['    ;

    // Styles
    _p = libgwr::text::Console_add_style      ( _p, _ta.style()   );
    *(_p++) = ';'    ;

    // Colors
    _p = libgwr::color::Console_add_fg        ( _p, _ta.fgcol()   );
    *(_p++) = ';'    ;

    _p = libgwr::color::Console_add_bg        ( _p, _ta.bgcol()   );
    *(_p++) = 'm'    ;

    return _p;
}

GWR_NAMESPACE_END(text)
GWR_NAMESPACE_END(libgwr)
//  ............................................................................

#endif  // __LIBGWR_TEXT_ATTRIBUTES_H__
