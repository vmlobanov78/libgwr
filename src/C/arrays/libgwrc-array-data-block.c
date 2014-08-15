/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-data-block.c                                              *
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
    *   Purpose :   Memory region containing data.                              *
    *                                                                           *
    *****************************************************************************
*/

#include    "libgwrc-array-common.h"
#include    "libgwrc-array-data-block.h"

//  ----------------------------------------------------------------------------
//  gwr_array_data_block_new()
//  ----------------------------------------------------------------------------
GwrCADBlock *
gwr_array_data_block_new(
        guint32                         _block_size   )
{
    GwrCADBlock             *   b   =   NULL;
    //  ........................................................................
    //  create a GwrCADBlock  struct
    b                   =   (GwrCADBlock *)g_new0( GwrCADBlock  , 1 );

    gwr_array_data_block_alloc( b, _block_size );

    return b;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_block_alloc()
//  ----------------------------------------------------------------------------
void
gwr_array_data_block_alloc(
        GwrCADBlock                 *   _block          ,
        guint32                         _block_size     )
{
    //  ........................................................................
    _block->d_mem           =   g_malloc0( _block_size );
    _block->a_size          =   _block_size;
    _block->a_used_bytes    =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_block_dealloc()
//  ----------------------------------------------------------------------------
void
gwr_array_data_block_dealloc(
        GwrCADBlock                 *   _block  )
{
    //  ........................................................................
    g_free( _block->d_mem );
    _block->a_size  =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_block_reset()
//  ----------------------------------------------------------------------------
void
gwr_array_data_block_reset(
        GwrCADBlock                 *   _block  )
{
    //  ........................................................................
    _block->a_used_bytes    =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_block_available_bytes()
//  ----------------------------------------------------------------------------
inline
guint32
gwr_array_data_block_available_bytes(
        GwrCADBlock                 *   _block          )
{
    return _block->a_size - _block->a_used_bytes;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_block_add()
//  ----------------------------------------------------------------------------
void
gwr_array_data_block_add(
        GwrCADBlock                 *   _block          ,
        gpointer                        _data           ,
        guint32                         _data_size      )
{
    //  ........................................................................
    memcpy( _block->d_mem + _block->a_used_bytes, _data, _data_size );

    _block->a_used_bytes    =   _block->a_used_bytes    +   _data_size;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_block_add_with_extra_data_index()
//  ----------------------------------------------------------------------------
void
gwr_array_data_block_add_with_extra_data_index(
        GwrCADBlock                 *   _block              ,
        gpointer                        _data               ,
        guint32                         _data_size          ,
        guint32                         _extra_data_index   ,
        guint8                      *   _xdi_len            )
{
    //  ........................................................................
    memcpy( _block->d_mem + _block->a_used_bytes                , _data                 , _data_size        );
    memcpy( _block->d_mem + _block->a_used_bytes + _data_size   , &(_extra_data_index)  , sizeof(guint32)   );

    _block->a_used_bytes    =   _block->a_used_bytes    +   _data_size  +   sizeof(guint32);

    *_xdi_len               =   4;
}
//  ----------------------------------------------------------------------------
//  gwr_array_data_block_dump()
//  ----------------------------------------------------------------------------
void
gwr_array_data_block_dump(
        GwrCADBlock                 *   _block          )
{
    printf("GwrCADBlock   :[%p] mem[%p] siz[%06i] usd[%06i] ava[%06i]\n"           ,
        _block                                                  ,
        _block->d_mem,  _block->a_size, _block->a_used_bytes    ,
        gwr_array_data_block_available_bytes( _block)     );
}
