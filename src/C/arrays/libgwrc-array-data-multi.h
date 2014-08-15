/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-data-multi.h                                              *
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
    *   Purpose :   Array containing datas of any sizes.                        *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWRC_ARRAY_DATA_MULTI_H__
#define     __LIBGWRC_ARRAY_DATA_MULTI_H__
//  ............................................................................
#include    <glib.h>
//  ............................................................................
#include    "libgwrc-array-equal.h"
//  ............................................................................
typedef struct  _GwrCADMDataInfo        GwrCADMDataInfo;
//! \struct _GwrCADMDataInfo
//!
//! \brief  Info for accessing data in a GwrCArrayDataMulti.
struct  _GwrCADMDataInfo
{
    guint16     a_block_index;                                                  //!< Index of GwrCADMBlock where data is stored
    guint32     a_offset;                                                       //!< Offset of data in the GwrCADMBlock
    guint16     a_len;                                                          //!< Len    of data in the GwrCADMBlock
};

extern  guint32     GwrCADMDataInfo_SSIZE;                                      //!< Sizeof struct GwrCADMDataInfo
//  ............................................................................
typedef struct  _GwrCArrayDataMulti     GwrCArrayDataMulti;
//! \struct _GwrCArrayDataMulti
//!
//! \brief  Array containing datas of any sizes.
struct  _GwrCArrayDataMulti
{
    gchar                   *   d_name;

    GwrCArrayEqualSimple    *   d_data_blocks;                                  //!< Array of GwrCADMBlock24 structs
    GwrCArrayEqualSimple    *   d_info_blocks;                                  //!< Array of GwrCADMDataInfo structs

    guint32                     a_data_block_size;                              //!< Size ( in bytes ) of data GwrCADMBlock24
    guint32                     a_data_size;                                    //!< Size ( in bytes ) of all the datas contained in the GwrCADMBlocks
};
//  ............................................................................
typedef struct  _GwrCArrayDataMultiStat GwrCArrayDataMultiStat;
//! \struct _GwrCArrayDataMultiStat
//!
//! \brief  Convenience struct for memory statistics.
struct  _GwrCArrayDataMultiStat
{
    GwrCArrayEqualSimpleStat    a_data_stat;
    GwrCArrayEqualSimpleStat    a_info_stat;

    guint32                     a_data_block_size;

    GwrCAMFP                    a_mfp;
};
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          GwrCArrayDataMulti  *   gwr_array_data_multi_new                (
    const   gchar           *               _name                           ,
            guint32                         _data_block_size                ,
            guint32                         _data_blocks_storage_realloc    ,
            guint32                         _info_blocks_storage_realloc    );

extern          void                    gwr_array_data_multi_delete             (
            GwrCArrayDataMulti      *       _adm            );

extern          void                    gwr_array_data_multi_reset              (
            GwrCArrayDataMulti      *       _adm            );

extern          void                    gwr_array_data_multi_add_data           (
            GwrCArrayDataMulti      *       _adm            ,
            gpointer                        _data           ,
            guint16                         _data_len       );

extern          void                    gwr_array_data_multi_add_data_and_extra_data(
            GwrCArrayDataMulti      *       _adm                ,
            gpointer                        _data               ,
            guint16                         _len                ,
            gpointer                        _extra_data         ,
            guint8                          _extra_data_len     );

extern          void                    gwr_array_data_multi_get_data                   (
            GwrCArrayDataMulti      *       _adm            ,
            guint32                         _data_index     ,
            GwrCData16              *       _data           );

extern          void                    gwr_array_data_multi_dump_data          (
            GwrCArrayDataMulti      *       _adm            );

extern          void                    gwr_array_data_multi_get_stats          (
            GwrCArrayDataMulti      *       _adm                        ,
            GwrCArrayDataMultiStat  *       _adm_stat                   );

extern          void                    gwr_array_data_multi_get_mfp(
            GwrCArrayDataMulti      *       _aem            ,
            GwrCAMFP                *       _out            );

#if ( __cplusplus )
}
#endif
//  ============================================================================

#endif                                                                          //  __LIBGWRC_ARRAY_DATA_MULTI_H__
