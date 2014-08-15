/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-common.h                                                  *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
    *                                                                           *
    *   Copyright (C) 2011-2014 Guillaume Wardavoir                             *
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
    *   Purpose :   Array of constant-size data                                 *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWRC_ARRAY_COMMON_H__
#define     __LIBGWRC_ARRAY_COMMON_H__
//  ............................................................................
#include    <glib.h>

#include    <string.h>
#include    <stdio.h>
//  ............................................................................
typedef struct  _GwrCArrayMemoryFootPrint   GwrCAMFP;
//! \struct _GwrCArrayMemoryFootPrint
//!
//! \brief  Memory FootPrint for array structs.
struct  _GwrCArrayMemoryFootPrint
{
    guint32     a_ss;                                                           //!< Struct itself
    guint32     a_sa;                                                           //!< Internal allocated memory
    guint32     a_su;                                                           //!< Recursive allocated memory
};

typedef struct  _GwrCData16 GwrCData16;
//! \struct _GwrCData16
//!
//! \brief  Data, maximum 16 bits len.
struct  _GwrCData16
{
    gpointer    a_mem;                                                          //!< Address of data in memory
    guint16     a_size;                                                         //!< Size of data
};

#if ( __cplusplus )
extern "C" {
#endif

void    gwr_array_mfp_reset (GwrCAMFP*);
void    gwr_array_mfp_add   (GwrCAMFP* _mfp_dst, GwrCAMFP* _mfp_src);

#if ( __cplusplus )
}
#endif


#endif                                                                          //  __LIBGWRC_ARRAY_COMMON_H__
