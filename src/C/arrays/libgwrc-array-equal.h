/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-equal.h                                                   *
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

#ifndef     __LIBGWRC_ARRAY_EQUAL_H__
#define     __LIBGWRC_ARRAY_EQUAL_H__
//  ............................................................................
#include    "libgwrc-array-data-block24.h"
#include    "libgwrc-array-equal-simple.h"
//  ............................................................................
typedef struct  _GwrCArrayEqual     GwrCArrayEqual;

//! \struct _GwrCArrayEqual
//!
//! \brief  Array of constant size datas.
struct  _GwrCArrayEqual
{
    GwrCArrayEqualSimple        *   d_blocks;                                   //!< Array of GwrCADBlock24 structs

    gchar                       *   d_name;

    guint32                         a_data_size;                                //!< Size ( in bytes ) of a data
    guint32                         a_dbk_capacity;                             //!< Size ( in data unit ) of a GwrCADBlock24
    guint32                         a_dbk_size;                                 //!< Size ( in bytes ) of a GwrCADBlock24
    guint32                         a_dbk_realloc;                              //!< # of GwrCADBlock24 to realloc when all are full
    guint32                         a_data_card;                                //!< # of data stored in the GwrCArrayEqual

    guint32                         a_stat_realloc;
};
//  ............................................................................
typedef struct  _GwrCArrayEqualStat GwrCArrayEqualStat;
//! \struct _GwrCArrayEqualStat
//!
//! \brief  Convenience struct for memory statistics.
struct  _GwrCArrayEqualStat
{
    GwrCArrayEqualSimpleStat        a_simple_stat;

    guint32                         a_dbk_capacity;
    guint32                         a_dbk_size;

    guint32                         a_realloc;

    GwrCAMFP                        a_mfp;
};
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          GwrCArrayEqual      *   gwr_array_equal_new                     (
    const   gchar                   *       _name           ,
            guint32                         _data_size      ,
            guint32                         _block_capacity ,
            guint32                         _block_realloc  );

extern          void                    gwr_array_equal_delete                  (
            GwrCArrayEqual          *       _ae             );

extern          void                    gwr_array_equal_reset                   (
            GwrCArrayEqual          *       _ae             );

extern          void                    gwr_array_equal_add_data                (
            GwrCArrayEqual          *       _ae     ,
            gpointer                        _data           );

extern          gboolean                gwr_array_equal_addb_data               (
            GwrCArrayEqual          *       _ae     ,
            gpointer                        _data           );
/*
extern          GwrCADBlock24       *   gwr_array_equal_get_block               (
            GwrCArrayEqual          *       _ae             ,
            guint32                         _block_index     );
*/
extern          gpointer                gwr_array_equal_get_data                (
            GwrCArrayEqual          *       _ae             ,
            guint32                         _data_index     );

extern          gboolean                gwr_array_equal_getb_data               (
            GwrCArrayEqual          *       _ae             ,
            guint32                         _data_index     ,
            gpointer                *       _dest           );

extern          void                    gwr_array_equal_dump                    (
            GwrCArrayEqual          *       _ae             );

extern          void                    gwr_array_equal_get_stats(
            GwrCArrayEqual          *       _ae             ,
            GwrCArrayEqualStat      *       _ae_stat        );

extern          void                    gwr_array_equal_get_mfp(
            GwrCArrayEqual          *       _ae             ,
            GwrCAMFP                *       _out            );

#if ( __cplusplus )
}
#endif
//  ============================================================================

#endif                                                                          //  __LIBGWRC_ARRAY_EQUAL_H__
