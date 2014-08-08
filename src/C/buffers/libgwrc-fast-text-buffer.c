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
    _line_attr->a_xdis      =   ( _line_desc->a_data1 & eGFTB_LD_XD_MASK ) >> eGFTB_LD_XD_OFFSET;
}
void                gwr_fast_text_buffer_line_desc_from_attributes(
    GwrFastTextBufferLineDesc    *   _line_desc  ,
    GwrFastTextBufferLineAttr    *   _line_attr  )
{
    _line_desc->a_data1     =   0;
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_fg)   << eGFTB_LD_FG_OFFSET ) & eGFTB_LD_FG_MASK );
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_bg)   << eGFTB_LD_BG_OFFSET ) & eGFTB_LD_BG_MASK );
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_st)   << eGFTB_LD_ST_OFFSET ) & eGFTB_LD_ST_MASK );
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_xdis) << eGFTB_LD_XD_OFFSET ) & eGFTB_LD_XD_MASK );
}
//  ============================================================================
//  ----------------------------------------------------------------------------
GwrFastTextBuffer*   gwr_fast_text_buffer_new(
        guint32                         _lines_text_data_block_size             ,
        guint32                         _lines_text_data_blocks_storage_realloc ,
        guint32                         _lines_text_info_blocks_storage_realloc ,

        guint32                         _lines_desc_info_blocks_storage_capacity    ,
        guint32                         _lines_desc_infos_blocks_storage_realloc    ,

        guint32                         _extra_data_data_block_size             ,
        guint32                         _extra_data_data_blocks_storage_realloc ,
        guint32                         _extra_data_info_block_size             ,
        guint32                         _extra_data_info_blocks_storage_realloc )
{
    GwrFastTextBuffer        *   b   =   NULL;
    GwrFastTextBufferPrivate *   p   =   NULL;
    //  ........................................................................
    b                   =   (GwrFastTextBuffer*)         g_new0( GwrFastTextBuffer        , 1 );
    p                   =   (GwrFastTextBufferPrivate*)  g_new0( GwrFastTextBufferPrivate , 1 );

    p->d_lines_text     =   gwr_array_data_multi_new(
                                "GwrFastTextBuffer:text lines",
                                _lines_text_data_block_size, _lines_text_data_blocks_storage_realloc  ,
                                                             _lines_text_info_blocks_storage_realloc  );

    p->d_lines_desc     =   gwr_array_equal_new(
                                "GwrFastTextBuffer::line desciptions"       ,
                                sizeof(GwrFastTextBufferLineDesc)           ,
                                _lines_desc_info_blocks_storage_capacity    ,
                                _lines_desc_infos_blocks_storage_realloc    );

    p->d_extra_data     =   gwr_array_data_multi_new(
                                "GwrFastTextBuffer:extra data",
                                _extra_data_data_block_size , _extra_data_data_blocks_storage_realloc   ,
                                                              _extra_data_info_blocks_storage_realloc   );

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
    bp->lines_card          =   0;
}

