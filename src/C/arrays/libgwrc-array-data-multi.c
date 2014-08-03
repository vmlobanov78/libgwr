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

#include    "libgwrc-array-data-block.h"
#include    "libgwrc-array-data-multi.h"

//  ****************************************************************************
//  STATIC
//  ****************************************************************************
static
void
Gwr_array_data_multi_ensure_first_block_exists(
        GwrCArrayDataMulti          *   _adm            )
{
    GwrCADBlock24           b;
    //  ........................................................................
    //  if blocks are in use, at least one block exist : nothing to do
    if ( _adm->d_blocks->a_data_card )
        return;

    //  else create first block and add it
    gwr_array_dbk24_alloc   ( &b            , _adm->a_blocks_size );
    gwr_array_equal_add_data( _adm->d_blocks, &b );
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
    printf("GwrCADMDataInfo     :[%p] idx[%06i] off[%06i] len[%06i]\n"    ,
        _data_info                  ,
        _data_info->a_block_index   ,
        _data_info->a_offset        ,
        _data_info->a_len           );
}
//  ============================================================================
//  GwrCArrayDataMulti
//  ============================================================================

//  ****************************************************************************
//  PUBLIC
//  ****************************************************************************
guint32     GwrCADMDataInfo_SSIZE   =   sizeof(GwrCADMDataInfo);
//  ============================================================================
//  ArrayDataMulti
//  ============================================================================
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_new()
//  ----------------------------------------------------------------------------
GwrCArrayDataMulti*
gwr_array_data_multi_new(
            guint32                         _blocks_size                ,
            guint32                         _blocks_storage_capacity    ,
            guint32                         _blocks_storage_realloc     ,
            guint32                         _infos_storage_capacity     ,
            guint32                         _infos_storage_realloc      )
{
    GwrCArrayDataMulti      *   a   =   NULL;
    //  ........................................................................
    //  create  GwrCArrayDataMulti structure
    a                   =   (GwrCArrayDataMulti*)g_new0( GwrCArrayDataMulti , 1 );

    a->d_blocks         =   NULL;
    a->d_infos          =   NULL;

    a->a_blocks_size    =   _blocks_size;
    a->a_data_size      =   0;
    //  ........................................................................
    //  create  array of GwrCADBlock s and GwrCADMDataInfo
    a->d_blocks         =   gwr_array_equal_new( "GwrCArrayDataMulti::data"     , GwrCADBlock24_SSIZE   , _blocks_storage_capacity , _blocks_storage_realloc    );
    a->d_infos          =   gwr_array_equal_new( "GwrCArrayDataMulti::infos"    , GwrCADMDataInfo_SSIZE ,  _infos_storage_capacity , _infos_storage_realloc     );

    Gwr_array_data_multi_ensure_first_block_exists(a);

    return a;
}
void
gwr_array_data_multi_delete(
        GwrCArrayDataMulti          *   _adm            )
{
    guint32             i   =   0;
    GwrCADBlock24   *   b   =   NULL;
    //  ........................................................................
    //  the GwrCADBlock24 structures themseleves are not allocated, but the data
    //  areas they point to are.
    for ( i = 0 ; i != _adm->d_blocks->a_data_card ; i++ )                      //  _GWR_TODO_ inline this with a function
    {
        b = gwr_array_equal_get_data( _adm->d_blocks, i );
        if ( ! b )
        {
            printf("ERR:Block [%i] could not be obtained\n", i);
            continue;
        }

        gwr_array_dbk24_dealloc( b );
    }

    gwr_array_equal_delete( _adm->d_blocks );
    gwr_array_equal_delete( _adm->d_infos  );
    g_free( _adm );
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_reset()
//  ----------------------------------------------------------------------------
void
gwr_array_data_multi_reset(
        GwrCArrayDataMulti      *       _adm        )
{
    guint32             i   =   0;
    GwrCADBlock24   *   b   =   NULL;
    //  ........................................................................
    for ( i = 0 ; i != _adm->d_blocks->a_data_card ; i++ )
    {
        b = gwr_array_equal_get_data( _adm->d_blocks, i );
        if ( ! b )
        {
            printf("ERR:Block [%i] could not be obtained\n", i);
            continue;
        }

        gwr_array_dbk24_reset( b );
    }
    //  ........................................................................
    gwr_array_equal_reset( _adm->d_blocks );
    gwr_array_equal_reset( _adm->d_infos  );

    _adm->a_data_size   =   0;

    Gwr_array_data_multi_ensure_first_block_exists(_adm);
}
//  ----------------------------------------------------------------------------
#include    "libgwrc-array-data-multi-infos.cci"
#include    "libgwrc-array-data-multi-core.cci"
