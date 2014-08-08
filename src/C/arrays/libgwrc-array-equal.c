/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-equal.c                                                   *
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

#include    "libgwrc-array-data-block.h"
#include    "libgwrc-array-equal.h"

//  ****************************************************************************
//  STATIC
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  gwr_array_data_multi_ensure_first_block_exists()
//  ----------------------------------------------------------------------------
void                                                                            //  _OPTIM_01_
gwr_array_equal_ensure_first_block_exists(
        GwrCArrayEqual          *       _ae     )
{
    GwrCADBlock24           b;
    //  ........................................................................
    //  if blocks are in use, at least one block exist : nothing to do
    if ( _ae->d_blocks->a_slots_used )
        return;

    //  if no block is in use, but first blocks exists, indicate
    //  that it is in use
    if ( _ae->d_blocks->a_slots_card )
    {
        _ae->d_blocks->a_slots_used   =   1;
        return;
    }

    //  else create first block and add it
    gwr_array_dbk24_alloc( &b, _ae->a_dbk_size );
    _ae->a_stat_realloc =   _ae->a_stat_realloc + 1;
    gwr_array_equal_simple_add_data( _ae->d_blocks , &b );
}
//  ****************************************************************************
//  PUBLIC
//  ****************************************************************************
//  ----------------------------------------------------------------------------
//  gwr_array_equal_new()
//  ----------------------------------------------------------------------------
GwrCArrayEqual*
gwr_array_equal_new(
    const   gchar               *       _name                           ,
            guint32                     _data_size                      ,
            guint32                     _block_capacity                 ,
            guint32                     _block_realloc                  )
{
    GwrCArrayEqual          *   a   =   NULL;
    //  ........................................................................
    a                       =   (GwrCArrayEqual*)g_new0( GwrCArrayEqual , 1 );

    a->d_name               =   g_strdup(_name);

    a->d_blocks             =   gwr_array_equal_simple_new( "GwrCArrayEqual", GwrCADBlock24_SSIZE, _block_realloc );

    a->a_data_size          =   _data_size;
    a->a_dbk_capacity       =   _block_capacity;
    a->a_dbk_size           =   _block_capacity * _data_size;
    a->a_dbk_realloc        =   _block_realloc;
    a->a_data_card          =   0;

    a->a_stat_realloc       =   0;

    gwr_array_equal_ensure_first_block_exists( a );

    return a;
}
void
gwr_array_equal_delete(
        GwrCArrayEqual          *       _ae             )
{
    guint32             i;
    GwrCADBlock24   *   dbk24   =   NULL;
    //  ........................................................................
    for ( i = 0 ; i != _ae->d_blocks->a_slots_used ; i++ )
    {
        dbk24 = gwr_array_equal_simple_get_data( _ae->d_blocks, i );
        gwr_array_dbk24_dealloc(dbk24);
    }

    gwr_array_equal_simple_delete( _ae->d_blocks );

    g_free( _ae->d_name );

    g_free( _ae );
}
void
gwr_array_equal_reset(
            GwrCArrayEqual      *       _ae             )
{
    guint32             i;
    GwrCADBlock24   *   dbk24   =   NULL;
    //  ........................................................................
    for ( i = 0 ; i != _ae->d_blocks->a_slots_used ; i++ )
    {
        dbk24 = gwr_array_equal_simple_get_data( _ae->d_blocks, i );
        gwr_array_dbk24_reset(dbk24);
    }

    gwr_array_equal_simple_reset( _ae->d_blocks );

    gwr_array_equal_ensure_first_block_exists(_ae);

    _ae->a_data_card        =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_add_data()
//  ----------------------------------------------------------------------------
void
gwr_array_equal_add_data(
        GwrCArrayEqual          *       _ae     ,
        gpointer                        _data   )
{
    GwrCADBlock24   *   dbk24_c;
    GwrCADBlock24       dbk24_n;
    //  ........................................................................
    dbk24_c =                                                                   //  get current working block
        gwr_array_equal_simple_get_data( _ae->d_blocks, _ae->d_blocks->a_slots_used - 1 );

    //  enough size to add data in the working block
    if ( gwr_array_dbk24_available_bytes( dbk24_c ) >=  _ae->a_data_size )
    {
        //D printf("> gwr_array_equal_add_data():[%p] + [%5i] = [%p]\n",
        //D    dbk24_c->d_mem                      ,
        //D    gwr_array_dbk24_used_bytes(dbk24_c) ,
        //D    dbk24_c->d_mem                      +
        //D    gwr_array_dbk24_used_bytes(dbk24_c) );


        gwr_array_dbk24_add( dbk24_c, _data, _ae->a_data_size );                //  add the data
        _ae->a_data_card    =   _ae->a_data_card + 1;

        return;
    }

    //  not enough size to add data in the working block
    gwr_array_dbk24_alloc   ( &dbk24_n , _ae->a_dbk_size );                     //  alloc a new GwrCADBlock24
    _ae->a_stat_realloc =   _ae->a_stat_realloc + 1;

    gwr_array_dbk24_add     ( &dbk24_n , _data, _ae->a_data_size );             //  add data to GwrCADBlock24
    gwr_array_equal_simple_add_data( _ae->d_blocks, &dbk24_n );                 //  copy the GwrCADBlock24 struct in a GwrCArrayEqualSimple slot

    _ae->a_data_card    =   _ae->a_data_card + 1;
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_addb_data()
//  ----------------------------------------------------------------------------
gboolean
gwr_array_equal_addb_data(
        GwrCArrayEqual          *       _ae     ,
        gpointer                        _data   )
{
    g_return_val_if_reached( FALSE );
}
/*
//  ----------------------------------------------------------------------------
//  gwr_array_equal_get_block()
//  ----------------------------------------------------------------------------
GwrCADBlock24*
gwr_array_equal_get_block(
        GwrCArrayEqual          *       _ae             ,
        guint32                         _block_index    )
{
    GwrCADBlock24       *   blk24                   =   NULL;
    //  ........................................................................
    g_return_val_if_fail( _block_index < _ae->d_blocks->a_slots_used, NULL );   //  index verification

    blk24 = gwr_array_equal_simple_get( _ae->d_blocks, _block_index );

    return blk24;
}
*/
//  ----------------------------------------------------------------------------
//  gwr_array_equal_get_data()
//  ----------------------------------------------------------------------------
gpointer
gwr_array_equal_get_data(
        GwrCArrayEqual          *       _ae             ,
        guint32                         _data_index     )
{
    guint32                 data_block_index        =   0;
    guint32                 data_offset_in_block    =   0;
    GwrCADBlock24       *   blk24                   =   NULL;
    //  ........................................................................
    data_block_index        =   _data_index / _ae->a_dbk_capacity;
    data_offset_in_block    =   ( _data_index % _ae->a_dbk_capacity ) * _ae->a_data_size;

    g_return_val_if_fail( data_block_index      <  _ae->d_blocks->a_slots_used, NULL        );   //  index verification

    blk24 = gwr_array_equal_simple_get_data( _ae->d_blocks, data_block_index );

    g_return_val_if_fail( data_offset_in_block  <= gwr_array_dbk24_used_bytes(blk24), NULL  );   //  index verification

    //D printf("> gwr_array_equal_get_data():[%p]\n", blk24->d_mem + data_offset_in_block);

    return blk24->d_mem + data_offset_in_block;
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_getb_data()
//  ----------------------------------------------------------------------------
gboolean
gwr_array_equal_getb_data(
        GwrCArrayEqual          *       _ae             ,
        guint32                         _block_index    ,
        gpointer                *       _dest           )
{
    g_return_val_if_reached(FALSE);
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_dump()
//  ----------------------------------------------------------------------------
void
gwr_array_equal_dump(
        GwrCArrayEqual          *       _ae     )
{
    printf("+-----------------------+\n");
    printf("| GwrCArrayEqual:dump() |\n");
    printf("+-----------------------+\n");
    printf("GwrCArrayEqual      :%s\n", _ae->d_name);
    printf("                    :[%p] dsz[%06i] bcp[%06i] bsz[%06i] rea[%03i]\n",
        _ae                 ,
        _ae->a_data_size    ,
        _ae->a_dbk_capacity ,
        _ae->a_dbk_size     ,
        _ae->a_dbk_realloc  );

    gwr_array_equal_simple_dump( _ae->d_blocks, FALSE );
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_get_stats()
//  ----------------------------------------------------------------------------
void
gwr_array_equal_get_stats(
        GwrCArrayEqual      *       _ae             ,
        GwrCArrayEqualStat  *       _ae_stat        )
{
    gwr_array_equal_simple_get_stats( _ae->d_blocks, &(_ae_stat->a_simple_stat) );

    _ae_stat->a_dbk_capacity=   _ae->a_dbk_capacity;
    _ae_stat->a_dbk_size    =   _ae->a_dbk_size;

    _ae_stat->a_realloc     =   _ae->a_stat_realloc;

    gwr_array_equal_get_mfp( _ae, &( _ae_stat->a_mfp ) );
}
//  ----------------------------------------------------------------------------
//  gwr_array_equal_get_mfp()
//  ----------------------------------------------------------------------------
void
gwr_array_equal_get_mfp(
        GwrCArrayEqual          *       _ae             ,
        GwrCAMFP                *       _out            )
{
    guint32             i;
    GwrCADBlock24   *   dbk24   =   NULL;
    //  ........................................................................
    gwr_array_equal_simple_get_mfp( _ae->d_blocks, _out );

    _out->a_ss  =   _out->a_ss  +   sizeof( GwrCArrayEqual );
    _out->a_sa  =   _out->a_sa  +   _ae->d_blocks->a_slots_used *
                                    _ae->a_dbk_size;
    for ( i = 0 ; i != _ae->d_blocks->a_slots_used ; i++ )
    {
        dbk24 = gwr_array_equal_simple_get_data( _ae->d_blocks, i );
        _out->a_su  =   _out->a_su  +   gwr_array_dbk24_used_bytes(dbk24);
    }
}