//  ----------------------------------------------------------------------------
gboolean            gwr_fast_text_buffer_get_line(
    GwrFastTextBuffer       *   _buffer     ,
    guint32                     _line_index ,
    GwrFastTextBufferLine   *   _line       )
{
    GwrFastTextBufferPrivate    *   bp  =   NULL;
    GwrCData16                      d_text;
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

    _line->a_extra_data     =   NULL;
    _line->a_extra_data_len =   0;

    return TRUE;
}
//  ----------------------------------------------------------------------------
gboolean            gwr_fast_text_buffer_get_line_and_extra_data(
    GwrFastTextBuffer       *   _buffer     ,
    guint32                     _line_index ,
    GwrFastTextBufferLine   *   _line       )
{
    GwrFastTextBufferPrivate    *   bp  =   NULL;
    GwrCData16                      d_text;
    GwrCData16                      extra_data;
    GwrFastTextBufferLineDesc   *   line_desc;
    guint32                         xdi =   0;
    //  ........................................................................
    bp  =   (GwrFastTextBufferPrivate*)_buffer->priv;

    if ( _line_index >= bp->lines_card )
        return FALSE;

    line_desc = gwr_array_equal_get_data( bp->d_lines_desc, _line_index);

    gwr_array_data_multi_get_data( bp->d_lines_text, _line_index, &d_text );

    gwr_fast_text_buffer_line_attributes_from_desc( &(_line->a_attr), line_desc );

    _line->a_str            =   d_text.a_mem;
    _line->a_str_len        =   d_text.a_size;

    _line->a_extra_data     =   NULL;
    _line->a_extra_data_len =   0;

    //  we have extra data
    if ( _line->a_attr.a_xdis )
    {
        memcpy( &( xdi ), _line->a_str + _line->a_str_len, _line->a_attr.a_xdis );

        gwr_array_data_multi_get_data( bp->d_extra_data, xdi, &extra_data );

        //D printf("gwr_fast_text_buffer_get_line():got line[%3i] xdis[%1i] xd[%p] xds[%3i] [",
        //D      _line_index            ,
        //D      _line->a_attr.a_xdis   ,
        //D     extra_data.a_mem        ,
        //D     extra_data.a_size       );

        //D for ( guint32 i = 0 ; i != extra_data.a_size ; i++ )
        //D {
        //D     printf("%c", *((gchar*)(extra_data.a_mem + i)) );
        //D }
        //D printf("]\n");

        _line->a_extra_data     =   extra_data.a_mem;
        _line->a_extra_data_len =   extra_data.a_size;
    }

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

    line_attr.a_fg      =   _fg;
    line_attr.a_bg      =   _bg;
    line_attr.a_st      =   _st;
    line_attr.a_xdis    =   0;

    gwr_fast_text_buffer_line_desc_from_attributes(&line_desc, &line_attr);

    gwr_array_equal_add_data        ( bp->d_lines_desc, &line_desc );
    //D printf("gwr_fast_text_buffer_add_line():adding line [%3i] fg [%2i]\n",
    //D    bp->d_lines_desc->a_data_card - 1   ,
    //D    line_attr.a_fg                      );

    gwr_array_data_multi_add_data   ( bp->d_lines_text, (gpointer)_text, (guint16)( (len) & 0x0000ffff) );
    //  ........................................................................
    //  update buffer infos
    if ( len > bp->lines_max_len )
        bp->lines_max_len = len;

    bp->lines_card          =   bp->d_lines_desc->a_data_card;
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
        (gpointer)_text                                         ,
        (guint16)( (len) & 0x0000ffff)                          ,
        (guint32)bp->d_extra_data->d_info_blocks->a_slots_used  ,
        &( line_attr.a_xdis )                                   );

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
void                gwr_fast_text_buffer_get_stats(
        GwrFastTextBuffer        *       _buffer ,
        GwrFastTextBufferStat    *       _s      )
{
    GwrFastTextBufferPrivate *   bp  =   NULL;
    //  ........................................................................
    bp  =   (GwrFastTextBufferPrivate*)_buffer->priv;

    gwr_array_data_multi_get_stats      ( bp->d_lines_text, &( _s->ld ) );
    gwr_array_equal_get_stats           ( bp->d_lines_desc, &( _s->li ) );
    gwr_array_data_multi_get_stats      ( bp->d_extra_data, &( _s->lx ) );
}
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_dump_mem(
        GwrFastTextBuffer       *       _buffer )
{
    GwrFastTextBufferPrivate    *   bp      =   NULL;
    GwrFastTextBufferStat           sftb;
    GwrCArrayDataMultiStat      *   sadm    =   NULL;
    GwrCArrayEqualStat          *   saeq    =   NULL;
    gchar                           text    [256];
    //  ........................................................................
    //I GwrCArrayDataMulti  *   d_lines_text;                                       //!< Contain all lines text
    //I GwrCArrayEqual      *   d_lines_desc;                                       //!< Contain all lines metadata
    //I GwrCArrayDataMulti  *   d_extra_data;
    //  ........................................................................
    bp  =   (GwrFastTextBufferPrivate*)_buffer->priv;

    gwr_fast_text_buffer_get_stats(_buffer, &sftb);
    //  ........................................................................
    //  header
    sprintf(text, "---------------- GwrFastTextBuffer statistics ----------------");
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
    //  ........................................................................
    //  text
    sadm    =   &( sftb.ld );                                                   //  data
    sprintf(text, "Data:data:using [%6i] blocks of [%6i] bytes",
        sadm->a_data_stat.a_slots_used  ,
        sadm->a_data_block_size         );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
    sprintf(text, "    :info:using [%6i] blocks of [%6i] bytes",
        sadm->a_info_stat.a_slots_used  ,
        GwrCADMDataInfo_SSIZE           );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
    sprintf(text, "    :mem :using [%6i] bytes  for self structs",
        sadm->a_mfp.a_ss );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
    sprintf(text, "    :mem :using [%6i] bytes  of [%6i] for data storage",
        sadm->a_mfp.a_su ,
        sadm->a_mfp.a_sa );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );

    saeq    =   &( sftb.li );                                                   //  info
    //sprintf(text, "Info     :using [%6i] blocks of [%6i] bytes",
    //    saeq->a_simple_stat.a_slots_used    ,
    //    saeq->a_dbk_size                    );
    //gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
    sprintf(text, "Info:mem :using [%6i] bytes  of [%6i] for data storage",
        saeq->a_mfp.a_su ,
        saeq->a_mfp.a_sa );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );

    sadm    =   &( sftb.lx );                                                   //  extra data
    sprintf(text, "Xtra:data:using [%6i] blocks of [%6i] bytes",
        sadm->a_data_stat.a_slots_used  ,
        sadm->a_data_block_size         );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
    sprintf(text, "    :info:using [%6i] blocks of [%6i] bytes",
        sadm->a_info_stat.a_slots_used  ,
        GwrCADMDataInfo_SSIZE           );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
    sprintf(text, "    :mem :using [%6i] bytes  for self structs",
        sadm->a_mfp.a_ss  );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
    sprintf(text, "    :mem :using [%6i] bytes  of [%6i] for data storage",
        sadm->a_mfp.a_su ,
        sadm->a_mfp.a_sa );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );

    sprintf(text, "--------------------------------------------------------------");
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
}

















