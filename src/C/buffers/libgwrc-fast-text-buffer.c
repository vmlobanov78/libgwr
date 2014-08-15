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
static inline
void                    gwr_fast_text_buffer_uint32_pack1(
    gpointer                        _out    ,
    guint32                         _u1     ,
    guint8          *               _o1     )
{
    memcpy( _out, & _u1, sizeof(guint32) );
    * _o1   =   4;
}
static inline
void                    gwr_fast_text_buffer_uint32_pack2(
    gpointer                        _out    ,
    guint32                         _u1     ,
    guint32                         _u2     ,
    guint8          *               _o1     ,
    guint8          *               _o2     )
{
    memcpy( _out                    , & _u1, sizeof(guint32) );
    memcpy( _out + sizeof(guint32)  , & _u2, sizeof(guint32) );

    * _o1   =   4;
    * _o2   =   4;
}
static inline
void                    gwr_fast_text_buffer_uint32_unpack2(
    gpointer                        _src    ,
    guint32         *               _u1     ,
    guint32         *               _u2     ,
    guint8                          _l1     ,
    guint8                          _l2     )
{
    memcpy( _u1, _src       , _l1 );
    memcpy( _u2, _src + _l1 , _l2 );
}
//  ============================================================================
static inline
void                gwr_fast_text_buffer_line_attributes_from_desc(
    GwrFastTextBufferLineAttr    *   _line_attr  ,
    GwrFastTextBufferLineDesc    *   _line_desc  )
{
    _line_attr->a_fg        =   ( _line_desc->a_data1 & eGFTB_LD_FG_MASK ) >> eGFTB_LD_FG_OFFSET;
    _line_attr->a_bg        =   ( _line_desc->a_data1 & eGFTB_LD_BG_MASK ) >> eGFTB_LD_BG_OFFSET;
    _line_attr->a_st        =   ( _line_desc->a_data1 & eGFTB_LD_ST_MASK ) >> eGFTB_LD_ST_OFFSET;
    _line_attr->a_dis       =   ( _line_desc->a_data1 & eGFTB_LD_DS_MASK ) >> eGFTB_LD_DS_OFFSET;
    _line_attr->a_uis       =   ( _line_desc->a_data1 & eGFTB_LD_US_MASK ) >> eGFTB_LD_US_OFFSET;
}
static inline
void                gwr_fast_text_buffer_line_desc_from_attributes(
    GwrFastTextBufferLineDesc   *   _line_desc  ,
    GwrFastTextBufferLineAttr   *   _line_attr  )
{
    _line_desc->a_data1      =  ( ( (guint32)(_line_attr->a_fg)   << eGFTB_LD_FG_OFFSET ) & eGFTB_LD_FG_MASK );
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_bg)   << eGFTB_LD_BG_OFFSET ) & eGFTB_LD_BG_MASK );
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_st)   << eGFTB_LD_ST_OFFSET ) & eGFTB_LD_ST_MASK );
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_dis)  << eGFTB_LD_DS_OFFSET ) & eGFTB_LD_DS_MASK );
    _line_desc->a_data1     |=  ( ( (guint32)(_line_attr->a_uis)  << eGFTB_LD_US_OFFSET ) & eGFTB_LD_US_MASK );
}
inline
gboolean            gwr_fast_text_buffer_line_has_html(GwrFastTextBufferLine* _line)
{
    return ( _line->a_attr.a_uis != 0 );
}
inline
gboolean            gwr_fast_text_buffer_line_has_data(GwrFastTextBufferLine* _line)
{
    return ( _line->a_attr.a_dis != 0 );
}
//  ============================================================================
static inline
void                gwr_fast_text_buffer_segment_desc_from_attributes(
    GwrFastTextBufferSegmentDesc    *   _segment_desc   ,
    GwrFastTextBufferSegmentAttr    *   _segment_attr   )
{
}
//  ============================================================================
static inline
void                gwr_fast_text_buffer_xdu_desc_pack(
    GwrFastTextBufferXduDesc        *   _xdu_desc       ,
    guint16                             _url_offset     ,
    guint8                              _url_len        )
{
    _xdu_desc->a_data1       =  ( ( (guint16)(_url_offset)  << eGFTB_XDU_OF_OFFSET ) & eGFTB_XDU_OF_MASK );
    _xdu_desc->a_data1      |=  ( ( (guint16)(_url_len)     << eGFTB_XDU_LN_OFFSET ) & eGFTB_XDU_LN_MASK );
}
static inline
void                gwr_fast_text_buffer_xdu_desc_unpack(
    GwrFastTextBufferXduDesc        *   _xdu_desc       ,
    guint16                         *   _url_offset     ,
    guint8                          *   _url_len        )
{
    * _url_offset           =             ( _xdu_desc->a_data1 & eGFTB_XDU_OF_MASK ) >> eGFTB_XDU_OF_OFFSET;
    * _url_len              =   (guint8)( ( _xdu_desc->a_data1 & eGFTB_XDU_LN_MASK ) >> eGFTB_XDU_LN_OFFSET );
}
//  ============================================================================
//  ----------------------------------------------------------------------------
GwrFastTextBuffer*   gwr_fast_text_buffer_new(
        guint32                         _lines_text_data_block_size             ,
        guint32                         _lines_text_data_blocks_storage_realloc ,
        guint32                         _lines_text_info_blocks_storage_realloc ,

        guint32                         _lines_desc_infos_blocks_storage_realloc    ,

        guint32                         _extra_data_data_block_size             ,
        guint32                         _extra_data_data_blocks_storage_realloc ,
        guint32                         _extra_data_info_blocks_storage_realloc ,

        guint32                         _xd_url_info_blocks_storage_capacity    ,
        guint32                         _xd_url_info_blocks_storage_realloc     )
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

    p->d_lines_desc     =   gwr_array_equal_simple_new(
                                "GwrFastTextBuffer::line desciptions"       ,
                                sizeof(GwrFastTextBufferLineDesc)           ,
                                _lines_desc_infos_blocks_storage_realloc    );

    p->d_extra_data     =   gwr_array_data_multi_new(
                                "GwrFastTextBuffer:extra data",
                                _extra_data_data_block_size , _extra_data_data_blocks_storage_realloc   ,
                                                              _extra_data_info_blocks_storage_realloc   );

    p->d_xdu_offsets    =   gwr_array_equal_new(
                                "GwrFastTextBuffer:urls offsets",
                                sizeof(GwrFastTextBufferXduDesc)            ,
                                _xd_url_info_blocks_storage_capacity        ,
                                _xd_url_info_blocks_storage_realloc         );

    p->lines_max_len    =   0;

    b->priv             =   (gpointer)p;

    return b;
}
void                gwr_fast_text_buffer_reset            (GwrFastTextBuffer* _buffer)
{
    GwrFastTextBufferPrivate *   bp  =   NULL;
    //  ........................................................................
    bp                  =   (GwrFastTextBufferPrivate*)(_buffer->priv);

    gwr_array_data_multi_reset  ( bp->d_lines_text      );
    gwr_array_equal_simple_reset       ( bp->d_lines_desc      );
    gwr_array_data_multi_reset  ( bp->d_extra_data      );
    gwr_array_equal_reset       ( bp->d_xdu_offsets     );

    bp->lines_max_len       =   0;
    bp->lines_card          =   0;
}
//  ----------------------------------------------------------------------------
gboolean            gwr_fast_text_buffer_line_exist_and_has_data(
            GwrFastTextBuffer       *       _buffer         ,
            guint32                         _line_index     )
{
    GwrFastTextBufferPrivate    *   bp          =   (GwrFastTextBufferPrivate*)(_buffer->priv);
    GwrFastTextBufferLineDesc   *   line_desc   =   NULL;
    //  ........................................................................
    if ( _line_index < bp->lines_card )
    {
        line_desc = gwr_array_equal_simple_get_data( bp->d_lines_desc, _line_index);

        return ( ( line_desc->a_data1 & eGFTB_LD_DS_MASK ) != 0 );
    }

    return FALSE;
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

    gwr_array_data_multi_get_data( bp->d_lines_text, _line_index, &d_text );

    line_desc = gwr_array_equal_simple_get_data( bp->d_lines_desc, _line_index);
    gwr_fast_text_buffer_line_attributes_from_desc( &(_line->a_attr), line_desc );

    _line->a_str            =   d_text.a_mem;
    _line->a_str_len        =   d_text.a_size;

    _line->a_extra_data     =   NULL;
    _line->a_extra_data_len =   0;

    _line->a_ho             =   0;
    _line->a_hl             =   0;

    return TRUE;
}
//  ----------------------------------------------------------------------------
gboolean            gwr_fast_text_buffer_get_line_and_data(
    GwrFastTextBuffer       *   _buffer     ,
    guint32                     _line_index ,
    GwrFastTextBufferLine   *   _line       )
{
    GwrFastTextBufferPrivate    *   bp  =   NULL;
    GwrCData16                      d_text;
    GwrFastTextBufferLineDesc   *   line_desc;

    guint32                         di  =   0;
    GwrCData16                      extra_data;

    guint32                         hi  =   0;
    GwrFastTextBufferXduDesc    *   xdu_desc;
    //  ........................................................................
    bp  =   (GwrFastTextBufferPrivate*)_buffer->priv;

    if ( _line_index >= bp->lines_card )
        return FALSE;

    gwr_array_data_multi_get_data( bp->d_lines_text, _line_index, &d_text );    //  text

    line_desc = gwr_array_equal_simple_get_data( bp->d_lines_desc, _line_index);//  packed metadata

    gwr_fast_text_buffer_line_attributes_from_desc(
        &(_line->a_attr)    ,
        line_desc           );

    _line->a_str            =   d_text.a_mem;
    _line->a_str_len        =   d_text.a_size;

    _line->a_extra_data     =   NULL;
    _line->a_extra_data_len =   0;

    _line->a_ho             =   0;
    _line->a_hl             =   0;

    //  we have extra data
    if ( _line->a_attr.a_dis )
    {
        memcpy(                                                                 //  _GWR_REM_ endianness
            &( di )                             ,
            _line->a_str + _line->a_str_len     ,
            _line->a_attr.a_dis                 );

        //D printf("retrived data index:[%3i]\n", di);

        gwr_array_data_multi_get_data( bp->d_extra_data, di, &extra_data );

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

        //  we have html offsets
        if ( _line->a_attr.a_uis )
        {
            memcpy(                                                             //  _GWR_REM_ endianness
                &( hi )                                                     ,
                _line->a_str + _line->a_str_len + _line->a_attr.a_dis       ,
                _line->a_attr.a_uis                                         );

            xdu_desc    =   gwr_array_equal_get_data( bp->d_xdu_offsets, hi );
            gwr_fast_text_buffer_xdu_desc_unpack(
                xdu_desc        ,
                & _line->a_ho   ,
                & _line->a_hl   );

            //D printf("retrived url index:[%3i] [%3i,%3i]\n", hi, _line->a_ho, _line->a_hl);

            //_line->a_extra_data     =   extra_data.a_mem;
            //_line->a_extra_data_len =   extra_data.a_size;
        }
    }

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  gwr_fast_text_buffer_add_line()
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
    line_attr.a_dis     =   0;
    line_attr.a_uis     =   0;

    gwr_fast_text_buffer_line_desc_from_attributes(&line_desc, &line_attr);

    gwr_array_equal_simple_add_data( bp->d_lines_desc, &line_desc );
    //D printf("gwr_fast_text_buffer_add_line():adding line sa[%5i] su[%5i]\n",
    //D    bp->d_lines_desc->a_slots_card   ,
    //D    bp->d_lines_desc->a_slots_used   );

    gwr_array_data_multi_add_data   ( bp->d_lines_text, (gpointer)_text, (guint16)( (len) & 0x0000ffff) );
    //  ........................................................................
    //  update buffer infos
    if ( len > bp->lines_max_len )
        bp->lines_max_len = len;

    bp->lines_card          =   bp->d_lines_desc->a_slots_used;
}
//  ----------------------------------------------------------------------------
//  gwr_fast_text_buffer_add_line_with_data()
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_add_line_with_data(
            GwrFastTextBuffer   *   _buffer         ,
    const   gchar               *   _text           ,
            guint8                  _fg             ,
            guint8                  _bg             ,
            guint16                 _st             ,
            gpointer                _data           ,
            guint16                 _data_len       )
{
    GwrFastTextBufferPrivate    *   bp              =   NULL;

    GwrFastTextBufferLineDesc       line_desc;
    GwrFastTextBufferLineAttr       line_attr;

    char                            tmp[8];

    guint32                         len             =   0;
    //  ........................................................................
    //  init some vars
    bp   =   _buffer->priv;

    len  =   strlen(_text);

    g_return_if_fail( len <= 512 );

    line_attr.a_fg      =   _fg;
    line_attr.a_bg      =   _bg;
    line_attr.a_st      =   _st;
    gwr_fast_text_buffer_uint32_pack1(                                          //  tmp will contain packed data index,
        tmp                                                     ,               //  and a_dis will contain sizeof(tmp)
        (guint32)bp->d_extra_data->d_info_blocks->a_slots_used  ,
        & line_attr.a_dis                                       );
    line_attr.a_uis     =   0;

    gwr_array_data_multi_add_data_and_extra_data(
        bp->d_lines_text                    ,
        (gpointer)_text                     ,
        (guint16)( (len) & 0x0000ffff)      ,
        tmp                                 ,
        line_attr.a_dis                     );

    gwr_fast_text_buffer_line_desc_from_attributes(&line_desc, &line_attr);
    gwr_array_equal_simple_add_data( bp->d_lines_desc, &line_desc );

    gwr_array_data_multi_add_data(
        bp->d_extra_data        ,
        (gpointer)_data         ,
        _data_len               );
    //  ........................................................................
    //  update buffer infos
    if ( len > bp->lines_max_len )
        bp->lines_max_len = len;

    bp->lines_card          =   bp->d_lines_desc->a_slots_used;
}
//  ----------------------------------------------------------------------------
//  gwr_fast_text_buffer_add_line_with_data_and_url()
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_add_line_with_data_and_url(
            GwrFastTextBuffer   *   _buffer         ,
    const   gchar               *   _text           ,
            guint8                  _fg             ,
            guint8                  _bg             ,
            guint16                 _st             ,
            gpointer                _data           ,
            guint16                 _data_len       ,
            guint16                 _url_offset     ,
            guint8                  _url_len        )
{
    GwrFastTextBufferPrivate    *   bp              =   NULL;

    GwrFastTextBufferLineDesc       line_desc;
    GwrFastTextBufferLineAttr       line_attr;

    GwrFastTextBufferXduDesc        xdu_desc;

    char                            tmp[8];

    guint32                         len             =   0;
    //  ........................................................................
    //  init some vars
    bp   =   _buffer->priv;

    len  =   strlen(_text);

    g_return_if_fail( len <= 512 );

    line_attr.a_fg      =   _fg;
    line_attr.a_bg      =   _bg;
    line_attr.a_st      =   _st;
    gwr_fast_text_buffer_uint32_pack2(                                          //  tmp will contain packed datas indexes,
        tmp                                                     ,               //  and  :
        (guint32)bp->d_extra_data->d_info_blocks->a_slots_used  ,               //      a_dis will contain sizeof(tmp pack 1)
        (guint32)bp->d_xdu_offsets->a_data_card                 ,               //      a_uis will contain sizeof(tmp pack 2)
        & line_attr.a_dis                                       ,
        & line_attr.a_uis                                       );

    //D printf("stored url index:[%3i] [%3i,%3i]\n", bp->d_xdu_offsets->a_data_card, line_attr.a_dis, line_attr.a_uis);

    gwr_array_data_multi_add_data_and_extra_data(
        bp->d_lines_text                    ,
        (gpointer)_text                     ,
        (guint16)( (len) & 0x0000ffff)      ,
        tmp                                 ,
        line_attr.a_dis + line_attr.a_uis   );

    gwr_fast_text_buffer_line_desc_from_attributes( &line_desc, &line_attr );
    gwr_array_equal_simple_add_data( bp->d_lines_desc, &line_desc );

    gwr_array_data_multi_add_data(                                              //  data
        bp->d_extra_data        ,
        (gpointer)_data         ,
        _data_len               );

    gwr_fast_text_buffer_xdu_desc_pack( &xdu_desc, _url_offset, _url_len );
    gwr_array_equal_add_data( bp->d_xdu_offsets, & xdu_desc );                  //  html offsets
    //  ........................................................................
    //  update buffer infos
    if ( len > bp->lines_max_len )
        bp->lines_max_len = len;

    bp->lines_card          =   bp->d_lines_desc->a_slots_used;
}









