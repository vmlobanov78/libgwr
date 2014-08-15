/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-array-data-block16.c                                            *
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
#include    "libgwrc-array-data-block16.h"

guint32     GwrCADBlock16_SSIZE   =   sizeof( GwrCADBlock16 );

//  ----------------------------------------------------------------------------
//  gwr_array_dbk16_new()
//  ----------------------------------------------------------------------------
GwrCADBlock16*
gwr_array_dbk16_new(
        guint16                         _dbk16_size   )
{
    GwrCADBlock16             *   b   =   NULL;
    //  ........................................................................
    //  create a GwrCADBlock16  struct
    b                   =   (GwrCADBlock16 *)g_new0( GwrCADBlock16  , 1 );

    gwr_array_dbk16_alloc( b, _dbk16_size );

    return b;
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk16_alloc()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk16_alloc(
        GwrCADBlock16               *   _dbk16          ,
        guint16                         _dbk16_size     )
{
    //  ........................................................................
    _dbk16->d_mem           =   g_malloc( _dbk16_size );
    _dbk16->a_size          =   _dbk16_size;
    _dbk16->a_used_bytes    =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk16_dealloc()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk16_dealloc(
        GwrCADBlock16                 *   _dbk16  )
{
    //  ........................................................................
    g_free( _dbk16->d_mem );
    _dbk16->a_size          =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk16_reset()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk16_reset(
        GwrCADBlock16                 *   _dbk16  )
{
    //  ........................................................................
    _dbk16->a_used_bytes    =   0;
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk16_available_bytes()
//  ----------------------------------------------------------------------------
inline
guint32
gwr_array_dbk16_available_bytes(
        GwrCADBlock16                 *   _dbk16          )
{
    return _dbk16->a_size - _dbk16->a_used_bytes;
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk16_add()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk16_add(
        GwrCADBlock16               *   _dbk16          ,
        gpointer                        _data           ,
        guint16                         _data_size      )
{
    //  ........................................................................
    memcpy( _dbk16->d_mem + _dbk16->a_used_bytes, _data, _data_size );

    _dbk16->a_used_bytes    =   _dbk16->a_used_bytes    +   _data_size;
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk16_add_with_extra_data_index()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk16_add_with_extra_data_index(
        GwrCADBlock16               *   _dbk16              ,
        gpointer                        _data               ,
        guint16                         _data_size          ,
        guint32                         _extra_data_index   ,
        guint8                      *   _xdi_len            )
{
    //  ........................................................................
    memcpy( _dbk16->d_mem + _dbk16->a_used_bytes                , _data                 , _data_size        );
    memcpy( _dbk16->d_mem + _dbk16->a_used_bytes + _data_size   , &(_extra_data_index)  , sizeof(guint32)   );

    _dbk16->a_used_bytes    =   _dbk16->a_used_bytes    +   _data_size  +   sizeof(guint32);

    *_xdi_len               =   4;
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk16_dump()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk16_dump(
        GwrCADBlock16                 *   _dbk16          )
{
    printf("GwrCADBlock16   :[%p] mem[%p] siz[%06i] usd[%06i] ava[%06i]\n"           ,
        _dbk16                                                  ,
        _dbk16->d_mem,  _dbk16->a_size, _dbk16->a_used_bytes    ,
        gwr_array_dbk16_available_bytes( _dbk16)     );
}
//  ----------------------------------------------------------------------------
//  gwr_array_dbk16_get_mfp()
//  ----------------------------------------------------------------------------
void
gwr_array_dbk16_get_mfp(
        GwrCADBlock16           *       _dbk16          ,
        GwrCAMFP                *       _out            )
{
    _out->a_ss  =   sizeof( GwrCADBlock16 );
    _out->a_sa  =   (guint32)(_dbk16->a_size);
    _out->a_su  =   (guint32)(_dbk16->a_used_bytes);
}







