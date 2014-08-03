/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-data-block24.h                                            *
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

#ifndef     __LIBGWRC_ARRAY_DATA_BLOCK_24_H__
#define     __LIBGWRC_ARRAY_DATA_BLOCK_24_H__
//  ............................................................................
#include    "libgwrc-array-common.h"
//  ............................................................................
typedef struct  _GwrCADBlock24          GwrCADBlock24;

//! \struct _GwrCADBlock24
//!
//! \brief  Block of data that maintain the count of used bytes in it.
//!     16 bits version, 2^24 = 16.777.216 available bits of data.
struct  _GwrCADBlock24
{
    gpointer    d_mem;                                                          //!< Base memory
    gchar       a_blob[6];                                                      //!< Size ( in bytes ) + Size ( in bytes ) of used bytes
}  __attribute__(( packed ));

extern  guint32     GwrCADBlock24_SSIZE;                                          //!< sizeof( GwrCADBlock24 )
//  ............................................................................
GwrCADBlock24*  gwr_array_dbk24_new(
                    guint32                         _block_size         );

void            gwr_array_dbk24_alloc(
                    GwrCADBlock24               *   _block              ,
                    guint32                         _block_size         );

void            gwr_array_dbk24_dealloc(
                    GwrCADBlock24               *   _block              );

void            gwr_array_dbk24_reset(
                    GwrCADBlock24               *   _block              );

guint32         gwr_array_dbk24_available_bytes(
                    GwrCADBlock24               *   _block              );

guint32         gwr_array_dbk24_used_bytes(
                    GwrCADBlock24               *   _block              );

void            gwr_array_dbk24_add(
                    GwrCADBlock24               *   _block              ,
                    gpointer                        _data               ,
                    guint32                         _data_size          );

void            gwr_array_dbk24_add_with_extra_data_index(
                    GwrCADBlock24               *   _block              ,
                    gpointer                        _data               ,
                    guint32                         _data_size          ,
                    guint32                         _extra_data_index   ,
                    guint8                      *   _xdi_len            );

void            gwr_array_dbk24_dump(
                    GwrCADBlock24               *   _block              );


#endif                                                                          //  __LIBGWRC_ARRAY_DATA_BLOCK_24_H__
