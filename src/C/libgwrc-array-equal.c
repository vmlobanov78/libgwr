/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-equal.c                                                   *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
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
    *   Purpose : Array of constant-size data                                   *
    *                                                                           *
    *****************************************************************************
*/

#include    <stdio.h>
#include    <string.h>

#include    "C/libgwrc-array-equal.h"

//  ****************************************************************************
//  STATIC
//  ****************************************************************************
static
gboolean
gwr_array_array_equal_realloc(
        GwrCArrayEqual          *       _ae )
{
    gpointer    v   =   NULL;
    //  ........................................................................
    g_return_val_if_fail( _ae->a_blocks_card == _ae->a_blocks_used, FALSE );    //  ensure array is full

    v = g_malloc0( _ae->a_block_size *                                          //  alloc new array
        ( _ae->a_blocks_card + _ae->a_realloc_size ) );
    g_return_val_if_fail( v, FALSE );

    memcpy( v, _ae->d_mem, _ae->a_block_size * _ae->a_blocks_card );            //  copy data of old array into new array

    _ae->d_mem  =   v;                                                          //  replace old array with new array

    _ae->a_blocks_card      =   _ae->a_blocks_card  +   _ae->a_realloc_size;    //  update blocks card
    _ae->a_stat_realloc     =   _ae->a_stat_realloc +   1;                      //  update stat::reallocations

    return TRUE;
}
//  ****************************************************************************
//  PUBLIC
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  gwr_array_equal_dump()
//  ----------------------------------------------------------------------------
void
gwr_array_equal_dump(
        GwrCArrayEqual          *       _ae     )
{
    printf("GwrCArrayEqual:[%p] m[%p] bs[%06i] r[%06i] c[%03i] u[%03i] a[%03i]\n",
        _ae                 ,
        _ae->d_mem          ,
        _ae->a_block_size   ,
        _ae->a_realloc_size ,
        _ae->a_blocks_card  ,
        _ae->a_blocks_used  ,
        _ae->a_stat_realloc );
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_new()
//  ----------------------------------------------------------------------------
GwrCArrayEqual*
gwr_array_equal_new(
        guint32                         _block_size     ,
        guint32                         _realloc_size   )
{
    GwrCArrayEqual          *   a   =   NULL;
    //  ........................................................................
    //  create  GwrCArrayEqual structure
    a                       =   (GwrCArrayEqual*)g_new0( GwrCArrayEqual , 1 );

    a->d_mem                =   NULL;
    a->a_block_size         =   _block_size;
    a->a_realloc_size       =   _realloc_size;

    a->a_blocks_card        =   0;
    a->a_blocks_used        =   0;

    a->a_stat_realloc       =   0;

    return a;
}
void
gwr_array_equal_delete(
        GwrCArrayEqual          *       _ae             )
{
    gwr_array_equal_dealloc(_ae);
    g_free( _ae );
}
void
gwr_array_equal_dealloc(
        GwrCArrayEqual          *       _ae             )
{
    if ( _ae->d_mem )g_free( _ae->d_mem );
}
void
gwr_array_equal_reset(
            GwrCArrayEqual          *       _ae             )
{
    _ae->a_blocks_used      =   0;

    _ae->a_stat_realloc     =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_add()
//  ----------------------------------------------------------------------------
gboolean
gwr_array_equal_add(
        GwrCArrayEqual          *       _ae     ,
        gpointer                        _data   )
{
    if ( _ae->a_blocks_used == _ae->a_blocks_card )                             //  eventually realloc
    {
        g_return_val_if_fail( gwr_array_array_equal_realloc(_ae), FALSE );
    }

    memcpy(                                                                     //  copy data
        _ae->d_mem + _ae->a_block_size * _ae->a_blocks_used ,
        _data                                               ,
        _ae->a_block_size                                   );

    _ae->a_blocks_used  =   _ae->a_blocks_used  +   1;                          //  update used blocks card

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_get()
//  ----------------------------------------------------------------------------
gpointer
gwr_array_equal_get(
        GwrCArrayEqual          *       _ae             ,
        guint32                         _block_index    )
{
    g_return_val_if_fail( _block_index < _ae->a_blocks_used, NULL );            //  index verification

    return _ae->d_mem + _ae->a_block_size * _block_index;
}

