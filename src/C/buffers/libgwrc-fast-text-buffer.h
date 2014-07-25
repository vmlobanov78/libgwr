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
    guint16         a_st;                                                       //!< Style infos
    guint16         a_ln;                                                       //!< String len
};

typedef struct  _GwrFastTextBufferLine       GwrFastTextBufferLine;

struct  _GwrFastTextBufferLine
{
    GwrFastTextBufferLineAttr        a_attr;
    gchar                       *   a_str;                                      //!< String
}   __attribute__ ((packed));

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
    GwrCArrayDataMultiStat  lt;
    GwrCArrayEqualStat      ld;
};
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

/*
extern  inline  guint16                 gwr_fast_text_buffer_line_get_length      (
            GwrFastTextBufferLine    *       _line           );

extern  inline  void                    gwr_fast_text_buffer_line_set_length      (
            GwrFastTextBufferLine    *       _line           ,
            guint16                         _len            );

extern  inline  void                    gwr_fast_text_buffer_line_add_length      (
            GwrFastTextBufferLine    *       _line           ,
            guint16                         _add            );

extern  inline  guint16                 gwr_fast_text_buffer_line_get_style       (
            GwrFastTextBufferLine    *       _line           );
*/
extern  inline  void                    gwr_fast_text_buffer_line_set_attributes(
            GwrFastTextBufferLine    *       _line           ,
            guint32                         _attributes     );

#if ( __cplusplus )
}
#endif
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          GwrFastTextBuffer    *   gwr_fast_text_buffer_new               (
            guint32                         _lines_text_block_size      ,
            guint32                         _lines_text_blocks_realloc  ,
            guint32                         _lines_text_infos_realloc   ,
            guint32                         _lines_desc_realloc         );

extern          void                    gwr_fast_text_buffer_reset              (
            GwrFastTextBuffer        *       _buffer         );

extern          gboolean                gwr_fast_text_buffer_get_line           (
            GwrFastTextBuffer        *       _buffer         ,
            guint32                         _line_index     ,
            GwrFastTextBufferLine    *       _line           );

extern          void                    gwr_fast_text_buffer_add_line           (
            GwrFastTextBuffer        *       _buffer         ,
    const   gchar*                          _text           ,
            guint8                          _fg             ,
            guint8                          _bg             ,
            guint16                         _st             );

extern          void                    gwr_fast_text_buffer_get_stats          (
            GwrFastTextBuffer        *       _buffer ,
            GwrFastTextBufferStat    *       _s      );

#if ( __cplusplus )
}
#endif




#endif                                                                          // __LIBGWRC_FAST_TEXT_BUFFER_H__
