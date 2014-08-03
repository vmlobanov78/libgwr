/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-data-block24.c                                            *
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
#include    "libgwrc-array-data-block24.h"

guint32     GwrCADBlock24_SSIZE   =   sizeof( GwrCADBlock24 );

//  24-bit value handling :
//  ---------------------
//
//  x86 endianness :
//          value           memory
//  u32     0x44332211      0x11 0x22 0x33 0x44
//
//  what we want is :
//  u32     0x00332211      0x11 0x22 0x33 0x00

//  ----------------------------------------------------------------------------
//  Gwr_array_dbk24_set_blob_size()
//  ----------------------------------------------------------------------------
static inline void
Gwr_array_dbk24_set_blob_size(
        GwrCADBlock24               *   _dbk24          ,
        guint32                         _u32            )
{
    memcpy( _dbk24->a_blob, &_u32, 3 );
}
//  ----------------------------------------------------------------------------
//  Gwr_array_dbk24_set_blob_used_bytes()
//  ----------------------------------------------------------------------------
static inline void
Gwr_array_dbk24_set_blob_used_bytes(
        GwrCADBlock24               *   _dbk24          ,
        guint32                         _u32            )
{
    memcpy( _dbk24->a_blob + 3, &_u32, 3 );
}
//  ----------------------------------------------------------------------------
//  Gwr_array_dbk24_get_blob_size()
//  ----------------------------------------------------------------------------
static inline guint32
Gwr_array_dbk24_get_blob_size(
        GwrCADBlock24               *   _dbk24          )
{
    guint32     u32 =   0;
    //  ........................................................................
    memcpy( &u32, _dbk24->a_blob, 3 );
    return u32;
}
//  ----------------------------------------------------------------------------
//  Gwr_array_dbk24_get_blob_used_bytes()
//  ----------------------------------------------------------------------------
static inline guint32
Gwr_array_dbk24_get_blob_used_bytes(
        GwrCADBlock24               *   _dbk24          )
{
    guint32     u32 =   0;
    //  ........................................................................
    memcpy( &u32, _dbk24->a_blob + 3, 3 );
    return u32;
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_new()
//  ----------------------------------------------------------------------------
GwrCADBlock24*
gwr_array_dbk24_new(
        guint32                         _dbk24_size   )
{
    GwrCADBlock24             *   b   =   NULL;
    //  ........................................................................
    //  create a GwrCADBlock24  struct
    b                   =   (GwrCADBlock24 *)g_new0( GwrCADBlock24  , 1 );

    gwr_array_dbk24_alloc( b, _dbk24_size );

    return b;
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_alloc()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk24_alloc(
        GwrCADBlock24               *   _dbk24          ,
        guint32                         _dbk24_size     )
{
    //  ........................................................................
    _dbk24->d_mem           =   g_malloc( _dbk24_size );

    Gwr_array_dbk24_set_blob_size       ( _dbk24, _dbk24_size );
    Gwr_array_dbk24_set_blob_used_bytes ( _dbk24, 0 );
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_dealloc()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk24_dealloc(
        GwrCADBlock24                 *   _dbk24  )
{
    //  ........................................................................
    g_free( _dbk24->d_mem );
    memset( _dbk24->a_blob, 0, 6 );
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_reset()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk24_reset(
        GwrCADBlock24                 *   _dbk24  )
{
    //  ........................................................................
    Gwr_array_dbk24_set_blob_used_bytes ( _dbk24, 0 );
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_available_bytes()
//  ----------------------------------------------------------------------------
guint32
gwr_array_dbk24_available_bytes(
        GwrCADBlock24                 *   _dbk24          )
{
    return
        Gwr_array_dbk24_get_blob_size(_dbk24)       -
        Gwr_array_dbk24_get_blob_used_bytes(_dbk24) ;
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_used_bytes()
//  ----------------------------------------------------------------------------
guint32
gwr_array_dbk24_used_bytes(
        GwrCADBlock24                 *   _dbk24          )
{
    return  Gwr_array_dbk24_get_blob_used_bytes(_dbk24);
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_add()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk24_add(
        GwrCADBlock24               *   _dbk24          ,
        gpointer                        _data           ,
        guint32                         _data_size      )
{
    guint32     a   =   gwr_array_dbk24_available_bytes(_dbk24);
    guint32     u   =   Gwr_array_dbk24_get_blob_used_bytes(_dbk24);
    //  ........................................................................
    //printf("* gwr_array_dbk24_add(%i):a[%i] u[%i]\n", _data_size, a, u );

    memcpy(
        _dbk24->d_mem + Gwr_array_dbk24_get_blob_used_bytes(_dbk24) ,
        _data                                                       ,
        _data_size                                                  );

    Gwr_array_dbk24_set_blob_used_bytes( _dbk24, Gwr_array_dbk24_get_blob_used_bytes(_dbk24) + _data_size );

    //gwr_array_dbk24_dump( _dbk24 );
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_add_with_extra_data_index()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk24_add_with_extra_data_index(
        GwrCADBlock24               *   _dbk24              ,
        gpointer                        _data               ,
        guint32                         _data_size          ,
        guint32                         _extra_data_index   ,
        guint8                      *   _xdi_len            )
{
    //  ........................................................................
    memcpy(
        _dbk24->d_mem + Gwr_array_dbk24_get_blob_used_bytes(_dbk24) ,
        _data                                                       ,
        _data_size                                                  );

    memcpy(
        _dbk24->d_mem + Gwr_array_dbk24_get_blob_used_bytes(_dbk24) + _data_size    ,
        &(_extra_data_index)                                                        ,
        sizeof(guint32)                                                             );

    Gwr_array_dbk24_set_blob_used_bytes( _dbk24, Gwr_array_dbk24_get_blob_used_bytes(_dbk24) + _data_size + sizeof(guint32) );

    *_xdi_len               =   sizeof(guint32);
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_dump()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk24_dump(
        GwrCADBlock24                 *   _dbk24          )
{
    printf("GwrCADBlock24       :[%p] mem[%p] siz[%06i] usd[%06i] ava[%06i]\n"           ,
        _dbk24                                          ,
        _dbk24->d_mem                                   ,
        Gwr_array_dbk24_get_blob_size(_dbk24)           ,
        Gwr_array_dbk24_get_blob_used_bytes(_dbk24)     ,
            Gwr_array_dbk24_get_blob_size(_dbk24)       -
            Gwr_array_dbk24_get_blob_used_bytes(_dbk24) );
}

