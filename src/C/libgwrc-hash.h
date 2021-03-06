/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-hash .h                                                         *
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
    *   Purpose : String hashes                                                 *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWRC_HASH_H__
#define     __LIBGWRC_HASH_H__
//  ...........................................................................
#include    "C/libgwrc-common.h"
//  ...........................................................................
typedef guint32     GwrCStrHash;

#if ( __cplusplus )
extern "C" {
#endif

extern  GwrCStrHash gwr_str_hash(const gchar* _str);

#if ( __cplusplus )
}
#endif


#endif                                                                          //  __LIBGWRC_HASH_H__
