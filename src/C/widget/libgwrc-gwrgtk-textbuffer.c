/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-gwrgtk-textbuffer.c                                             *
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
    *   Purpose : A Simple and _FAST_ GtkTextBuffer-like for using as model of  *
    *       GwrGtkTextView.                                                     *
    *                                                                           *
    *****************************************************************************
*/
#include    <string.h>

#include    "libgwrc.h"
#include    "C/widget/libgwrc-gwrgtk-textbuffer.h"
#include    "C/widget/libgwrc-gwrgtk-textbuffer-private.h"

guint32     GwrGtkTextBufferLine_S      =   sizeof(GwrGtkTextBufferLine);
guint32     GwrGtkTextBufferLineDesc_S  =   sizeof(GwrGtkTextBufferLineDesc_S);
//  ============================================================================
guint16             gwrgtk_text_buffer_line_get_style       (GwrGtkTextBufferLine* _line)
{
    return _line->a_style;
}
void                gwrgtk_text_buffer_line_set_attributes  (GwrGtkTextBufferLine* _line, guint32 _attributes)
{
    _line->a_fg     =   ( _attributes & 0xff000000 ) >> 24;
    _line->a_bg     =   ( _attributes & 0x00ff0000 ) >> 16;
    _line->a_style  =   ( _attributes & 0x0000ffff );
}
//  ============================================================================
//  ----------------------------------------------------------------------------
GwrGtkTextBuffer*   gwrgtk_text_buffer_new              (
        guint32                         _lines_text_block_size       ,
        guint32                         _lines_text_blocks_realloc   ,
        guint32                         _lines_text_infos_realloc    ,
        guint32                         _lines_desc_block_size       ,
        guint32                         _lines_desc_blocks_realloc   ,
        guint32                         _lines_desc_infos_realloc    )
{
    GwrGtkTextBuffer        *   b   =   NULL;
    GwrGtkTextBufferPrivate *   p   =   NULL;
    //  ........................................................................
    b                   =   (GwrGtkTextBuffer*)         g_new0( GwrGtkTextBuffer        , 1 );
    p                   =   (GwrGtkTextBufferPrivate*)  g_new0( GwrGtkTextBufferPrivate , 1 );

    p->d_lines_text     =   gwr_array_data_multi_new( _lines_text_block_size, _lines_text_blocks_realloc, _lines_text_infos_realloc );
    p->d_lines_desc     =   gwr_array_data_multi_new( _lines_desc_block_size, _lines_desc_blocks_realloc, _lines_desc_infos_realloc );
    p->lines_max_len    =   0;

    b->priv             =   (gpointer)p;

    b->lines_card       =   p->d_lines_desc->d_infos->a_blocks_used;
    return b;
}
void                gwrgtk_text_buffer_reset            (GwrGtkTextBuffer* _buffer)
{
    GwrGtkTextBufferPrivate *   p   =   NULL;
    //  ........................................................................
    p                   =   (GwrGtkTextBufferPrivate*)(_buffer->priv);

    gwr_array_data_multi_reset( p->d_lines_text );
    gwr_array_data_multi_reset( p->d_lines_desc );

    p->lines_max_len        =   0;
    _buffer->lines_card     =   p->d_lines_desc->d_infos->a_blocks_used;
}
//  ----------------------------------------------------------------------------
gboolean            gwrgtk_text_buffer_get_line         (GwrGtkTextBuffer* _buffer, guint32 _line_index, GwrGtkTextBufferLine* _line)
{
    GwrGtkTextBufferPrivate *   bp  =   NULL;
    GwrCADMData                 d_desc;
    GwrCADMData                 d_text;
    GwrGtkTextBufferLineDesc    line_desc;
    //  ........................................................................
    if ( _line_index >= _buffer->lines_card )
        return FALSE;

    bp  =   (GwrGtkTextBufferPrivate*)_buffer->priv;

    g_return_val_if_fail(
        gwr_array_data_multi_get_data( bp->d_lines_desc, _line_index, &d_desc ),
        FALSE           );

    g_return_val_if_fail(
        gwr_array_data_multi_get_data( bp->d_lines_text, _line_index, &d_text ),
        FALSE           );

    memcpy( &line_desc, d_desc.a_mem, d_desc.a_size );

    gwrgtk_text_buffer_line_set_attributes(_line, line_desc.a_attributes );

    _line->a_len    =   d_text.a_size;
    _line->a_str    =   d_text.a_mem;

    return TRUE;
}
void                gwrgtk_text_buffer_add_line         (GwrGtkTextBuffer* _buffer, guint32 _attributes, const gchar* _text)
{
    GwrGtkTextBufferPrivate *   bp                      =   NULL;
    guint32                     len                     =   0;
    GwrGtkTextBufferLineDesc    line_desc;
    //  ........................................................................
    //  init some vars
    bp   =   _buffer->priv;

    len  =   strlen(_text);
    g_return_if_fail( len < 32768 );

    line_desc.a_attributes  =   _attributes;
    gwr_array_data_multi_add_data( bp->d_lines_desc, &line_desc, GwrGtkTextBufferLineDesc_S );
    gwr_array_data_multi_add_data( bp->d_lines_text, (gpointer)_text, (guint16)( (1+len) & 0x0000ffff) );
    //  ........................................................................
    //  update buffer infos
    if ( len > bp->lines_max_len )
        bp->lines_max_len = len;

    bp->lines_card          =   bp->d_lines_desc->d_infos->a_blocks_used;
    _buffer->lines_card     =   bp->lines_card ;
}
//  ----------------------------------------------------------------------------
void                gwrgtk_text_buffer_get_stats_alloc  (
        GwrGtkTextBuffer        *       _buffer ,
        GwrGtkTextBufferStat    *       _s      )
{
    GwrGtkTextBufferPrivate *   bp  =   NULL;
    //  ........................................................................
    bp  =   (GwrGtkTextBufferPrivate*)_buffer->priv;

    gwr_array_data_multi_get_stats_alloc( bp->d_lines_text, &( _s->lt ) );
    gwr_array_data_multi_get_stats_alloc( bp->d_lines_desc, &( _s->ld ) );
}


















