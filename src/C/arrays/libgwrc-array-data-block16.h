/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-data-block16.h                                            *
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

#ifndef     __LIBGWRC_ARRAY_DATA_BLOCK_16_H__
#define     __LIBGWRC_ARRAY_DATA_BLOCK_16_H__
//  ............................................................................
#include    "libgwrc-array-common.h"
//  ............................................................................
typedef struct  _GwrCADBlock16          GwrCADBlock16;

//! \struct _GwrCADBlock16
//!
//! \brief  Block of data that maintain the count of used bytes in it.
//!     16 bits version, 2^16 = 65536 available bits of data.
struct  _GwrCADBlock16
{
    gpointer    d_mem;                                                          //!< Base memory
    guint16     a_size;                                                         //!< Size ( in bytes )
    guint16     a_used_bytes;                                                   //!< Size ( in bytes ) of used bytes
}  __attribute__(( packed ));

extern  guint32     GwrCADBlock16_SSIZE;                                          //!< sizeof( GwrCADBlock16 )
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          GwrCADBlock16*  gwr_array_dbk16_new(
                    guint16                         _block_size         );

extern          void            gwr_array_dbk16_alloc(
                    GwrCADBlock16               *   _block              ,
                    guint16                         _block_size         );

extern          void            gwr_array_dbk16_dealloc(
                    GwrCADBlock16               *   _block              );

extern          void            gwr_array_dbk16_reset(
                    GwrCADBlock16               *   _block              );

extern          guint32         gwr_array_dbk16_available_bytes(
                    GwrCADBlock16               *   _block              );

extern          void            gwr_array_dbk16_add(
                    GwrCADBlock16               *   _block              ,
                    gpointer                        _data               ,
                    guint16                         _data_size          );

extern          void            gwr_array_dbk16_add_with_extra_data_index(
                    GwrCADBlock16               *   _block              ,
                    gpointer                        _data               ,
                    guint16                         _data_size          ,
                    guint32                         _extra_data_index   ,
                    guint8                      *   _xdi_len            );

extern          void            gwr_array_dbk16_dump(
                    GwrCADBlock16               *   _block              );

extern          void            gwr_array_dbk16_get_mfp(
                    GwrCADBlock16           *       _dbk16          ,
                    GwrCAMFP                *       _out            );

#if ( __cplusplus )
}
#endif


#endif                                                                          //  __LIBGWRC_ARRAY_DATA_BLOCK_16_H__
