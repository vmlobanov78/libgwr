/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-array-data-multi.h                                               *
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
    *   Purpose :   Array of data constitued of smaller arrays of data.         *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_ARRAY_DATA_MULTI_H__
#define     __LIBGWR_ARRAY_DATA_MULTI_H__
//  ............................................................................
#include    <glib.h>
//  ............................................................................
#include    "libgwr-array-equal.h"
//  ............................................................................
typedef struct  _GwrCADMData            GwrCADMData;

struct  _GwrCADMData
{
    gpointer    a_mem;                                                          //!< Data location
    guint16     a_size;                                                         //!< Data size
};
//  ............................................................................
typedef struct  _GwrCADMBlock           GwrCADMBlock;

struct  _GwrCADMBlock
{
    gpointer    d_mem;                                                          //!< Base memory       of the GwrCArrayDataMultiBlock
    guint32     a_size;                                                         //!< Size ( in bytes ) of the GwrCArrayDataMultiBlock
    guint32     a_used_bytes;                                                   //!< Size ( in bytes ) of used bytes in the GwrCArrayDataMultiBlock
};

extern  guint32     GwrCADMBlock_S;
//  ............................................................................
typedef struct  _GwrCADMDataInfo        GwrCADMDataInfo;

struct  _GwrCADMDataInfo
{
    guint16     a_block_index;                                                  //!< Index of GwrCADMultiBlock where data is stored
    guint32     a_offset;                                                       //!< Offset of data in the GwrCADMultiBlock
    guint16     a_len;                                                          //!< Len    of data in the GwrCADMultiBlock
};

extern  guint32     GwrCADMDataInfo_S;
//  ............................................................................
typedef struct  _GwrCArrayDataMulti     GwrCArrayDataMulti;

struct  _GwrCArrayDataMulti
{
    GwrCArrayEqual  *   d_blocks;                                               //!< Array of GwrCADMultiBlocks
    GwrCArrayEqual  *   d_infos;                                                //!< Array of GwrCADMDataInfos

    guint32             a_block_size;                                           //!< Size ( in bytes ) that GwrCADMultiBlock can cpntain
    guint32             a_data_size;                                            //!< Size ( in bytes ) of all the data contained in the GwrCADMultiBlocks
};
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          void                    gwr_array_data_multi_dump               (
            GwrCArrayDataMulti      *       _adm            );

extern          GwrCArrayDataMulti  *   gwr_array_data_multi_new                (
            guint32                         _block_size     );

extern          void                    gwr_array_data_multi_delete             (
            GwrCArrayDataMulti      *       _adm            );

extern          gboolean                gwr_array_data_multi_add_data           (
            GwrCArrayDataMulti      *       _adm            ,
            gpointer                        _data           ,
            guint16                         _len            );

extern          gboolean                gwr_array_data_multi_get_data           (
            GwrCArrayDataMulti      *       _adm            ,
            guint32                         _data_index     ,
            GwrCADMData             *       _data           );

#if ( __cplusplus )
}
#endif
//  ============================================================================

#endif
