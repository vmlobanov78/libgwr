/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-fast-text-buffer.c                                              *
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
    *   Purpose : A Simple and _FAST_ GtkTextBuffer-like for using as model of  *
    *       GwrGtkTextView.                                                     *
    *                                                                           *
    *****************************************************************************
*/
#include    <string.h>

#include    "libgwrc-fast-text-buffer-private.h"
#include    "libgwrc-fast-text-buffer.h"

guint32     GwrFastTextBufferLine_S      =   sizeof(GwrFastTextBufferLine);
guint32     GwrFastTextBufferLineDesc_S  =   sizeof(GwrFastTextBufferLineDesc_S);
//  ============================================================================
void                gwr_fast_text_buffer_line_attributes_from_desc(
    GwrFastTextBufferLineAttr    *   _line_attr  ,
    GwrFastTextBufferLineDesc    *   _line_desc  )
{
    _line_attr->a_fg        =   ( _line_desc->a_data1 & eGFTB_LD_FG_MASK ) >> eGFTB_LD_FG_OFFSET;
    _line_attr->a_bg        =   ( _line_desc->a_data1 & eGFTB_LD_BG_MASK ) >> eGFTB_LD_BG_OFFSET;
    _line_attr->a_st        =   ( _line_desc->a_data1 & eGFTB_LD_ST_MASK ) >> eGFTB_LD_ST_OFFSET;
    _line_attr->a_xd        =   ( _line_desc->a_data1 & eGFTB_LD_XD_MASK ) >> eGFTB_LD_XD_OFFSET;
}
void                gwr_fast_text_buffer_line_desc_from_attributes(
    GwrFastTextBufferLineDesc    *   _line_desc  ,
    GwrFastTextBufferLineAttr    *   _line_attr  )
{
    _line_desc->a_data1     =   0;
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_fg) << eGFTB_LD_FG_OFFSET ) & eGFTB_LD_FG_MASK );
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_bg) << eGFTB_LD_BG_OFFSET ) & eGFTB_LD_BG_MASK );
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_st) << eGFTB_LD_ST_OFFSET ) & eGFTB_LD_ST_MASK );
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_xd) << eGFTB_LD_XD_OFFSET ) & eGFTB_LD_XD_MASK );
}
//  ============================================================================
//  ----------------------------------------------------------------------------
GwrFastTextBuffer*   gwr_fast_text_buffer_new(
        guint32                         _lines_text_block_size                  ,
        guint32                         _lines_text_blocks_storage_capacity     ,
        guint32                         _lines_text_blocks_storage_realloc      ,
        guint32                         _lines_text_infos_storage_capacity      ,
        guint32                         _lines_text_infos_storage_realloc       ,

        guint32                         _lines_desc_infos_blocks_storage_capacity   ,
        guint32                         _lines_desc_infos_blocks_storage_realloc    ,

        guint32                         _extra_data_block_size                  ,
        guint32                         _extra_data_blocks_storage_capacity     ,
        guint32                         _extra_data_blocks_storage_realloc      ,
        guint32                         _extra_data_infos_storage_capacity      ,
        guint32                         _extra_data_infos_storage_realloc       )
{
    GwrFastTextBuffer        *   b   =   NULL;
    GwrFastTextBufferPrivate *   p   =   NULL;
    //  ........................................................................
    b                   =   (GwrFastTextBuffer*)         g_new0( GwrFastTextBuffer        , 1 );
    p                   =   (GwrFastTextBufferPrivate*)  g_new0( GwrFastTextBufferPrivate , 1 );

    p->d_lines_text     =   gwr_array_data_multi_new(
                                _lines_text_block_size,
                                _lines_text_blocks_storage_capacity ,   _lines_text_blocks_storage_realloc  ,
                                _lines_text_infos_storage_capacity  ,   _lines_text_infos_storage_realloc   );

    p->d_lines_desc     =   gwr_array_equal_new(
                                "GwrFastTextBuffer::line desciptions"       ,
                                sizeof(GwrFastTextBufferLineDesc)           ,
                                _lines_desc_infos_blocks_storage_capacity   ,
                                _lines_desc_infos_blocks_storage_realloc    );

    p->d_extra_data     =   gwr_array_data_multi_new(
                                _extra_data_block_size              ,
                                _extra_data_blocks_storage_capacity ,   _extra_data_blocks_storage_realloc  ,
                                _extra_data_infos_storage_capacity  ,   _extra_data_infos_storage_realloc   );

    p->lines_max_len    =   0;

    b->priv             =   (gpointer)p;

    return b;
}
void                gwr_fast_text_buffer_reset            (GwrFastTextBuffer* _buffer)
{
    GwrFastTextBufferPrivate *   bp  =   NULL;
    //  ........................................................................
    bp                  =   (GwrFastTextBufferPrivate*)(_buffer->priv);

    gwr_array_data_multi_reset  ( bp->d_lines_text );
    gwr_array_equal_reset       ( bp->d_lines_desc );
    gwr_array_data_multi_reset  ( bp->d_extra_data );

    bp->lines_max_len       =   0;
    bp->lines_card          =   bp->d_lines_desc->a_data_card;
}
//  ----------------------------------------------------------------------------
gboolean            gwr_fast_text_buffer_get_line         (GwrFastTextBuffer* _buffer, guint32 _line_index, GwrFastTextBufferLine* _line)
{
    GwrFastTextBufferPrivate    *   bp  =   NULL;
    GwrCADMData                     d_text;
    GwrFastTextBufferLineDesc   *   line_desc;
    //  ........................................................................
    bp  =   (GwrFastTextBufferPrivate*)_buffer->priv;

    if ( _line_index >= bp->lines_card )
        return FALSE;

    line_desc = gwr_array_equal_get_data( bp->d_lines_desc, _line_index);

    gwr_array_data_multi_get_data( bp->d_lines_text, _line_index, &d_text );

    gwr_fast_text_buffer_line_attributes_from_desc( &(_line->a_attr), line_desc );

    _line->a_str            =   d_text.a_mem;
    _line->a_str_len        =   d_text.a_size;

    return TRUE;
}
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_add_line(
            GwrFastTextBuffer   *   _buffer ,
    const   gchar               *   _text   ,
            guint8                  _fg     ,
            guint8                  _bg     ,
            guint16                 _st     )
{
    GwrFastTextBufferPrivate    *   bp              =   NULL;
    guint32                         len             =   0;
    GwrFastTextBufferLineDesc       line_desc;
    GwrFastTextBufferLineAttr       line_attr;
    //  ........................................................................
    //  init some vars
    bp   =   _buffer->priv;

    len  =   strlen(_text);

    g_return_if_fail( len < 512 );

    line_attr.a_fg  =   _fg;
    line_attr.a_bg  =   _bg;
    line_attr.a_st  =   _st;
    line_attr.a_xd  =   0;

    gwr_fast_text_buffer_line_desc_from_attributes(&line_desc, &line_attr);

    gwr_array_equal_add_data        ( bp->d_lines_desc, &line_desc );
    gwr_array_data_multi_add_data   ( bp->d_lines_text, (gpointer)_text, (guint16)( (len) & 0x0000ffff) );
    //  ........................................................................
    //  update buffer infos
    if ( len > bp->lines_max_len )
        bp->lines_max_len = len;

    bp->lines_card          =   bp->d_lines_desc->a_data_card;

    gwr_fast_text_buffer_dump( _buffer );
}
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_add_line_with_extra_data(
            GwrFastTextBuffer   *   _buffer         ,
    const   gchar               *   _text           ,
            guint8                  _fg             ,
            guint8                  _bg             ,
            guint16                 _st             ,
            gpointer                _extra_data     ,
            guint16                 _extra_data_len )
{
    GwrFastTextBufferPrivate    *   bp              =   NULL;
    guint32                         len             =   0;
    GwrFastTextBufferLineDesc       line_desc;
    GwrFastTextBufferLineAttr       line_attr;
    //  ........................................................................
    //  init some vars
    bp   =   _buffer->priv;

    len  =   strlen(_text);

    g_return_if_fail( len < 512 );

    line_attr.a_fg  =   _fg;
    line_attr.a_bg  =   _bg;
    line_attr.a_st  =   _st;
    gwr_array_data_multi_add_data_and_extra_data_index  ( bp->d_lines_text  ,
        (gpointer)_text                             ,
        (guint16)( (len) & 0x0000ffff)              ,
        (guint32)bp->d_lines_desc->a_data_card      ,
        &( line_attr.a_xd )                         );

    gwr_fast_text_buffer_line_desc_from_attributes(&line_desc, &line_attr);

    gwr_array_data_multi_add_data( bp->d_extra_data  ,
        (gpointer)_extra_data           ,
        _extra_data_len                 );

    gwr_array_equal_add_data( bp->d_lines_desc, &line_desc );
    //  ........................................................................
    //  update buffer infos
    if ( len > bp->lines_max_len )
        bp->lines_max_len = len;

    bp->lines_card          =   bp->d_lines_desc->a_data_card;
}
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_dump(
        GwrFastTextBuffer        *       _buffer    )
{
    GwrFastTextBufferPrivate    *   bp              =   _buffer->priv;
    //  ........................................................................
    //  init some vars
    //gwr_array_data_multi_dump( bp->d_lines_text );
    //gwr_array_data_multi_dump( bp->d_extra_data );
    //gwr_array_equal_dump( bp->d_lines_desc );
}
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_get_stats(
        GwrFastTextBuffer        *       _buffer ,
        GwrFastTextBufferStat    *       _s      )
{
    GwrFastTextBufferPrivate *   bp  =   NULL;
    //  ........................................................................
    bp  =   (GwrFastTextBufferPrivate*)_buffer->priv;

    gwr_array_data_multi_get_stats      ( bp->d_lines_text, &( _s->lt ) );
    gwr_array_equal_get_stats           ( bp->d_lines_desc, &( _s->ld ) );
}


















