/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-style.h                                                          *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Gnome Universal Logger Library                                          *
    *                                                                           *
    *   Copyright (C) 2011-2011 Guillaume Wardavoir                             *
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

#ifndef     __LIBGWR_COMMON_H__
#define     __LIBGWR_COMMON_H__
//  ...........................................................................
namespace libgwr
{

namespace style
{
    enum
    {
        Std     =   0x00    ,
        Nwl     =   0x01    ,
        Fg      =   0x02    ,
        Bg      =   0x04    ,
        Und     =   0x08    ,
        Bld     =   0x10    ,
        Ita     =   0x20    ,
        Stk     =   0x40
    };


        s = _c->subchannel(_subchannel_index);
        sprintf(s2, "\033[0;32mINF:\033[0m%s:%s", THIS->get_header(), s1);

	Prompt color format : <ESC>[{attr};{fg};{bg}m

	0 Reset All Attributes (return to normal mode)
	1 Bright (usually turns on BOLD)
	2 Dim
	3 Underline
	5 Blink
	7 Reverse
	8 Hidden


    const   gchar   *   str_empty   =   "";
    const   gchar   *   str_und     =   "";
    const   gchar   *   str_bld     =   "";
    const   gchar   *   str_Ita     =   "";
    const   gchar   *   str_Stk     =   "";

    inline const gchar  *   cstr(TxtAtt _att)
    {
        if ( _att.flags() == syle::Std )
            return "";
    }

}

}   // libgwr

#endif
