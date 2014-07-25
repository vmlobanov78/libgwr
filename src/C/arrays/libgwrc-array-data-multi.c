/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-data-multi.c                                              *
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
    *   Purpose :   Array containing datas of any sizes.                        *
    *                                                                           *
    *****************************************************************************
*/
                                                                                /*
_OPTIM_01_ :
----------
    When adding data, we dont check is a first Block exits. Instead, we ensure
    there is always at least one block in GwrCArrayDataMulti->d_blocks.
                                                                                */

#include    <stdio.h>
#include    <string.h>

#include    "libgwrc-array-data-multi.h"

//  ****************************************************************************
//  STATIC
//  ****************************************************************************
//  ============================================================================
//  Block
//  ============================================================================
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_block_alloc()
//  ----------------------------------------------------------------------------
static
void
gwr_array_data_multi_block_alloc(
        GwrCADMBlock                *   _block          ,
        guint32                         _block_size     )
{
    //  ........................................................................
    _block->d_mem           =   g_malloc0( _block_size );
    _block->a_size          =   _block_size;
    _block->a_used_bytes    =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_block_new()
//  ----------------------------------------------------------------------------
static
GwrCADMBlock*
gwr_array_data_multi_block_new(
        guint32                         _block_size   )
{
    GwrCADMBlock            *   b   =   NULL;
    //  ........................................................................
    //  create a GwrCADMBlock struct
    b                   =   (GwrCADMBlock*)g_new0( GwrCADMBlock , 1 );

    gwr_array_data_multi_block_alloc( b, _block_size );

    return b;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_block_dealloc()
//  ----------------------------------------------------------------------------
static
void
gwr_array_data_multi_block_dealloc(
        GwrCADMBlock                *   _block  )
{
    //  ........................................................................
    g_free( _block->d_mem );
    _block->a_size  =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_block_reset()
//  ----------------------------------------------------------------------------
static
void
gwr_array_data_multi_block_reset(
        GwrCADMBlock                *   _block  )
{
    //  ........................................................................
    _block->a_used_bytes    =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_block_available_bytes()
//  ----------------------------------------------------------------------------
static inline
guint32
gwr_array_data_multi_block_available_bytes(
        GwrCADMBlock                *   _block          )
{
    return _block->a_size - _block->a_used_bytes;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_block_add()
//  ----------------------------------------------------------------------------
static
void
gwr_array_data_multi_block_add(
        GwrCADMBlock                *   _block          ,
        gpointer                        _data           ,
        guint32                         _data_size      )
{
    //  ........................................................................
    memcpy( _block->d_mem + _block->a_used_bytes, _data, _data_size );

    _block->a_used_bytes    =   _block->a_used_bytes    +   _data_size;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_block_dump()
//  ----------------------------------------------------------------------------
static
void
gwr_array_data_multi_block_dump(
        GwrCADMBlock                *   _block          )
{
    printf("GwrCADMBlock:[%p] m=[%p] s=%06i u=%06i a=%06i\n"           ,
        _block                                                  ,
        _block->d_mem,  _block->a_size, _block->a_used_bytes    ,
        gwr_array_data_multi_block_available_bytes( _block)     );
}
//  ============================================================================
//  DataInfo
//  ============================================================================
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_data_info_set()
//  ----------------------------------------------------------------------------
static
void
gwr_array_data_multi_data_info_set(
        GwrCADMDataInfo             *   _admdi          ,
        guint16                         _block_index    ,
        guint32                         _data_offset    ,
        guint16                         _data_len       )
{
    _admdi->a_block_index   =   _block_index;
    _admdi->a_offset        =   _data_offset;
    _admdi->a_len           =   _data_len;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_data_info_dump()
//  ----------------------------------------------------------------------------
static
void
gwr_array_data_multi_data_info_dump(
        GwrCADMDataInfo             *   _data_info      )
{
    printf("GwrCADMDataInfo:[%p] i=%06i o=%06i l=%06i\n"    ,
        _data_info                  ,
        _data_info->a_block_index   ,
        _data_info->a_offset        ,
        _data_info->a_len           );
}
//  ============================================================================
//  GwrCArrayDataMulti
//  ============================================================================
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_ensure_first_block_exists()
//  ----------------------------------------------------------------------------
void                                                                            //  _OPTIM_01_
gwr_array_data_multi_ensure_first_block_exists(
        GwrCArrayDataMulti      *       _adm        )
{
    GwrCADMBlock                b;
    //  ........................................................................
    //  if blocks are in use, at least one block exist : nothing to do
    if ( _adm->d_blocks->a_blocks_used )
        return;

    //  if no block is in use, but first blocks exists, indicate
    //  that it is in use
    if ( _adm->d_blocks->a_blocks_card )
    {
        _adm->d_blocks->a_blocks_used   =   1;
        return;
    }

    //  else create first block and add it
    gwr_array_data_multi_block_alloc( &b, _adm->a_block_size );
    gwr_array_equal_add( _adm->d_blocks, &b );
}
//  ****************************************************************************
//  PUBLIC
//  ****************************************************************************
guint32     GwrCADMBlock_S          =   sizeof(GwrCADMBlock);
guint32     GwrCADMDataInfo_S       =   sizeof(GwrCADMDataInfo);
//  ============================================================================
//  ArrayDataMulti
//  ============================================================================
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_dump()
//  ----------------------------------------------------------------------------
void
gwr_array_data_multi_dump(
        GwrCArrayDataMulti  *           _adm            )
{
    guint32                     i   =   0;
    GwrCADMBlock            *   b   =   NULL;
    GwrCADMDataInfo         *   di  =   NULL;

    guint32                     s1  =   0;
    guint32                     s2  =   0;
    //  ........................................................................
    printf("+---------------------------+\n");
    printf("| GwrCArrayDataMulti:dump() |\n");
    printf("+---------------------------+\n");

    printf("--- Dumping blocks:\n");
    gwr_array_equal_dump( _adm->d_blocks );
    for ( i = 0 ; i != _adm->d_blocks->a_blocks_used ; i++ )
    {
        b = gwr_array_equal_get( _adm->d_blocks, i );
        if ( ! b )
        {
            printf("ERR:Block [%i] could not be obtained\n", i);
            continue;
        }

        gwr_array_data_multi_block_dump( b );
        s1  =   s1  +   b->a_used_bytes;
    }

    printf("--- Dumping data infos:\n");
    gwr_array_equal_dump( _adm->d_infos );
    for ( i = 0 ; i != _adm->d_infos->a_blocks_used ; i++ )
    {
        di = gwr_array_equal_get( _adm->d_infos, i );
        if ( ! di )
        {
            printf("ERR:DataIifo [%i] could not be obtained\n", i);
            continue;
        }

        gwr_array_data_multi_data_info_dump( di );
        s2  =   s2  +   di->a_len;
    }

    printf("--- Data size:\n");
    printf("Blocks        : %06i\n", s1);
    printf("DataInfos     : %06i\n", s2);
    printf("ArrayDataMulti: %06i\n", _adm->a_data_size);
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_new()
//  ----------------------------------------------------------------------------
GwrCArrayDataMulti*
gwr_array_data_multi_new(
        guint32                         _data_block_size    ,
        guint32                         _blocks_realloc     ,
        guint32                         _infos_realloc      )
{
    GwrCArrayDataMulti      *   a   =   NULL;
    //  ........................................................................
    //  create  GwrCArrayDataMulti structure
    a                   =   (GwrCArrayDataMulti*)g_new0( GwrCArrayDataMulti , 1 );

    a->d_blocks         =   NULL;
    a->d_infos          =   NULL;
    a->a_block_size     =   _data_block_size;
    a->a_data_size      =   0;
    //  ........................................................................
    //  create  array of GwrCADMBlocks and GwrCADMDataInfo
    a->d_blocks         =   gwr_array_equal_new( GwrCADMBlock_S     , _blocks_realloc   );
    a->d_infos          =   gwr_array_equal_new( GwrCADMDataInfo_S  , _infos_realloc    );
    //  ........................................................................
    gwr_array_data_multi_ensure_first_block_exists( a );                        //  _OPTIM_01_

    return a;
}
void
gwr_array_data_multi_delete(
        GwrCArrayDataMulti          *   _adm            )
{
    guint32             i   =   0;
    GwrCADMBlock    *   b   =   NULL;
    //  ........................................................................
    //  the blocks structures themseleves are not allocated, but the data
    //  areas are.
    for ( i = 0 ; i != _adm->d_blocks->a_blocks_used ; i++ )
    {
        b = gwr_array_equal_get( _adm->d_blocks, i );
        if ( ! b )
        {
            printf("ERR:Block [%i] could not be obtained\n", i);
            continue;
        }

        gwr_array_data_multi_block_dealloc( b );
    }

    gwr_array_equal_delete( _adm->d_blocks );
    gwr_array_equal_delete( _adm->d_infos  );
    g_free( _adm );
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_add_data()
//  ----------------------------------------------------------------------------
void
gwr_array_data_multi_reset(
        GwrCArrayDataMulti      *       _adm        )
{
    guint32             i   =   0;
    GwrCADMBlock    *   b   =   NULL;
    //  ........................................................................
    for ( i = 0 ; i != _adm->d_blocks->a_blocks_used ; i++ )
    {
        b = gwr_array_equal_get( _adm->d_blocks, i );
        if ( ! b )
        {
            printf("ERR:Block [%i] could not be obtained\n", i);
            continue;
        }

        gwr_array_data_multi_block_reset( b );
    }
    //  ........................................................................
    gwr_array_equal_reset( _adm->d_blocks );
    gwr_array_equal_reset( _adm->d_infos  );

    _adm->a_data_size   =   0;

    gwr_array_data_multi_ensure_first_block_exists( _adm );                     //  _OPTIM_01_
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_add_data()
//  ----------------------------------------------------------------------------
gboolean
gwr_array_data_multi_add_data(
        GwrCArrayDataMulti          *   _adm        ,
        gpointer                        _data       ,
        guint16                         _data_len   )
{
    GwrCADMBlock        *   admb_candidate;
    GwrCADMBlock            admb_new;
    GwrCADMDataInfo         admdi_new;
    //  ........................................................................
    //  choose block to add data
    admb_candidate = gwr_array_equal_get(                                       //  get last block in use
                        _adm->d_blocks                      ,
                        _adm->d_blocks->a_blocks_used - 1   );                  //  _OPTIM_01_

    //  block full ? create another
    if ( gwr_array_data_multi_block_available_bytes(admb_candidate) < _data_len )
    {
        gwr_array_data_multi_block_alloc( &admb_new, _adm->a_block_size  );
        if ( ! gwr_array_equal_add( _adm->d_blocks, &admb_new ) )
        {
            gwr_array_data_multi_block_dealloc( &admb_new );
            return FALSE;
        }

        admb_candidate = gwr_array_equal_get(                                   //  get last block in use = admb_new
                            _adm->d_blocks                      ,
                            _adm->d_blocks->a_blocks_used - 1   );
    }
    //  ........................................................................
    //  create new data info
    gwr_array_data_multi_data_info_set(
        &admdi_new                          ,
        _adm->d_blocks->a_blocks_used - 1   ,
        admb_candidate->a_used_bytes        ,
        _data_len                           );

    if ( ! gwr_array_equal_add( _adm->d_infos, &admdi_new ) )
    {
        return FALSE;
    }
    //  ........................................................................
    //  add data to the block
    gwr_array_data_multi_block_add( admb_candidate, _data, _data_len );
    //  ........................................................................
    //  update infos
    _adm->a_data_size   =   _adm->a_data_size   +   _data_len;
    return TRUE;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_get_data()
//  ----------------------------------------------------------------------------
gboolean
gwr_array_data_multi_get_data(
        GwrCArrayDataMulti      *       _adm            ,
        guint32                         _data_index     ,
        GwrCADMData             *       _data           )
{
    GwrCADMDataInfo         *   di  =   NULL;
    GwrCADMBlock            *   b   =   NULL;
    //  ........................................................................
    di = gwr_array_equal_get( _adm->d_infos, _data_index );
    g_return_val_if_fail( di, FALSE );

    _data->a_size   =   di->a_len;

    b = gwr_array_equal_get( _adm->d_blocks, di->a_block_index );
    g_return_val_if_fail( b, FALSE );

    _data->a_mem    =   b->d_mem    +   di->a_offset;

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_get_stats_alloc()
//  ----------------------------------------------------------------------------
void
gwr_array_data_multi_get_stats(
        GwrCArrayDataMulti      *       _adm                        ,
        GwrCArrayDataMultiStat  *       _adm_stat                   )
{
    _adm_stat->a_blocks_card    =   _adm->d_blocks->a_blocks_card;
    _adm_stat->a_blocks_used    =   _adm->d_blocks->a_blocks_used;
    _adm_stat->a_blocks_alloc   =   _adm->d_blocks->a_stat_realloc;

    _adm_stat->a_descs_card     =   _adm->d_infos->a_blocks_card;
    _adm_stat->a_descs_used     =   _adm->d_infos->a_blocks_used;
    _adm_stat->a_descs_alloc    =   _adm->d_infos->a_stat_realloc;

    _adm_stat->a_data_card      =   _adm->d_infos->a_blocks_used;
    _adm_stat->a_data_size      =   _adm->a_data_size;
}