//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_get_stats(
        GwrFastTextBuffer        *       _buffer ,
        GwrFastTextBufferStat    *       _s      )
{
    GwrFastTextBufferPrivate *   bp  =   NULL;
    //  ........................................................................
    bp  =   (GwrFastTextBufferPrivate*)_buffer->priv;

    gwr_array_data_multi_get_stats      ( bp->d_lines_text,     &( _s->ld ) );
    gwr_array_equal_simple_get_stats    ( bp->d_lines_desc,     &( _s->li ) );
    gwr_array_data_multi_get_stats      ( bp->d_extra_data,     &( _s->lx ) );
    gwr_array_equal_get_stats           ( bp->d_xdu_offsets,    &( _s->lu ) );
}
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_dump_mem(
        GwrFastTextBuffer       *       _buffer )
{
    GwrFastTextBufferPrivate    *   bp      =   NULL;
    GwrFastTextBufferStat           sftb;
    GwrCArrayDataMultiStat      *   sadm    =   NULL;
    GwrCArrayEqualSimpleStat    *   saesq   =   NULL;
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

    saesq   =   &( sftb.li );                                                   //  info
    //sprintf(text, "Info     :using [%6i] blocks of [%6i] bytes",
    //    saeq->a_simple_stat.a_slots_used    ,
    //    saeq->a_dbk_size                    );
    //gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
    sprintf(text, "Info:mem :using [%6i] bytes  of [%6i] for data storage",
        saesq->a_mfp.a_su ,
        saesq->a_mfp.a_sa );
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

    saeq    =   &( sftb.lu );                                                   //  urls
    sprintf(text, "Urls:    :using [%6i] blocks of [%6i] bytes",
        saeq->a_simple_stat.a_slots_used    ,
        saeq->a_dbk_size                    );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
    sprintf(text, "    :mem :using [%6i] bytes  of [%6i] for data storage",
        saeq->a_mfp.a_su ,
        saeq->a_mfp.a_sa );
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );

    sprintf(text, "--------------------------------------------------------------");
    gwr_fast_text_buffer_add_line(_buffer, text, 0, 0, 0 );
}













