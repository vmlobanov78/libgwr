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
typedef struct  _GwrFastTextBufferLineAttr      GwrFastTextBufferLineAttr;

struct  _GwrFastTextBufferLineAttr
{
    guint8          a_fg;                                                       //!< Foreground color
    guint8          a_bg;                                                       //!< Background color
    guint8          a_st;                                                       //!< Style infos
    guint8          a_dis;
    guint8          a_uis;
};
//  ----------------------------------------------------------------------------
typedef struct  _GwrFastTextBufferSegmentAttr   GwrFastTextBufferSegmentAttr;

struct  _GwrFastTextBufferSegmentAttr
{
    guint16         a_of;                                                       //!< Offset
    guint16         a_ln;                                                       //!< Length
    guint8          a_fg;                                                       //!< Foreground color
    guint8          a_bg;                                                       //!< Backgrounf color
    guint8          a_st;                                                       //!< Style
};
//  ----------------------------------------------------------------------------
typedef struct  _GwrFastTextBufferLine       GwrFastTextBufferLine;

struct  _GwrFastTextBufferLine
{
    GwrFastTextBufferLineAttr       a_attr;

    gchar                       *   a_str;                                      //!< String
    guint16                         a_str_len;

    gpointer                        a_extra_data;
    guint16                         a_extra_data_len;

    guint16                         a_ho;
    guint8                          a_hl;
};

extern      guint32     GwrFastTextBufferLine_S;


extern  inline      gboolean    gwr_fast_text_buffer_line_has_data(GwrFastTextBufferLine* _line);
extern  inline      gboolean    gwr_fast_text_buffer_line_has_html(GwrFastTextBufferLine* _line);
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
    GwrCArrayDataMultiStat      ld;
    GwrCArrayEqualSimpleStat    li;
    GwrCArrayDataMultiStat      lx;
    GwrCArrayEqualStat          lu;
};

//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          GwrFastTextBuffer   *   gwr_fast_text_buffer_new                (
            guint32                         _lines_text_data_block_size                 ,
            guint32                         _lines_text_data_blocks_storage_realloc     ,
            guint32                         _lines_text_info_blocks_storage_realloc     ,

            guint32                         _lines_desc_info_blocks_storage_realloc     ,

            guint32                         _extra_data_data_block_size                 ,
            guint32                         _extra_data_data_blocks_storage_realloc     ,
            guint32                         _extra_data_info_blocks_storage_realloc     ,

            guint32                         _data_url_info_blocks_storage_capacity     ,
            guint32                         _data_url_info_blocks_storage_realloc      );

extern          void                    gwr_fast_text_buffer_reset              (
            GwrFastTextBuffer       *       _buffer         );

extern          gboolean                gwr_fast_text_buffer_line_exist_and_has_data    (
            GwrFastTextBuffer       *       _buffer         ,
            guint32                         _line_index     );

extern          gboolean                gwr_fast_text_buffer_get_line           (
            GwrFastTextBuffer       *       _buffer         ,
            guint32                         _line_index     ,
            GwrFastTextBufferLine   *       _line           );

extern          gboolean                gwr_fast_text_buffer_get_line_and_data  (
            GwrFastTextBuffer       *       _buffer         ,
            guint32                         _line_index     ,
            GwrFastTextBufferLine   *       _line           );

extern          void                    gwr_fast_text_buffer_add_line           (
            GwrFastTextBuffer       *       _buffer         ,
    const   gchar*                          _text           ,
            guint8                          _fg             ,
            guint8                          _bg             ,
            guint16                         _st             );

extern          void                    gwr_fast_text_buffer_add_line_with_data (
            GwrFastTextBuffer       *       _buffer         ,
    const   gchar                   *       _text           ,
            guint8                          _fg             ,
            guint8                          _bg             ,
            guint16                         _st             ,
            gpointer                        _data           ,
            guint16                         _data_len       );

extern          void                    gwr_fast_text_buffer_add_line_with_data_and_url (
            GwrFastTextBuffer       *       _buffer         ,
    const   gchar                   *       _text           ,
            guint8                          _fg             ,
            guint8                          _bg             ,
            guint16                         _st             ,
            gpointer                        _data           ,
            guint16                         _data_len       ,
            guint16                         _url_offset     ,
            guint8                          _url_len        );

extern          void                    gwr_fast_text_buffer_get_stats          (
            GwrFastTextBuffer       *       _buffer ,
            GwrFastTextBufferStat   *       _s      );

extern          void                    gwr_fast_text_buffer_dump_mem           (
            GwrFastTextBuffer       *       _buffer );



/*
extern          void                    gwr_fast_text_buffer_segmented_line_start   (
            GwrFastTextBuffer       *       _buffer         ,
            guint8                          _fg             ,
            guint8                          _bg             ,
            guint16                         _st             ,
    const   gchar*                          _text           );

extern          void                    gwr_fast_text_buffer_segmented_line_add     (
            GwrFastTextBuffer       *       _buffer         ,
            guint8                          _fg             ,
            guint8                          _bg             ,
            guint16                         _st             ,
    const   gchar*                          _text           );

extern          void                    gwr_fast_text_buffer_segmented_line_end     (
            GwrFastTextBuffer       *       _buffer         ,
            gpointer                        _extra_data     ,
            guint16                         _extra_data_len ,
            guint8                          _fg             ,
            guint8                          _bg             ,
            guint16                         _st             ,
    const   gchar*                          _text           );
*/


#if ( __cplusplus )
}
#endif




#endif                                                                          // __LIBGWRC_FAST_TEXT_BUFFER_H__
