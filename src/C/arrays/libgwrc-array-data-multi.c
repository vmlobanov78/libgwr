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
//  ----------------------------------------------------------------------------
//  Gwr_array_data_multi_ensure_first_block_exists_and_is_used()
//  ----------------------------------------------------------------------------
//! \fn     Gwr_array_data_multi_ensure_first_block_exists_and_is_used()
//!
//! \brief  IMPORTANT : Called _ONLY_ after gwr_array_data_multi_new() or
//!     gwr_array_data_multi_reset(). It does _NOT_ make sense to call this
//!     function from anyware else.
static
void
Gwr_array_data_multi_ensure_first_block_exists(
        GwrCArrayDataMulti          *   _adm            )
{
    GwrCADBlock24           dbk24;
    //  ........................................................................
    //  slot(s) are in use : the first slot exist
    if ( _adm->d_data_blocks->a_slots_used )
        return;

    //  no slot in use, but first block exist : mark as used
    if ( _adm->d_data_blocks->a_slots_card )
    {
        _adm->d_data_blocks->a_slots_used = 1;
        return;
    }

    //  no block exists : create one block and add it
    gwr_array_dbk24_alloc           ( &dbk24                , _adm->a_data_block_size );
    gwr_array_equal_simple_add_data ( _adm->d_data_blocks   , &dbk24 );
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
    const   gchar           *               _name                           ,
            guint32                         _data_block_size                ,
            guint32                         _data_blocks_storage_realloc    ,
            guint32                         _info_blocks_storage_realloc    )
{
    GwrCArrayDataMulti      *   adm =   NULL;
    //  ........................................................................
    //  create  GwrCArrayDataMulti structure
    adm                 =   (GwrCArrayDataMulti*)g_new0( GwrCArrayDataMulti , 1 );

    adm->d_name         =   g_strdup(_name);

    adm->a_data_block_size  =   _data_block_size;

    adm->a_data_size    =   0;
    //  ........................................................................
    //  create  array of GwrCADBlock s and GwrCADMDataInfo
    adm->d_data_blocks  =   gwr_array_equal_simple_new( "GwrCArrayDataMulti::datas" , GwrCADBlock24_SSIZE   , _data_blocks_storage_realloc );
    adm->d_info_blocks  =   gwr_array_equal_simple_new( "GwrCArrayDataMulti::infos" , GwrCADMDataInfo_SSIZE , _info_blocks_storage_realloc );

    Gwr_array_data_multi_ensure_first_block_exists(adm);

    return adm;
}
void
gwr_array_data_multi_delete(
        GwrCArrayDataMulti          *   _adm            )
{
    guint32             i       =   0;
    GwrCADBlock24   *   dbk24   =   NULL;
    //  ........................................................................
    //  the GwrCADBlock24 structures themseleves are not allocated, but the data
    //  areas they point to are.
    for ( i = 0 ; i != _adm->d_data_blocks->a_slots_used ; i++ )
    {
        dbk24 = gwr_array_equal_simple_get_data( _adm->d_data_blocks, i );
        if ( ! dbk24 )
        {
            printf("***ERR *** gwr_array_data_multi_delete():Block [%i] could not be obtained\n", i);
            continue;
        }

        gwr_array_dbk24_dealloc( dbk24 );
    }

    gwr_array_equal_simple_delete( _adm->d_data_blocks );
    gwr_array_equal_simple_delete( _adm->d_info_blocks );

    g_free(_adm->d_name);

    g_free( _adm );
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_reset()
//  ----------------------------------------------------------------------------
void
gwr_array_data_multi_reset(
        GwrCArrayDataMulti      *       _adm        )
{
    guint32             i       =   0;
    GwrCADBlock24   *   dbk24   =   NULL;
    //  ........................................................................
    for ( i = 0 ; i != _adm->d_data_blocks->a_slots_used ; i++ )
    {
        dbk24 = gwr_array_equal_simple_get_data( _adm->d_data_blocks, i );
        if ( ! dbk24 )
        {
            printf("***ERR *** gwr_array_data_multi_reset():Block [%i] could not be obtained\n", i);
            continue;
        }

        gwr_array_dbk24_reset( dbk24 );
    }
    //  ........................................................................
    gwr_array_equal_simple_reset( _adm->d_data_blocks );
    gwr_array_equal_simple_reset( _adm->d_info_blocks );

    _adm->a_data_size   =   0;

    Gwr_array_data_multi_ensure_first_block_exists(_adm);
}
//  ----------------------------------------------------------------------------
#include    "libgwrc-array-data-multi-infos.cci"
#include    "libgwrc-array-data-multi-core.cci"
