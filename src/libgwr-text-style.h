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

//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(text)
//  ............................................................................


//      01234567 012345637 01234567 012345637
//      fg color  bg color <----  style ---->

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



GWR_NAMESPACE_END(text)
GWR_NAMESPACE_END(libgwr)
//  ............................................................................

#endif  // __LIBGWR_TEXT_STYLE_H__
