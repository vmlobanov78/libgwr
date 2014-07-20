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
#include    <glib.h>
//  ............................................................................
typedef struct  _GwrCArrayEqual     GwrCArrayEqual;

//! \struct _GwrCArrayEqual
//!
//! \brief  Array of constant size datas.
struct  _GwrCArrayEqual
{
    gpointer    d_mem;                                                          //!< Location of array in memory
    guint32     a_block_size;                                                   //!< Size ( in bytes ) of a block
    guint32     a_realloc_size;                                                 //!< # of blocks to realloc when array is full

    guint32     a_blocks_card;                                                  //!< # of data blocks in the array
    guint32     a_blocks_used;                                                  //!< # of data blocks used in the array

    guint32     a_stat_realloc;                                                 //!< # of reallocations since creation / last reset
};
//  ............................................................................
typedef struct  _GwrCArrayEqualStat GwrCArrayEqualStat;
//! \struct _GwrCArrayEqualStat
//!
//! \brief  Convenience struct for memory statistics.
struct  _GwrCArrayEqualStat
{
    guint32             a_size_bytes;
    guint32             a_blocks_card;
    guint32             a_blocks_used;
    guint32             a_blocks_alloc;
};
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          void                    gwr_array_equal_dump                    (
            GwrCArrayEqual          *       _ae             );

extern          GwrCArrayEqual      *   gwr_array_equal_new                     (
            guint32                         _block_size     ,
            guint32                         _realloc_size   );

extern          void                    gwr_array_equal_delete                  (
            GwrCArrayEqual          *       _ae             );

extern          void                    gwr_array_equal_reset                   (
            GwrCArrayEqual          *       _ae             );

extern          void                    gwr_array_equal_dealloc                 (
            GwrCArrayEqual          *       _ae             );

extern          gboolean                gwr_array_equal_add                     (
            GwrCArrayEqual          *       _ae     ,
            gpointer                        _data           );

extern          gpointer                gwr_array_equal_get                     (
            GwrCArrayEqual          *       _ae     ,
            guint32                         _block_index    );

extern          gboolean                gwr_array_equal_getb                    (
            GwrCArrayEqual          *       _ae             ,
            guint32                         _block_index    ,
            gpointer                        _dest           );

extern          void                    gwr_array_equal_get_stats(
            GwrCArrayEqual          *       _ae             ,
            GwrCArrayEqualStat      *       _ae_stat        );

#if ( __cplusplus )
}
#endif
//  ============================================================================

#endif                                                                          //  __LIBGWRC_ARRAY_EQUAL_H__