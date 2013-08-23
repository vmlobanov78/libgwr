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
    *   Class   : Object                                                        *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_COMMON_H__
#define     __LIBGWR_COMMON_H__
//  ...........................................................................
#define     LIBGWR_USE_LOGGER   1
//  ...........................................................................
namespace libgwr
{

#define LIBGWR_COLOR_CARD_MAX           256
#define LIBGWR_COLOR_DEFINED_CARD       20

struct  guint32_x2
{
    guint32 u1;
    guint32 u2;

    inline  void    operator += (const guint32_x2& a)
    {
        u1 += a.u1;
        u2 += a.u2;
    }
};


}   // libgwr

#endif
