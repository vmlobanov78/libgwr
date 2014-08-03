/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-data-block32.h                                            *
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
    *   Purpose :   Memory region containing data.                              *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWRC_ARRAY_DATA_BLOCK_32_H__
#define     __LIBGWRC_ARRAY_DATA_BLOCK_32_H__
//  ............................................................................
#include    "libgwrc-array-common.h"
//  ............................................................................
typedef struct  _GwrCADBlock32          GwrCADBlock32;

//! \struct _GwrCADBlock32
//!
//! \brief  Block of data that maintain the count of used bytes in it.
//!     16 bits version, 2^32 = ... available bits of data.
struct  _GwrCADBlock32
{
    gpointer    d_mem;                                                          //!< Base memory
    guint32     a_size;                                                         //!< Size ( in bytes )
    guint32     a_used_bytes;                                                   //!< Size ( in bytes ) of used bytes
}  __attribute__(( packed ));

extern  guint32     GwrCADBlock32_SSIZE;                                          //!< sizeof( GwrCADBlock16 )
//  ............................................................................
GwrCADBlock32*  gwr_array_dbk32_new(
                    guint32                         _block_size         );

void            gwr_array_dbk32_alloc(
                    GwrCADBlock32               *   _block              ,
                    guint32                         _block_size         );

void            gwr_array_dbk32_dealloc(
                    GwrCADBlock32               *   _block              );

void            gwr_array_dbk32_reset(
                    GwrCADBlock32               *   _block              );

inline guint32  gwr_array_dbk32_available_bytes(
                    GwrCADBlock32               *   _block              );

void            gwr_array_dbk32_add(
                    GwrCADBlock32               *   _block              ,
                    gpointer                        _data               ,
                    guint32                         _data_size          );

void            gwr_array_dbk32_add_with_extra_data_index(
                    GwrCADBlock32               *   _block              ,
                    gpointer                        _data               ,
                    guint32                         _data_size          ,
                    guint32                         _extra_data_index   ,
                    guint8                      *   _xdi_len            );

void            gwr_array_dbk32_dump(
                    GwrCADBlock32               *   _block              );


#endif                                                                          //  __LIBGWRC_ARRAY_DATA_BLOCK_32_H__