/*
//  ----------------------------------------------------------------------------
//  gwr_fast_text_buffer_segmented_line_start()
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_segmented_line_start(
            GwrFastTextBuffer   *   _buffer         ,
            guint8                  _fg             ,
            guint8                  _bg             ,
            guint16                 _st             ,
    const   gchar*                  _text           )
{
    GwrFastTextBufferPrivate    *   bp              =   NULL;

    GwrFastTextBufferSegmentAttr    seg_attr;
    GwrFastTextBufferSegmentDesc    seg_desc;

    guint32                         len             =   strlen(_text);
    //  ........................................................................
    //  init some vars
    bp   =   _buffer->priv;
    //  ........................................................................
    g_return_if_fail( ! bp->segments.a_state );
    g_return_if_fail( len <= 512 );
    //  ........................................................................
    //  segment descriptor
    seg_attr.a_fg   =   _fg;
    seg_attr.a_bg   =   _bg;
    seg_attr.a_st   =   _st;

    seg_attr.a_of   =   0;
    seg_attr.a_ln   =   len & 0x0000ffff;

    gwr_fast_text_buffer_segment_desc_from_attributes( &seg_desc, &seg_attr );

    bp->segments.a_descs[0] = seg_desc.a_data1;                                 //  memorize the sd
    //  ........................................................................
    //  segment text
    memcpy( bp->segments.a_line, _text, len );
    bp->segments.a_line_len =   len;
    //  ........................................................................
    bp->segments.a_state    =   1;
}
//  ----------------------------------------------------------------------------
//  gwr_fast_text_buffer_segmented_line_add()
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_segmented_line_add(
            GwrFastTextBuffer   *   _buffer         ,
            guint8                  _fg             ,
            guint8                  _bg             ,
            guint16                 _st             ,
    const   gchar*                  _text           )
{
    GwrFastTextBufferPrivate    *   bp              =   NULL;

    GwrFastTextBufferSegmentAttr    seg_attr;
    GwrFastTextBufferSegmentDesc    seg_desc;

    guint32                         len             =   strlen(_text);
    guint32                         state;
    //  ........................................................................
    //  init some vars
    bp      =   _buffer->priv;
    state   =   bp->segments.a_state;
    //  ........................................................................
    g_return_if_fail( state                                 );
    g_return_if_fail( bp->segments.a_line_len + len <= 512  );
    //  ........................................................................
    //  segment descriptor
    seg_attr.a_fg   =   _fg;
    seg_attr.a_bg   =   _bg;
    seg_attr.a_st   =   _st;

    seg_attr.a_of   =   seg_attr.a_ln;
    seg_attr.a_ln   =   len & 0x0000ffff;

    gwr_fast_text_buffer_segment_desc_from_attributes( &seg_desc, &seg_attr );

    bp->segments.a_descs[state] = seg_desc.a_data1;                             //  memorize the sd
    //  ........................................................................
    //  segment text
    memcpy( bp->segments.a_line + bp->segments.a_line_len, _text, len );
    bp->segments.a_line_len =   bp->segments.a_line_len + len;
    //  ........................................................................
    //  state
    bp->segments.a_state    =   bp->segments.a_state + 1;
}
//  ----------------------------------------------------------------------------
//  gwr_fast_text_buffer_segmented_line_add()
//  ----------------------------------------------------------------------------
void                gwr_fast_text_buffer_segmented_line_end(
            GwrFastTextBuffer   *   _buffer         ,
            gpointer                _extra_data     ,
            guint16                 _extra_data_len ,
            guint8                  _fg             ,
            guint8                  _bg             ,
            guint16                 _st             ,
    const   gchar*                  _text           )
{
    GwrFastTextBufferPrivate    *   bp              =   NULL;

    GwrFastTextBufferSegmentAttr    seg_attr;
    GwrFastTextBufferSegmentDesc    seg_desc;

    guint32                         len             =   strlen(_text);
    guint32                         state;
    //  ........................................................................
    //  init some vars
    bp      =   _buffer->priv;
    state   =   bp->segments.a_state;
    //  ........................................................................
    g_return_if_fail( state || ( state < 8 )                );
    g_return_if_fail( bp->segments.a_line_len + len <= 512  );
    //  ........................................................................
    //  segment descriptor
    seg_attr.a_fg   =   _fg;
    seg_attr.a_bg   =   _bg;
    seg_attr.a_st   =   _st;

    seg_attr.a_of   =   seg_attr.a_ln;
    seg_attr.a_ln   =   len & 0x0000ffff;

    gwr_fast_text_buffer_segment_desc_from_attributes( &seg_desc, &seg_attr );

    bp->segments.a_descs[state] = seg_desc.a_data1;                             //  memorize the sd
    //  ........................................................................
    //  segment text
    memcpy( bp->segments.a_line + bp->segments.a_line_len, _text, len );
    bp->segments.a_line_len =   bp->segments.a_line_len + len;
    //  ........................................................................
    //  state
    bp->segments.a_state    =   0;
    bp->segments.a_line_len =   0;
    //  ........................................................................
    //  line
    //gwr_fast_text_buffer_add_line
}
*/








