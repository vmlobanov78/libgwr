/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-fast-text-buffer.h                                              *
    *                                                                           *
    *   --------------------------------------------------------------------    *
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
    *   Purpose : Header file for GwrFastTextBuffer                              *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWRC_FAST_TEXT_BUFFER_H__
#define __LIBGWRC_FAST_TEXT_BUFFER_H__
//  ............................................................................
#include <glib.h>
//  ............................................................................
#include "C/arrays/libgwrc-array-equal.h"
#include "C/arrays/libgwrc-array-data-multi.h"
//  ............................................................................
typedef struct  _GwrFastTextBufferLineAttr   GwrFastTextBufferLineAttr;

struct  _GwrFastTextBufferLineAttr
{
    guint8          a_fg;                                                       //!< Foreground color
    guint8          a_bg;                                                       //!< Background color
    guint8          a_st;                                                       //!< Style infos
    guint8          a_xdis;
};

typedef struct  _GwrFastTextBufferLine       GwrFastTextBufferLine;

struct  _GwrFastTextBufferLine
{
    GwrFastTextBufferLineAttr       a_attr;

    gchar                       *   a_str;                                      //!< String
    guint16                         a_str_len;

    gpointer                        a_extra_data;
    guint16                         a_extra_data_len;
};

extern      guint32     GwrFastTextBufferLine_S;
//  ----------------------------------------------------------------------------
typedef struct  _GwrFastTextBuffer           GwrFastTextBuffer;

struct  _GwrFastTextBuffer
{
    gpointer    priv;                                                           //!< Private data
};
//  ----------------------------------------------------------------------------
typedef struct  _GwrFastTextBufferStat       GwrFastTextBufferStat;

struct  _GwrFastTextBufferStat
{
    GwrCArrayDataMultiStat  ld;
    GwrCArrayEqualStat      li;
    GwrCArrayDataMultiStat  lx;
};

//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          GwrFastTextBuffer   *   gwr_fast_text_buffer_new                (
            guint32                         _lines_text_data_block_size                 ,
            guint32                         _lines_text_data_blocks_storage_realloc     ,
            guint32                         _lines_text_info_blocks_storage_realloc     ,

            guint32                         _lines_desc_info_blocks_storage_capacity    ,
            guint32                         _lines_desc_infos_blocks_storage_realloc    ,

            guint32                         _extra_data_data_block_size                 ,
            guint32                         _extra_data_data_blocks_storage_realloc     ,
            guint32                         _extra_data_info_block_size                 ,
            guint32                         _extra_data_info_blocks_storage_realloc     );

extern          void                    gwr_fast_text_buffer_reset              (
            GwrFastTextBuffer       *       _buffer         );

extern          gboolean                gwr_fast_text_buffer_get_line           (
            GwrFastTextBuffer       *       _buffer         ,
            guint32                         _line_index     ,
            GwrFastTextBufferLine   *       _line           );

extern          gboolean                gwr_fast_text_buffer_get_line_and_extra_data    (
            GwrFastTextBuffer       *       _buffer         ,
            guint32                         _line_index     ,
            GwrFastTextBufferLine   *       _line           );

extern          void                    gwr_fast_text_buffer_add_line           (
            GwrFastTextBuffer       *       _buffer         ,
    const   gchar*                          _text           ,
            guint8                          _fg             ,
            guint8                          _bg             ,
            guint16                         _st             );

extern          void                    gwr_fast_text_buffer_add_line_with_extra_data   (
            GwrFastTextBuffer       *       _buffer         ,
    const   gchar                   *       _text           ,
            guint8                          _fg             ,
            guint8                          _bg             ,
            guint16                         _st             ,
            gpointer                        _extra_data     ,
            guint16                         _extra_data_len );

extern          void                    gwr_fast_text_buffer_get_stats          (
            GwrFastTextBuffer       *       _buffer ,
            GwrFastTextBufferStat   *       _s      );

extern          void                    gwr_fast_text_buffer_dump_mem           (
            GwrFastTextBuffer       *       _buffer );

#if ( __cplusplus )
}
#endif




#endif                                                                          // __LIBGWRC_FAST_TEXT_BUFFER_H__
