/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-str.h                                                            *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *                                                                           *
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
    *   Class   : None                                                          *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWR_STR_H__
#define __LIBGWR_STR_H__

#include    <string.h>

GWR_NAMESPACE_START(libgwr)

//  ............................................................................
GWR_NAMESPACE_START(chr)

extern  gboolean    is_dec_digit(gchar c);
extern  gboolean    is_hex_digit(gchar c);

extern  gboolean    ascii_dec_to_u32(gchar _c, guint32* _u32);
extern  gboolean    ascii_dec_to_i32(gchar _c, gint32*  _i32);

extern  gboolean    ascii_hex_to_u32(gchar _c, guint32* _u32);
extern  gboolean    ascii_hex_to_i32(gchar _c, gint32*  _i32);

extern  gboolean    ascii_dec_to_u64(gchar _c, guint64* _u64);
extern  gboolean    ascii_dec_to_i64(gchar _c, gint64*  _i64);

extern  gboolean    ascii_hex_to_u64(gchar _c, guint64* _u64);
extern  gboolean    ascii_hex_to_i64(gchar _c, gint64*  _i64);

GWR_NAMESPACE_END(chr)
//  ............................................................................
GWR_NAMESPACE_START(str)

extern  void            set     ( gchar*& _str, const gchar* _s );
extern  void            append  ( gchar*& _str, ... );
extern  void            concat  ( gchar*& _str, ... );

inline  gboolean        eq(const gchar* _a, const char* _b)
{
    return ( ! strcmp(_a,_b) );
}

extern  gchar       *   dequote(const gchar* _str);

extern  gboolean        ascii_dec_to_u32(const gchar* _str, guint32* _i32);
extern  gboolean        ascii_dec_to_i32(const gchar* _str, gint32*  _i32);

extern  gboolean        ascii_hex_to_u32(const gchar* _str, guint32* _i32);
extern  gboolean        ascii_hex_to_i32(const gchar* _str, gint32*  _i32);

extern  gboolean        ascii_dec_to_u64(const gchar* _str, guint64* _i64);
extern  gboolean        ascii_dec_to_i64(const gchar* _str, gint64*  _i64);

extern  gboolean        ascii_hex_to_u64(const gchar* _str, guint64* _i64);
extern  gboolean        ascii_hex_to_i64(const gchar* _str, gint64*  _i64);

GWR_NAMESPACE_END(str)

GWR_NAMESPACE_END(libgwr)

#endif                                                                          //__LIBGWR_STR_H__
