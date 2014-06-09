/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-hash .h                                                          *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Hash functions                                                          *
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
    *   Class   :                                                               *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_HASH_H__
#define     __LIBGWR_HASH_H__
//  ...........................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(hash)

//  DJB Hash Function by Professor Daniel J. Bernstein
typedef guint32     tDJB;
extern  tDJB        DJB(const gchar* _str);





GWR_NAMESPACE_END(hash)
GWR_NAMESPACE_END(libgwr)

#endif                                                                          // __LIBGWR_HASH_H__
