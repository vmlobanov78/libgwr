/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-text-style.h                                                     *
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
    *   Nspace  : style                                                         *
    *                                                                           *
    *   Class   :                                                               *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_TEXT_STYLE_H__
#define     __LIBGWR_TEXT_STYLE_H__

//  ...........................................................................
namespace libgwr
{

namespace ts
{
/*
	Console text format : <ESC>[{attr};{fg};{bg}m

    * {attr} needs to be one of the following :
        0 Reset All Attributes (return to normal mode)
        1 Bright (usually turns on BOLD)
        2 Dim
        3 Underline
        5 Blink
        7 Reverse
        8 Hidden

    * {fg} needs to be one of the following :
        ...

    * {bg} needs to be one of the following:
        ...
*/

enum
{
    // console styles
    Std     =   0x000   ,
    Rst     =   0x001   ,
    Bld     =   0x002   ,
    Dim     =   0x004   ,
    Und     =   0x008   ,
    Blk     =   0x010   ,
    Rev     =   0x020   ,
    Hid     =   0x040   ,

    // GTK+ styles
    Ita     =   0x080   ,
    Stk     =   0x100   ,

    // Color specification
    Fg      =   0x200   ,
    Bg      =   0x400
};

inline gchar * g_add_console_styles(gchar* p, guint32 _style)
{
    if ( _style & (!Fg) & (!Bg) )
    {
        if ( _style & Rst ) {   *(p++) = '0'; *(p++) = ';'; };
        if ( _style & Bld ) {   *(p++) = '1'; *(p++) = ';'; };
        if ( _style & Dim ) {   *(p++) = '2'; *(p++) = ';'; };
        if ( _style & Und ) {   *(p++) = '3'; *(p++) = ';'; };
        if ( _style & Blk ) {   *(p++) = '5'; *(p++) = ';'; };
        if ( _style & Rev ) {   *(p++) = '7'; *(p++) = ';'; };
        if ( _style & Hid ) {   *(p++) = '9'; *(p++) = ';'; };
    }

    return p;
}

}   // namespace ts
}   // namespace libgwr

#endif  // __LIBGWR_TEXT_STYLE_H__
