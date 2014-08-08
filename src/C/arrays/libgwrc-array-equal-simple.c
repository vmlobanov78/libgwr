/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-equal-simple.c                                            *
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
    *   Purpose : Array of constant-size data                                   *
    *                                                                           *
    *****************************************************************************
*/

#include    <stdio.h>
#include    <string.h>

#include    "libgwrc-array-equal-simple.h"

//  ****************************************************************************
//  STATIC
//  ****************************************************************************
static
gboolean
gwr_array_array_equal_simple_realloc(
        GwrCArrayEqualSimple    *       _aes    )
{
    gpointer    v   =   NULL;
    //  ........................................................................
    g_return_val_if_fail( _aes->a_slots_card == _aes->a_slots_used, FALSE );    //  ensure array is full

    v = g_malloc0( _aes->a_data_size *                                          //  alloc new array
        ( _aes->a_slots_card + _aes->a_realloc_size ) );
    g_return_val_if_fail( v, FALSE );

    memcpy( v, _aes->d_mem, _aes->a_data_size * _aes->a_slots_card );           //  copy data of old array into new array

    g_free( _aes->d_mem );                                                      //  free old memory

    _aes->d_mem  =   v;                                                         //  replace old array with new array

    _aes->a_slots_card  =   _aes->a_slots_card      +   _aes->a_realloc_size;   //  update blocks card
    _aes->a_stat_realloc=   _aes->a_stat_realloc    +   1;                      //  update stat::reallocations

    return TRUE;
}
//  ****************************************************************************
//  PUBLIC
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  gwr_array_equal_simple_new()
//  ----------------------------------------------------------------------------
GwrCArrayEqualSimple*
gwr_array_equal_simple_new(
    const   gchar           *           _name           ,
            guint32                     _block_size     ,
            guint32                     _realloc_size   )
{
    GwrCArrayEqualSimple    *   a   =   NULL;
    //  ........................................................................
    //  create  GwrCArrayEqual structure
    a                       =   (GwrCArrayEqualSimple*)g_new0( GwrCArrayEqualSimple , 1 );

    a->d_name               =   g_strdup(_name);

    a->d_mem                =   NULL;
    a->a_data_size          =   _block_size;
    a->a_realloc_size       =   _realloc_size;

    a->a_slots_card         =   0;
    a->a_slots_used         =   0;

    a->a_stat_realloc       =   0;

    return a;
}

void
gwr_array_equal_simple_delete(
        GwrCArrayEqualSimple        *       _aes             )
{
    gwr_array_equal_simple_dealloc(_aes);
    g_free(_aes->d_name);
    g_free( _aes );
}

void
gwr_array_equal_simple_dealloc(
        GwrCArrayEqualSimple        *       _aes             )
{
    if ( _aes->d_mem )
        g_free( _aes->d_mem );

}

void
gwr_array_equal_simple_reset(
            GwrCArrayEqualSimple    *       _aes             )
{
    _aes->a_slots_used      =   0;
    _aes->a_stat_realloc    =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_simple_add_data()
//  ----------------------------------------------------------------------------
void
gwr_array_equal_simple_add_data(
        GwrCArrayEqualSimple        *       _aes     ,
        gpointer                            _data   )
{
    if ( _aes->a_slots_used == _aes->a_slots_card )                             //  eventually realloc
    {
        g_return_if_fail( gwr_array_array_equal_simple_realloc(_aes) );
    }

    memcpy(                                                                     //  copy data
        _aes->d_mem + _aes->a_data_size * _aes->a_slots_used    ,
        _data                                                   ,
        _aes->a_data_size                                       );

    _aes->a_slots_used  =   _aes->a_slots_used  +   1;                          //  update used blocks card
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_simple_addb_data()
//  ----------------------------------------------------------------------------
gboolean
gwr_array_equal_simple_addb_data(
        GwrCArrayEqualSimple    *           _aes     ,
        gpointer                            _data   )
{
    if ( _aes->a_slots_used == _aes->a_slots_card )                             //  eventually realloc
    {
        g_return_val_if_fail( gwr_array_array_equal_simple_realloc(_aes), FALSE );
    }

    memcpy(                                                                     //  copy data
        _aes->d_mem + _aes->a_data_size * _aes->a_slots_used ,
        _data                                               ,
        _aes->a_data_size                                   );

    _aes->a_slots_used  =   _aes->a_slots_used  +   1;                          //  update used blocks card

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_simple_get_data()
//  ----------------------------------------------------------------------------
gpointer
gwr_array_equal_simple_get_data(
        GwrCArrayEqualSimple    *           _aes             ,
        guint32                             _block_index    )
{
    g_return_val_if_fail( _block_index < _aes->a_slots_used, NULL );            //  index verification

    return _aes->d_mem + _aes->a_data_size * _block_index;
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_simple_getb_data()
//  ----------------------------------------------------------------------------
gboolean
gwr_array_equal_simple_getb_data(
        GwrCArrayEqualSimple    *           _aes             ,
        guint32                             _block_index    ,
        gpointer                            _dest           )
{
    g_return_val_if_fail( _block_index < _aes->a_slots_used, FALSE );           //  index verification

    memcpy( _dest, _aes->d_mem + _aes->a_data_size * _block_index, _aes->a_data_size );

    return TRUE;
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_simple_dump()
//  ----------------------------------------------------------------------------
void
gwr_array_equal_simple_dump(
        GwrCArrayEqualSimple    *       _aes        ,
        gboolean                        _dump_data  )
{
    guint32     i,j;
    gpointer    p;
    //  ........................................................................
    printf("GwrCArrayEqualSimple:[%p] mem[%p] bsz[%06i] rsz[%06i] crd[%03i] usd[%03i] rea[%03i]\n",
        _aes                    ,
        _aes->d_mem             ,
        _aes->a_data_size       ,
        _aes->a_realloc_size    ,
        _aes->a_slots_card      ,
        _aes->a_slots_used      ,
        _aes->a_stat_realloc    );

    if ( ! _dump_data )
        return;

    for ( i = 0 ; i != _aes->a_slots_used ; i++ )
    {
        printf("  Slot [%05i]:", i );
        for ( j = 0 ; j != _aes->a_data_size ; j++ )
        {
            p   =   _aes->d_mem + ( i * _aes->a_data_size ) + j;
            printf("%02x ", *( (guchar*)p ) );
        }
        printf("\n");
    }
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_simple_get_stats()
//  ----------------------------------------------------------------------------
void
gwr_array_equal_simple_get_stats(
        GwrCArrayEqualSimple        *       _aes             ,
        GwrCArrayEqualSimpleStat    *       _aes_stat        )
{
    _aes_stat->a_slots_card     =   _aes->a_slots_card;
    _aes_stat->a_slots_used     =   _aes->a_slots_used;
    _aes_stat->a_realloc        =   _aes->a_stat_realloc;

    gwr_array_equal_simple_get_mfp( _aes, &( _aes_stat->a_mfp ) );
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_simple_get_mfp()
//  ----------------------------------------------------------------------------
void
gwr_array_equal_simple_get_mfp(
        GwrCArrayEqualSimple    *       _aes    ,
        GwrCAMFP                *       _out    )
{
    _out->a_ss      =   sizeof(GwrCArrayEqualSimple);
    _out->a_sa      =   _aes->a_data_size * _aes->a_slots_card;
    _out->a_su      =   _aes->a_data_size * _aes->a_slots_used;
}
