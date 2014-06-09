/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-gwrgtk-textbuffer.h                                             *
    *                                                                           *
    *   --------------------------------------------------------------------    *
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
    *   Purpose : Header file for GwrGtkTextBuffer                              *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWRC_GWRGTK_TEXT_BUFFER_H__
#define __LIBGWRC_GWRGTK_TEXT_BUFFER_H__
//  ............................................................................
#include <glib.h>
//  ............................................................................
#include    "libgwrc.h"
//  ............................................................................
typedef struct  _GwrGtkTextBufferLine       GwrGtkTextBufferLine;

struct  _GwrGtkTextBufferLine
{
            guint8          a_fg;                                               //!< Foreground color
            guint8          a_bg;                                               //!< Background color
            guint16         a_style;                                            //!< Style infos
            guint16         a_len;                                              //!< String len
            gchar       *   a_str;                                              //!< String
}   __attribute__ ((packed));

extern      guint32     GwrGtkTextBufferLine_S;
//  ----------------------------------------------------------------------------
typedef struct  _GwrGtkTextBuffer           GwrGtkTextBuffer;

struct  _GwrGtkTextBuffer
{
    gpointer    priv;                                                           //!< Private data

    guint32     lines_card;
};
//  ----------------------------------------------------------------------------
typedef struct  _GwrGtkTextBufferStat       GwrGtkTextBufferStat;

struct  _GwrGtkTextBufferStat
{
    GwrCArrayDataMultiStat  lt;
    GwrCArrayDataMultiStat  ld;
};
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern  inline  guint16                 gwrgtk_text_buffer_line_get_length      (
            GwrGtkTextBufferLine    *       _line           );
extern  inline  void                    gwrgtk_text_buffer_line_set_length      (
            GwrGtkTextBufferLine    *       _line           ,
            guint16                         _len            );
extern  inline  void                    gwrgtk_text_buffer_line_add_length      (
            GwrGtkTextBufferLine    *       _line           ,
            guint16                         _add            );

extern  inline  guint16                 gwrgtk_text_buffer_line_get_style       (
            GwrGtkTextBufferLine    *       _line           );

extern  inline  void                    gwrgtk_text_buffer_line_set_attributes  (
            GwrGtkTextBufferLine    *       _line           ,
            guint32                         _attributes     );

#if ( __cplusplus )
}
#endif
//  ============================================================================
#if ( __cplusplus )
extern "C" {
#endif

extern          GwrGtkTextBuffer    *   gwrgtk_text_buffer_new                  (
            guint32                         _lines_text_block_size       ,
            guint32                         _lines_text_blocks_realloc   ,
            guint32                         _lines_text_infos_realloc    ,
            guint32                         _lines_desc_block_size       ,
            guint32                         _lines_desc_blocks_realloc   ,
            guint32                         _lines_desc_infos_realloc    );

extern          void                    gwrgtk_text_buffer_reset                (
            GwrGtkTextBuffer        *       _buffer         );

extern          gboolean                gwrgtk_text_buffer_get_line             (
            GwrGtkTextBuffer        *       _buffer         ,
            guint32                         _line_index     ,
            GwrGtkTextBufferLine    *       _line           );

extern          void                    gwrgtk_text_buffer_add_line             (
            GwrGtkTextBuffer        *       _buffer         ,
            guint32                         _attributes     ,
    const   gchar*                          _text           );

extern          void                    gwrgtk_text_buffer_get_stats_alloc      (
            GwrGtkTextBuffer        *       _buffer ,
            GwrGtkTextBufferStat    *       _s      );

#if ( __cplusplus )
}
#endif




#endif                                                                          // __LIBGWRC_GWRGTK_TEXT_BUFFER_H__
