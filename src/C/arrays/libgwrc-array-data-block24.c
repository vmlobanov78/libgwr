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

#define     GWR_ADB24_GET_SIZE_OFFSET   ( sizeof(gpointer)      )
#define     GWR_ADB24_GET_USED_OFFSET   ( sizeof(gpointer) + 2  )

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
    //  memory layout at _dbk24->a_blob :
    //
    //  offset :     0  1  2  3  4  5  6  7
    //  bytes  :    s0 s1 s2 u0 u1 u2 ?6 ?7
    //
    //  size = s0 + 8 * s1 + 16 * s2
    //  used = u0 + 8 * u1 + 16 * u2
    //  ........................................................................
    //memcpy( &u32, _dbk24->a_blob, 3 );

    u32 =   *( (guint32*)( (gpointer)_dbk24 + GWR_ADB24_GET_SIZE_OFFSET) );     //  u0 s2 s1 s0
    u32 =   u32 & 0x00ffffff;                                                   //  00 s2 s1 s0

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
    //guint64     u64 =   0;
    //  ........................................................................
    //  memory layout at _dbk24->a_blob :
    //
    //  offset :     0  1  2  3  4  5  6  7
    //  bytes  :    s0 s1 s2 u0 u1 u2 ?6 ?7
    //
    //  size = s0 + 8 * s1 + 16 * s2
    //  used = u0 + 8 * u1 + 16 * u2
    //  ........................................................................
    //memcpy( &u32, _dbk24->a_blob + 3, 3 );                                    //  function call memcpy

    //*( (gchar*)(& u32)        )   =   *( (gchar*)(_dbk24) + 11 );             //  gcc translate all
    //*( (gchar*)(& u32) + 1    )   =   *( (gchar*)(_dbk24) + 12 );
    //*( (gchar*)(& u32) + 2    )   =   *( (gchar*)(_dbk24) + 13 );


    //u64 =   *( (guint64*)( (gpointer)_dbk24 + 8) );     //  ?7 ?6 u2 u1 u0 s2 s1 s0   //  not 32bits compatible
    //u64 =   u64 >> 24;                                  //  00 00 00 ?7 ?6 u2 u1 u0
    //u64 =   u64 &  0x0000000000ffffff;                  //  00 00 00 00 00 u2 u1 00
    //return (guint32)( u64 );

    u32 =   *( (guint32*)( (gpointer)_dbk24 + GWR_ADB24_GET_USED_OFFSET) );     //  u2 u1 u0 s2
    u32 =   u32 >> 8;                                                           //  00 u2 u1 u0

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
    //D guint32     a   =   gwr_array_dbk24_available_bytes(_dbk24);
    //D guint32     u   =   Gwr_array_dbk24_get_blob_used_bytes(_dbk24);
    //  ........................................................................
    //D printf("* gwr_array_dbk24_add(%i):a[%i] u[%i]\n", _data_size, a, u );

    memcpy(
        _dbk24->d_mem + Gwr_array_dbk24_get_blob_used_bytes(_dbk24) ,
        _data                                                       ,
        _data_size                                                  );

    Gwr_array_dbk24_set_blob_used_bytes( _dbk24, Gwr_array_dbk24_get_blob_used_bytes(_dbk24) + _data_size );

    //D gwr_array_dbk24_dump( _dbk24 );
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_add_with_extra_data()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk24_add_with_extra_data(
        GwrCADBlock24               *   _dbk24              ,
        gpointer                        _data               ,
        guint32                         _data_size          ,
        gpointer                        _extra_data         ,
        guint32                         _extra_data_len     )
{
    //  ........................................................................
    memcpy(
        _dbk24->d_mem + Gwr_array_dbk24_get_blob_used_bytes(_dbk24) ,
        _data                                                       ,
        _data_size                                                  );

    memcpy(
        _dbk24->d_mem + Gwr_array_dbk24_get_blob_used_bytes(_dbk24) + _data_size    ,
        _extra_data                                                                 ,
        _extra_data_len                                                             );

    Gwr_array_dbk24_set_blob_used_bytes( _dbk24, Gwr_array_dbk24_get_blob_used_bytes(_dbk24) + _data_size + _extra_data_len );
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
//  ----------------------------------------------------------------------------
//  gwr_array_dbk24_get_mfp()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk24_get_mfp(
        GwrCADBlock24           *       _dbk24          ,
        GwrCAMFP                *       _out            )
{
    _out->a_ss  =   sizeof( GwrCADBlock24 );
    _out->a_sa  =   (guint32)Gwr_array_dbk24_get_blob_size(_dbk24);
    _out->a_su  =   (guint32)Gwr_array_dbk24_get_blob_used_bytes(_dbk24);
}


