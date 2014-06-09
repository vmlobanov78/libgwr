/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc.c                                                               *
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
    *   Purpose   : RGB colors facilities                                       *
    *                                                                           *
    *****************************************************************************
*/


//  ****************************************************************************
//
//  Colors
//
//  ****************************************************************************
#include    "C/libgwrc-color.h"
//  ............................................................................
libgwrC_Color   libgwrC_Colors[ LIBGWR_COLOR_CARD_MAX ];
//  ****************************************************************************
//
//  string hash
//
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  gwr_str_hash
//  ----------------------------------------------------------------------------
//! \name   gwr_str_hash
//!
//! \brief  DJB Hash Function by Professor Daniel J. Bernstein
GwrCStrHash gwr_str_hash(const gchar* _str)
{
    guint32     hash    = 5381;
    gchar   *   pc      = (gchar*)_str;

    while ( *pc != 0 )
    {
        hash = ( (hash << 5 ) + hash ) + (guint32)( *pc++ );
    }

    return hash;
}
