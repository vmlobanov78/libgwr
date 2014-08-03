/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-equal-simple.h                                            *
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
    *   Purpose :   Array of constant-size datas, simple version                *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWRC_ARRAY_EQUAL_SIMPLE_H__
#define     __LIBGWRC_ARRAY_EQUAL_SIMPLE_H__
//  ............................................................................
#include    <glib.h>
//  ............................................................................
typedef struct  _GwrCArrayEqualSimple     GwrCArrayEqualSimple;

//! \struct _GwrCArrayEqualSimple
//!
//! \brief  Array of constant size datas.
struct  _GwrCArrayEqualSimple
{
    gpointer    d_mem;                                                          //!< Location of array in memory
    guint32     a_data_size;                                                    //!< Size ( in bytes ) of a data
    guint32     a_realloc_size;                                                 //!< # of slots to realloc when no more available

    guint32     a_slots_card;                                                   //!< # of slots in the array
    guint32     a_slots_used;                                                   //!< # of slots used in the array

    guint32     a_stat_realloc;                                                 //!< # of reallocations since creation / last reset
};
//  ............................................................................
typedef struct  _GwrCArrayEqualSimpleStat GwrCArrayEqualSimpleStat;
//! \struct _GwrCArrayEqualSimpleStat
//!
//! \brief  Convenience struct for memory statistics.
struct  _GwrCArrayEqualSimpleStat
{
    guint32             a_size_bytes;
    guint32             a_slots_card;
    guint32             a_slots_used;
    guint32             a_realloc;
};
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          GwrCArrayEqualSimple*   gwr_array_equal_simple_new              (
            guint32                         _block_size     ,
            guint32                         _realloc_size   );

extern          void                    gwr_array_equal_simple_delete           (
            GwrCArrayEqualSimple    *       _aes            );

extern          void                    gwr_array_equal_simple_reset            (
            GwrCArrayEqualSimple    *       _aes            );

extern          void                    gwr_array_equal_simple_dealloc          (
            GwrCArrayEqualSimple    *       _aes            );

extern          void                    gwr_array_equal_simple_add              (
            GwrCArrayEqualSimple    *       _aes            ,
            gpointer                        _data           );

extern          gboolean                gwr_array_equal_simple_addb             (
            GwrCArrayEqualSimple    *       _aes            ,
            gpointer                        _data           );

extern          gpointer                gwr_array_equal_simple_get              (
            GwrCArrayEqualSimple    *       _aes            ,
            guint32                         _block_index    );

extern          gboolean                gwr_array_equal_simple_getb             (
            GwrCArrayEqualSimple    *       _aes            ,
            guint32                         _block_index    ,
            gpointer                        _dest           );

extern          void                    gwr_array_equal_simple_dump             (
            GwrCArrayEqualSimple    *       _aes            );

extern          void                    gwr_array_equal_simple_get_stats    (
            GwrCArrayEqualSimple    *       _aes            ,
            GwrCArrayEqualSimpleStat*       _ae_stat        );

#if ( __cplusplus )
}
#endif
//  ============================================================================

#endif                                                                          //  __LIBGWRC_ARRAY_EQUAL_SIMPLE_H__
