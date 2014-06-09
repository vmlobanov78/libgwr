/*
    *****************************************************************************
    *                                                                           *
    *   libgwr.cc                                                               *
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
    *   Nspace  :                                                               *
    *                                                                           *
    *   Class   : LibGwrStatic                                                  *
    *                                                                           *
    *****************************************************************************
*/


//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)

Logger  *   GD_libgwr_logger            = NULL;
guint32     GA_libgwr_logger_channel    = 0;

//  ****************************************************************************
//
//  chr utilities
//
//  ****************************************************************************
namespace chr
{
//  ----------------------------------------------------------------------------
//  0-9  : 48 -> 57
static      guchar  s_ascii_is_shell_variable_character[256]   =
{
    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0,     //    0
    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0,     //   16
    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0,     //   32
    1,   1,   1,   1,   1,   1,   1,   1,       1,   1,   0,   0,   0,   0,   0,   0,     //   48
    0,   1,   1,   1,   1,   1,   1,   1,       1,   1,   1,   1,   1,   1,   1,   1,     //   64
    1,   1,   1,   1,   1,   1,   1,   1,       1,   1,   1,   0,   0,   0,   0,   1,     //   80
    0,   1,   1,   1,   1,   1,   1,   1,       1,   1,   1,   1,   1,   1,   1,   1,     //   96
    1,   1,   1,   1,   1,   1,   1,   1,       1,   1,   1,   0,   0,   0,   0,   0,     //  112

    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0,     //  128+
    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,       0,   0,   0,   0,   0,   0,   0,   0
};
//  ----------------------------------------------------------------------------
//  0-9  : 48 -> 57
static      guchar  s_ascii_is_dec[256]   =
{
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //    0
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //   16
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //   32
    0,   1,   2,   3,   4,   5,   6,   7,       8,   9, 255, 255, 255, 255, 255, 255,     //   48
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //   64
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //   80
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //   96
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //  112

    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //  128+
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255
};
//  ----------------------------------------------------------------------------
//  0-9  : 48 ->  57
//  A-F  : 65 ->  70
//  a-f  : 97 -> 102
static      guchar  s_ascii_is_hex[256]   =
{
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //    0
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //   16
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //   32
    0,   1,   2,   3,   4,   5,   6,   7,       8,   9, 255, 255, 255, 255, 255, 255,     //   48
    255,  10,  11,  12,  13,  14,  15, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //   64
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //   80
    255,  10,  11,  12,  13,  14,  15, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //   96
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //  112

    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,     //  128+
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,     255, 255, 255, 255, 255, 255, 255, 255
};

gboolean    is_shell_variable_character(gchar _c)
{
    return ( s_ascii_is_shell_variable_character[ (guchar)_c ] );
}

gboolean    is_dec_digit(gchar _c)
{
    return ( s_ascii_is_dec[ (guchar)_c ] != 255 );
}

gboolean    is_hex_digit(gchar _c)
{
    return ( s_ascii_is_hex[ (guchar)_c ] != 255 );
}

gboolean    ascii_dec_to_u32(gchar _c, guint32* _u32)
{
    guchar   c = s_ascii_is_dec[ (guchar)_c ];

    if ( c != 255 )
    {
        *(_u32) = (guint32)c;
        return TRUE;
    }

    *(_u32) = (guint32)0;
    return FALSE;
}
gboolean    ascii_dec_to_i32(gchar _c, gint32* _i32)
{
    guchar   c = s_ascii_is_dec[ (guchar)_c ];

    if ( c != 255 )
    {
        *(_i32) = (gint32)c;
        return TRUE;
    }

    *(_i32) = (gint32)0;
    return FALSE;
}

gboolean    ascii_hex_to_u32(gchar _c, guint32* _u32)
{
    guchar   c = s_ascii_is_hex[ (guchar)_c ];

    if ( c != 255 )
    {
        *(_u32) = (guint32)c;
        return TRUE;
    }

    *(_u32) = (guint32)0;
    return FALSE;
}
gboolean    ascii_hex_to_i32(gchar _c, gint32* _i32)
{
    guchar   c = s_ascii_is_hex[ (guchar)_c ];

    if ( c != 255 )
    {
        *(_i32) = (gint32)c;
        return TRUE;
    }

    *(_i32) = (gint32)0;
    return FALSE;
}
//  ============================================================================
//  64 bits related
//  ============================================================================
//  ----------------------------------------------------------------------------
//  libgwr::chr::ascii_dec_to_u64
//  ----------------------------------------------------------------------------
//! \name   ascii_dec_to_u64
//!
//! \brief  Transform a chr into a u64 integer
gboolean    ascii_dec_to_u64(gchar _c, guint64* _u64)
{
    guchar   c = s_ascii_is_dec[ (guchar)_c ];

    if ( c != 255 )
    {
        *(_u64) = (guint64)c;
        return TRUE;
    }

    *(_u64) = (guint64)0;
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  libgwr::chr::ascii_dec_to_i64
//  ----------------------------------------------------------------------------
//! \name   ascii_dec_to_i64
//!
//! \brief  Transform a chr into a i64 integer
gboolean    ascii_dec_to_i64(gchar _c, gint64* _i64)
{
    guchar   c = s_ascii_is_dec[ (guchar)_c ];

    if ( c != 255 )
    {
        *(_i64) = (gint64)c;
        return TRUE;
    }

    *(_i64) = (gint64)0;
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  libgwr::chr::ascii_hex_to_u64
//  ----------------------------------------------------------------------------
//! \name   ascii_hex_to_u64
//!
//! \brief  Transform a chr into a u64 integer
gboolean    ascii_hex_to_u64(gchar _c, guint64* _u64)
{
    guchar   c = s_ascii_is_hex[ (guchar)_c ];

    if ( c != 255 )
    {
        *(_u64) = (guint64)c;
        return TRUE;
    }

    *(_u64) = (guint64)0;
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  libgwr::chr::ascii_hex_to_i64
//  ----------------------------------------------------------------------------
//! \name   ascii_hex_to_i64
//!
//! \brief  Transform a chr into a i64 integer
gboolean    ascii_hex_to_i64(gchar _c, gint64* _i64)
{
    guchar   c = s_ascii_is_hex[ (guchar)_c ];

    if ( c != 255 )
    {
        *(_i64) = (gint64)c;
        return TRUE;
    }

    *(_i64) = (gint64)0;
    return FALSE;
}

}
//  ****************************************************************************
//
//  str utilities
//
//  ****************************************************************************
namespace str
{
//  ----------------------------------------------------------------------------
static  guint32     s_u32_10_power[] =
{
    1,                          //  10 ^  0
    10,
    100,
    1000,                       //  10 ^  3
    10000,
    100000,
    1000000,                    //  10 ^  6
    10000000,
    100000000,
    1000000000                  //  10 ^  9
};
//  ----------------------------------------------------------------------------
static  guint32     s_u32_16_power[] =
{
    0x00000001,         //  16 ^   0
    0x00000010,         //  16 ^   1
    0x00000100,         //  16 ^   2
    0x00001000,         //  16 ^   3
    0x00010000,         //  16 ^   4
    0x00100000,         //  16 ^   5
    0x01000000,         //  16 ^   6
    0x10000000          //  16 ^   7
};
//  ----------------------------------------------------------------------------
//  libgwr::str::dequote
//  ----------------------------------------------------------------------------
//! \name   dequote
//!
//! \brief  Dequote a string ( remove leading and trailing ' and " )
gchar   *  dequote(const gchar* _str)
{
    gchar       quote   = 0;
    const   gchar   *   start   = NULL;
    const   gchar   *   end     = NULL;
    //..........................................................................
    if ( _str[0] == '"'     )   quote = '"'     ;
    if ( _str[0] == '\''    )   quote = '\''    ;

    if ( ! quote )
        return g_strdup(_str);

    start   = _str + 1;
    end     = strchr( 1 + _str, (int)quote );

    if ( ! end )
        return g_strdup(_str);

    if ( ( end - start ) <= 0 )
        _GWR_BREAK_;

    return g_strndup( 1 + _str, (gsize)(end - start) );
}
//  ----------------------------------------------------------------------------
//  libgwr::str::ascii_dec_to_u32
//  ----------------------------------------------------------------------------
//! \name   ascii_dec_to_u32
//!
//! \brief  try to get a u32 integer from a 0-terminated string.
//! format :    +-[0-9]* ( this kind of procedure that should be coded in asm )
gboolean    ascii_dec_to_u32(const gchar* _str, guint32* _u32)
{
    //  minimum i32 possible = - 2 147 483 648 in decimal representation = (1) + 10 = 11 chr maxi
    //  maximum i32 possible = + 2 147 483 647 in decimal representation = (1) + 10 = 11 chr maxi
    //  maximum u32 possible = + 4 294 967 295 in decimal representation = (1) + 10 = 11 chr maxi

    gint        len         = 0;
    const   guint32     u32M        = 0xFFFFFFFF;

    guint32     i           = 0;
    guint32     u32t        = 0;                                        //  temporary u32 ( for intermediate calculation )
    guint32     u32r        = 0;                                        //  temporary r32 ( for intermediate calculation )

    const   gchar   *   p1          = NULL;
    const   gchar   *   p2          = NULL;
    gboolean    sign        = FALSE;
    //gboolean    negative    = FALSE;
    //  ........................................................................
    if ( ! _str )                                                               //  NULL input string !
        goto lab_failure;

    len     = strlen( _str );                                                   //  input string length

    if ( ! len )                                                                //  empty input string !
        goto lab_failure;

    //..........................................................................
    //  some tests
    if ( len > 11 )                                                             //  cf i32m & i32M, 11 chr maxi
        goto lab_failure;

    switch ( _str[0] )
    {
    case '-'    :
        //sign        = TRUE;
        //negative    = TRUE;
        //if ( len == 1 )
        //    return FALSE;
        goto lab_failure;
        break;

    case '+'    :
        sign        = TRUE;
        if ( len == 1 )
            goto lab_failure;
        //  negative = FALSE;                                               //  already initialized !
        break;

    default     :
        if ( len == 11 )                                                    //  no sign : 11 chr is too much
            goto lab_failure;
        //  negative = FALSE;                                               //  already initialized !
        break;
    }

    //..........................................................................
    //  go !
    p1      = sign ? _str + 1 : _str;                                           //  p1 = first 'maybe decimal digit'
    p2      = _str + len;                                                       //  p2 = terminal 0 of the string

    while ( -- p2 >= p1 )
    {
        if ( ! libgwr::chr::ascii_dec_to_u32(*p2, &u32t) )                      //  not decimal chr
            goto lab_failure;

        u32t = u32t * s_u32_10_power[ i++ ];                                    //  finish computing chr value

        //  check overflow
        if ( u32t > ( u32M - u32r ) )
            goto lab_failure;

        u32r = u32r + u32t;                                                     //  ok, add to final result
    }

    *_u32 = u32r;

    return TRUE;
    //..........................................................................
    //  failure
lab_failure:
    printf("libgwr::str::ascii_dec_to_u32():error [%s]\n", _str ? _str : "NULL");
    *_u32 = 0;
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  libgwr::str::ascii_dec_to_i32
//  ----------------------------------------------------------------------------
//! \name   ascii_dec_to_i32
//!
//! \brief  try to get a i32 integer from a 0-terminated string.
//! format :    +-[0-9]* ( this kind of procedure that should be coded in asm )
gboolean    ascii_dec_to_i32(const gchar* _str, gint32* _i32)
{
    //  minimum i32 possible = - 2 147 483 648 in decimal representation = (1) + 10 = 11 chr maxi
    //  maximum i32 possible = + 2 147 483 647 in decimal representation = (1) + 10 = 11 chr maxi
    //  maximum u32 possible = + 4 294 967 295 in decimal representation = (1) + 10 = 11 chr maxi

    gint        len         = 0;
    const   guint32     u32_i32m    = 0x80000000;
    const   guint32     u32_i32M    = 0x7FFFFFFF;

    guint32     i           = 0;
    guint32     u32t        = 0;                                        //  temporary u32 ( for intermediate calculation )
    guint32     u32r        = 0;                                        //  temporary u32 ( for intermediate calculation )

    const   gchar   *   p1          = NULL;
    const   gchar   *   p2          = NULL;
    gboolean    sign        = FALSE;
    gboolean    negative    = FALSE;
    //  ........................................................................
    if ( ! _str )                                                               //  NULL input string !
        goto lab_failure;

    len     = strlen( _str );                                                   //  input string length

    if ( ! len )                                                                //  empty input string !
        goto lab_failure;

    //..........................................................................
    //  some tests
    if ( len > 11 )                                                             //  cf i32m & i32M, 11 chr maxi
        goto lab_failure;

    switch ( _str[0] )
    {
    case '-'    :
        sign        = TRUE;
        negative    = TRUE;
        if ( len == 1 )
            goto lab_failure;
        break;

    case '+'    :
        sign        = TRUE;
        if ( len == 1 )
            goto lab_failure;
        //  negative = FALSE;                                               //  already initialized !
        break;

    default     :
        if ( len == 11 )                                                    //  no sign : 11 chr is too much
            goto lab_failure;
        //  negative = FALSE;                                               //  already initialized !
        break;
    }

    //..........................................................................
    //  go !
    p1      = sign ? _str + 1 : _str;                                           //  p1 = first 'maybe decimal digit'
    p2      = _str + len;                                                       //  p2 = terminal 0 of the string

    while ( -- p2 >= p1 )
    {
        if ( ! libgwr::chr::ascii_dec_to_u32(*p2, &u32t) )                      //  not decimal chr
            goto lab_failure;

        u32t = u32t * s_u32_10_power[ i++ ];                                    //  finish computing chr value

        u32r = u32r + u32t;                                                     //  add to final result
        //  ....................................................................
        //  check overflows
        if ( negative )
        {
            if ( u32r > u32_i32m )
                goto lab_failure;
        }
        else
        {
            if ( u32r > u32_i32M )
                goto lab_failure;
        }
    }

    //  affect final result
    if ( negative )
        *_i32 = -u32r;
    else
        *_i32 = u32r;

    return TRUE;
    //..........................................................................
    //  failure
lab_failure:
    printf("libgwr::str::ascii_dec_to_i32():error [%s]\n", _str ? _str : "NULL");
    *_i32 = 0;
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  libgwr::str::ascii_hex_to_u32
//  ----------------------------------------------------------------------------
//! \name   ascii_hex_to_u32
//!
//! \brief  try to get a u32 from a 0-terminated string
//!     format :    0x[:xdigit:]
gboolean    ascii_hex_to_u32(const gchar* _str, guint32* _u32)
{
    //  u32Max  = 0xFFFFFFFF = (2) + 10  = 12 chr maximum

    gint        len         = 0;

    guint32     i           = 0;
    guint32     u32t        = 0;                                        //  temporary u64 ( for intermediate calculation )
    guint32     u32r        = 0;                                        //  temporary u64 ( for intermediate calculation )

    const   gchar   *   p1          = NULL;
    const   gchar   *   p2          = NULL;
    //  ........................................................................
    if ( ! _str )                                                               //  NULL input string !
        goto lab_failure;

    len     = strlen( _str );                                                   //  input string length

    if ( ! len )                                                                //  empty input string !
        goto lab_failure;
    //  ........................................................................
    if ( len > 12 )                                                             //  too much chr
        goto lab_failure;

    if ( len < 3 )                                                              //  0x?.. is not possible
        goto lab_no_0x;

    if ( ( _str[0] != '0' ) || (( _str[1] != 'x' ) && (_str[1] != 'X' )) )      //  not 0x?...
        goto lab_no_0x;

    //  init loop pointers
    p1      = _str + 2;                                                         //  p1 = first 'maybe hex digit'
    p2      = _str + len;                                                       //  p2 = terminal 0 of the string

    goto lab_loop;
    //  ........................................................................
lab_no_0x:
    if ( len > 10 )                                                             //  no 0x => 10 chr maxi
        goto lab_failure;

    //  init loop pointers
    p1      = _str;                                                             //  p1 = first 'maybe hex digit'
    p2      = _str + len;                                                       //  p2 = terminal 0 of the string
    //  ........................................................................
lab_loop:
    while ( -- p2 >= p1 )
    {
        if ( ! libgwr::chr::ascii_hex_to_u32(*p2, &u32t) )                      //  not hexadecimal representation of an int...
            goto lab_failure;

        u32t = u32t * s_u32_16_power[ i++ ];                                    //  finish computing chr value

        u32r = u32r + u32t;
    }
    //  ........................................................................
    *_u32 = (gint32)u32r;
    return TRUE;
    //  ........................................................................
lab_failure:
    printf("libgwr::str::ascii_hex_to_u32():error [%s]\n", _str ? _str : "NULL");
    *_u32 = 0;
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  libgwr::str::ascii_hex_to_i32
//  ----------------------------------------------------------------------------
//! \name   ascii_hex_to_i32
//!
//! \brief  try to get a i32 from a 0-terminated string
//!     format :    0x[:xdigit:]
gboolean    ascii_hex_to_i32(const gchar* _str, gint32* _i32)
{
    guint32 u32t = 0;

    if ( ! ascii_hex_to_u32(_str, &u32t) )
        return FALSE;

    *(_i32) = (gint32)u32t;
    return TRUE;
}
//  ----------------------------------------------------------------------------
//  libgwr::str::split
//  ----------------------------------------------------------------------------
//! \name   split
//!
//! \brief  Splits a string with a given delimiter
libgwr::TArrayP < gchar > *
            split( const gchar* _str, gchar _c)
{
    libgwr::TArrayP < gchar >   *   a   =   NULL;
    gchar                       *   p   =   NULL;                               //  left delimiter
    gchar                       *   q   =   NULL;                               //  right delimiter
    gchar                           c   =   0;
    guint32                         l   =   0;
    //  ........................................................................
    //  test that _str is zero-terminated
    l = 0;
    p = (gchar*)_str ; while ( * (p++) )
    {
        l   =   l + 1;
        if ( l >= 1024 )
            return NULL;
    }
    //  ........................................................................
    a   =   GWR_NEW_CAST( libgwr::TArrayP < gchar >, 50, 5 );

    p   =   (gchar*)_str - 1;
    q   =   (gchar*)_str - 1;

    //  ........................................................................
lab_loop:
    {

    c = ( *(++q) );

    if ( ! c )                                                                  //  end of string
        goto lab_loop_end;

    if ( c != _c )                                                              //  not a delimiter
        goto lab_loop;
    //  ........................................................................
    //  at least 1 char between 2 delimiters
    if ( ( q - p ) > 1 )
    {
        a->add( g_strndup( p + 1, q - p - 1 ) );                                //  add the string

        p   =   q;                                                              //  wrap old pointer

        goto lab_loop;
    }
    //  ........................................................................
    //  2 consecutives delimiters ( ignored )
    if ( ( q - p ) == 1 )
    {
        p   =   q;

        goto lab_loop;
    }
    //  ........................................................................
    //  string begins with a delimiter ( ignored )
    {
        // nothing
        goto lab_loop;
    }

    }
    //  ........................................................................
    //  0 terminating char is considered as a delimiter
lab_loop_end:
    {

    //  ........................................................................
    //  at least 1 char between 2 delimiters
    if ( ( q - p ) > 1 )
    {
        a->add( g_strndup( p + 1, q - p - 1 ) );                                //  add the string
    }
    //  ........................................................................
    //  2 consecutives delimiters ( ignored )
    if ( ( q - p ) == 1 )
    {
        //  no need to memorize anything, it is the end
    }
    //  ........................................................................
    //  string begins with a delimiter ( ignored )
    {
        //  no need to memorize anything, it is the end
    }

    }

    return a;
}
//  ============================================================================
//  64 bits related
//  ============================================================================
//  ----------------------------------------------------------------------------
//  guint64 max = + 18 446 744 073 709 551 615 : max = 10 ^ 19
static  guint64 s_u64_10_power[20] =
{
    1,                          //  10 ^  0
    10,
    100,
    1000,                       //  10 ^  3
    10000,
    100000,
    1000000,                    //  10 ^  6
    10000000,
    100000000,
    1000000000,                 //  10 ^  9
    10000000000,
    100000000000,
    1000000000000,              //  10 ^ 12
    10000000000000,
    100000000000000,
    1000000000000000,           //  10 ^ 15
    10000000000000000,
    100000000000000000,
    1000000000000000000,        //  10 ^ 18
    10000000000000000000        //  10 ^ 19
};
//  ----------------------------------------------------------------------------
static  guint64 s_u64_16_power[16] =
{
    0x0000000000000001,         //  16 ^   0
    0x0000000000000010,         //  16 ^   1
    0x0000000000000100,         //  16 ^   2
    0x0000000000001000,         //  16 ^   3
    0x0000000000010000,         //  16 ^   4
    0x0000000000100000,         //  16 ^   5
    0x0000000001000000,         //  16 ^   6
    0x0000000010000000,         //  16 ^   7
    0x0000000100000000,         //  16 ^   8
    0x0000001000000000,         //  16 ^   9
    0x0000010000000000,         //  16 ^  10
    0x0000100000000000,         //  16 ^  11
    0x0001000000000000,         //  16 ^  12
    0x0010000000000000,         //  16 ^  13
    0x0100000000000000,         //  16 ^  14
    0x1000000000000000          //  16 ^  15
};
//  ----------------------------------------------------------------------------
//  libgwr::str::ascii_dec_to_u64
//  ----------------------------------------------------------------------------
//! \name   ascii_dec_to_u64
//!
//! \brief  try to get a u64 integer from a 0-terminated string.
//! format : +-[0-9]* ( this kind of procedure that should be coded in asm )
gboolean    ascii_dec_to_u64(const gchar* _str, guint64* _u64)
{
    //  minimum i64 possible = -  9 223 372 036 854 775 808 in decimal representation = (1) + 19 = 20 chr maxi
    //  maximum i64 possible = +  9 223 372 036 854 775 807 in decimal representation = (1) + 19 = 20 chr maxi
    //  maximum u64 possible = + 18 446 744 073 709 551 615 in decimal representation = (1) + 20 = 21 chr maxi

    gint        len         = 0;
    const   guint64     u64M        = 0xFFFFFFFFFFFFFFFF;

    guint32     i           = 0;
    guint64     u64t        = 0;                                        //  temporary u64 ( for intermediate calculation )
    guint64     u64r        = 0;                                        //  temporary r64 ( for intermediate calculation )

    const   gchar   *   p1          = NULL;
    const   gchar   *   p2          = NULL;
    gboolean    sign        = FALSE;
    //gboolean    negative    = FALSE;
    //  ........................................................................
    if ( ! _str )                                                               //  NULL input string !
        goto lab_failure;

    len     = strlen( _str );                                                   //  input string length

    if ( ! len )                                                                //  empty input string !
        goto lab_failure;

    //..........................................................................
    //  some tests
    if ( len > 20 )                                                             //  cf i64m & i64M, 20 chr maxi
        goto lab_failure;

    switch ( _str[0] )
    {
    case '-'    :
        //sign        = TRUE;
        //negative    = TRUE;
        //if ( len == 1 )
        //    return FALSE;
        goto lab_failure;
        break;

    case '+'    :
        sign        = TRUE;
        if ( len == 1 )
            goto lab_failure;
        //  negative = FALSE;                                               //  already initialized !
        break;

    default     :
        if ( len == 20 )                                                    //  no sign : 20 chr is too much
            goto lab_failure;
        //  negative = FALSE;                                               //  already initialized !
        break;
    }

    //..........................................................................
    //  go !
    p1      = sign ? _str + 1 : _str;                                           //  p1 = first 'maybe decimal digit'
    p2      = _str + len;                                                       //  p2 = terminal 0 of the string

    while ( -- p2 >= p1 )
    {
        if ( ! libgwr::chr::ascii_dec_to_u64(*p2, &u64t) )                      //  not decimal chr
            goto lab_failure;

        u64t = u64t * s_u64_10_power[ i++ ];                                    //  finish computing chr value

        //  check overflow
        if ( u64t > ( u64M - u64r ) )
            goto lab_failure;

        u64r = u64r + u64t;                                                     //  ok, add to final result
    }

    *_u64 = u64r;

    return TRUE;
    //..........................................................................
    //  failure
lab_failure:
    printf("libgwr::str::ascii_dec_to_u64():error [%s]\n", _str ? _str : "NULL");
    *_u64 = 0;
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  libgwr::str::ascii_dec_to_i64
//  ----------------------------------------------------------------------------
//! \name   ascii_dec_to_i64
//!
//! \brief  try to get a i64 integer from a 0-terminated string.
//! format : +-[0-9]* ( this kind of procedure that should be coded in asm )
gboolean    ascii_dec_to_i64(const gchar* _str, gint64* _i64)
{
    //  minimum i64 possible = -  9 223 372 036 854 775 808 in decimal representation = (1) + 19 = 20 chr maxi
    //  maximum i64 possible = +  9 223 372 036 854 775 807 in decimal representation = (1) + 19 = 20 chr maxi
    //  maximum u64 possible = + 18 446 744 073 709 551 615 in decimal representation = (1) + 20 = 21 chr maxi

    gint        len         = 0;
    const   guint64     u64_i64m    = 0x8000000000000000;
    const   guint64     u64_i64M    = 0x7FFFFFFFFFFFFFFF;

    guint32     i           = 0;
    guint64     u64t        = 0;                                        //  temporary u64 ( for intermediate calculation )
    guint64     u64r        = 0;                                        //  temporary r64 ( for intermediate calculation )

    const   gchar   *   p1          = NULL;
    const   gchar   *   p2          = NULL;
    gboolean    sign        = FALSE;
    gboolean    negative    = FALSE;
    //  ........................................................................
    if ( ! _str )                                                               //  NULL input string !
        goto lab_failure;

    len     = strlen( _str );                                                   //  input string length

    if ( ! len )                                                                //  empty input string !
        goto lab_failure;

    //..........................................................................
    //  some tests
    if ( len > 20 )                                                             //  cf i64m & i64M, 20 chr maxi
        goto lab_failure;

    switch ( _str[0] )
    {
    case '-'    :
        sign        = TRUE;
        negative    = TRUE;
        if ( len == 1 )
            goto lab_failure;
        break;

    case '+'    :
        sign        = TRUE;
        if ( len == 1 )
            goto lab_failure;
        //  negative = FALSE;                                               //  already initialized !
        break;

    default     :
        if ( len == 20 )                                                    //  no sign : 20 chr is too much
            goto lab_failure;
        //  negative = FALSE;                                               //  already initialized !
        break;
    }

    //..........................................................................
    //  go !
    p1      = sign ? _str + 1 : _str;                                           //  p1 = first 'maybe decimal digit'
    p2      = _str + len;                                                       //  p2 = terminal 0 of the string

    while ( -- p2 >= p1 )
    {
        if ( ! libgwr::chr::ascii_dec_to_u64(*p2, &u64t) )                      //  not decimal chr
            goto lab_failure;

        u64t = u64t * s_u64_10_power[ i++ ];                                    //  finish computing che value

        u64r = u64r + u64t;                                                     //  add to final result
        //  ....................................................................
        //  check overflows
        if ( negative )
        {
            if ( u64r > u64_i64m )
            {
                printf("libgwr::str::ascii_dec_to_i64():negative overflow\n");
                goto lab_failure;
            }
        }
        else
        {
            //  check for positive overflow : stay on 64bits signed integers
            if ( u64r > u64_i64M )
            {
                printf("libgwr::str::ascii_dec_to_i64():positive overflow\n");
                goto lab_failure;
            }
        }
    }

    //  affect final result
    if ( negative )
        *_i64 = -u64r;
    else
        *_i64 = u64r;

    return TRUE;
    //..........................................................................
    //  failure
lab_failure:
    printf("libgwr::str::ascii_dec_to_i64():error [%s]\n", _str ? _str : "NULL");
    *_i64 = 0;
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  libgwr::str::ascii_hex_to_u64
//  ----------------------------------------------------------------------------
//! \name   ascii_hex_to_u64
//!
//! \brief  try to get a u64 integer from a 0-terminated string.
//! format : [:xdigit:] ( this kind of procedure that should be coded in asm )
gboolean    ascii_hex_to_u64(const gchar* _str, guint64* _u64)
{
    gint        len         = 0;

    guint32     i           = 0;
    guint64     u64t        = 0;                                        //  temporary u64 ( for intermediate calculation )
    guint64     u64r        = 0;                                        //  temporary u64 ( for intermediate calculation )

    const   gchar   *   p1          = NULL;
    const   gchar   *   p2          = NULL;
    //  ........................................................................
    if ( ! _str )                                                               //  NULL input string !
        goto lab_failure;

    len     = strlen( _str );                                                   //  input string length

    if ( ! len )                                                                //  empty input string !
        goto lab_failure;
    //  ........................................................................
    if ( len > 18 )                                                             //  too much chr
        goto lab_failure;

    if ( len < 3 )                                                              //  0x?.. is not possible
        goto lab_no_0x;

    if ( ( _str[0] != '0' ) || (( _str[1] != 'x' ) && (_str[1] != 'X' )) )      //  not 0x?...
        goto lab_no_0x;

    //  init loop pointers
    p1      = _str + 2;                                                         //  p1 = first 'maybe hex digit'
    p2      = _str + len;                                                       //  p2 = terminal 0 of the string

    goto lab_loop;
    //  ........................................................................
lab_no_0x:
    if ( len > 16 )                                                             //  no 0x => 16 chr maxi
        goto lab_failure;

    //  init loop pointers
    p1      = _str;                                                             //  p1 = first 'maybe hex digit'
    p2      = _str + len;                                                       //  p2 = terminal 0 of the string
    //  ........................................................................
lab_loop:
    while ( -- p2 >= p1 )
    {
        if ( ! libgwr::chr::ascii_hex_to_u64(*p2, &u64t) )                      //  not hexadecimal representation of an int...
            goto lab_failure;

        u64t = u64t * s_u64_16_power[ i++ ];                                    //  finish computing chr value

        u64r = u64r + u64t;
    }
    //  ........................................................................
    *_u64 = (guint64)u64r;
    return TRUE;
    //  ........................................................................
lab_failure:
    printf("libgwr::str::ascii_hex_to_u64():error [%s]\n", _str ? _str : "NULL");
    *_u64 = 0;
    return FALSE;
}
//  ----------------------------------------------------------------------------
//  libgwr::str::ascii_hex_to_i64
//  ----------------------------------------------------------------------------
//! \name   ascii_hex_to_i64
//!
//! \brief  try to get a i64 integer from a 0-terminated string.
//! format : [:xdigit:] ( this kind of procedure that should be coded in asm )
gboolean    ascii_hex_to_i64(const gchar* _str, gint64* _i64)
{
    guint64 u64t    = 0;

    if ( ! ascii_hex_to_u64(_str, &u64t) )
        return FALSE;

    *(_i64) = (gint64)u64t;
    return TRUE;
}

}
//  ****************************************************************************
//
//  dstr
//
//  ****************************************************************************
namespace dstr
{

void        clr     ( gchar*& _dyn_str )    {   _dyn_str    =   NULL;               }
void        del     ( gchar*& _dyn_str )    {   g_free( _dyn_str );                 }
void       sdel     ( gchar*& _dyn_str )    {   g_free_safe( _dyn_str );            }
void        rst     ( gchar*& _dyn_str )    {   sdel( _dyn_str ); clr( _dyn_str );  }
void        rep     ( gchar*& _dyn_str, gchar* _dyn_str_rep)
                                            {   _dyn_str = _dyn_str_rep;            }

void        set     ( gchar*& _dyn_str, const gchar*  _s )
                                            {
                                                sdel( _dyn_str );
                                                _dyn_str = g_strdup( _s );
                                            }
//  ----------------------------------------------------------------------------
//  libgwr::dstr::append
//  ----------------------------------------------------------------------------
//! \namespace  libgwr::dstr
//! \fn         append
//!
//! \brief  Replace a dstr by itself + a concatenation of newly allocated
//!         gchar*. Old gchar* will be freed. NULL sentinel.
//!
//! \param[in,out]  _dyn_str    The gchar* that will be replaced ( NULL ALLOWED ).
//! \param[in]      va_list     The gchar* list to copy.
void        append(gchar*& _dyn_str, ...)
{
    va_list                 vl;
    const   gchar       *   p       = NULL;
            gchar       *   u       = NULL;
            gchar       *   v       = NULL;
    //  ........................................................................
    u   =   _dyn_str ? g_strdup( _dyn_str ) : g_strdup( "" );

    va_start( vl, _dyn_str );

    p = va_arg(vl, const gchar*); while ( p )
    {
        v = u;
        u = g_strconcat( u, p , NULL );
        g_free( v );
        p = va_arg(vl, const gchar*);
    }

    va_end(vl);

    libgwr::dstr::set( _dyn_str, u );
}

void        vset(gchar*& _dyn_str, ...)
{
    //  ........................................................................
    va_list                 vl;
    const   gchar       *   p       = NULL;
            gchar       *   u       = NULL;
            gchar       *   v       = NULL;
    //  ........................................................................
    //  free actual string
    libgwr::dstr::rst( _dyn_str );

    //  append other strs
    u   =   g_strdup( "" );                                                     //  init u because of g_free( v ) below

    va_start( vl, _dyn_str );

    p = va_arg(vl, const gchar*); while ( p )
    {
        v = u;
        u = g_strconcat( u, p , NULL );
        g_free( v );
        p = va_arg(vl, const gchar*);
    }

    va_end(vl);

    _dyn_str = u;
}

}
//  ****************************************************************************
//
//  Env utilities
//
//  ****************************************************************************
namespace   env
{
namespace   str
{

static  libgwr::TArrayP < gchar >   p_rsv_vars_replaced     (50, 10);
static  libgwr::TArrayP < gchar >   p_rsv_vars_not_found    (50, 10);

const   libgwr::TArrayP < gchar >   *
rsv_vars_replaced()
{
    return  &p_rsv_vars_replaced;
}

const   libgwr::TArrayP < gchar >   *
rsv_vars_not_found()
{
    return  &p_rsv_vars_not_found;
}

const gchar*
replace_shell_variables(const gchar*  _str)
{
    static  gchar       str [1024];
    static  gchar       var [256];
    gchar   *   val = NULL;

    gchar   *   p   = NULL;
    gchar   *   q   = NULL;
    gchar   *   r   = NULL;

    gchar       c   = 0;
    //..........................................................................
    p_rsv_vars_replaced.reset();
    p_rsv_vars_not_found.reset();
    bzero(str, 1024);
    p   =   const_cast< gchar* > ( _str );
    r   =   str;
    //  ........................................................................
lab_loop:
    c = * ( p++ );

    //  detect end of string
    if ( ! c )
    {
        * ( r++ ) = c;
        goto lab_end;
    }

    //  detect '$'
    if ( c != '$' )
    {
        * ( r++ ) = c;
        goto lab_loop;
    }
    //  ........................................................................
    //  '$' detected
    q   =   var;

    c   = * ( p++ );

    if ( ! c )                                                                  //  '$'\000
    {
        * ( r++ ) = '$';
        * ( r++ ) = c;
        goto lab_end;
    }

    if ( libgwr::chr::is_dec_digit(c) )                                         //  shell VARS dont begin with [0-9]
    {                                                                           //  although [0-9] is a valid shell var char
        * ( r++ ) = '$';
        * ( r++ ) = c;
        goto lab_loop;
    }

    if ( ! libgwr::chr::is_shell_variable_character(c) )                        //  not a shell var char
    {
        * ( r++ ) = '$';
        * ( r++ ) = c;
        goto lab_loop;
    }

    * ( q++ )   = c;                                                            //  store first char of VAR

    //  ........................................................................
lab_var_loop:
    c   = * ( p++ );

    if ( ! c )
        goto lab_var_replace_and_end;

    if ( ! libgwr::chr::is_shell_variable_character(c) )
        goto lab_var_replace;

    * ( q++ )   = c;
    goto lab_var_loop;

    //  ........................................................................
lab_var_replace:
    * ( q )     = 0;                                                            //  terminate VAR string

    val = getenv(var);

    if (  ! val )                                                               //  no VAR in environment
    {
        strcat(r, "$");
        strcat(r, var);
        p_rsv_vars_not_found.add( var );
        LIBGWR_ERR("env::replace_shell_variables():$%s not found in environment", var);
    }
    else
    {
        strcat(r, val);
        p_rsv_vars_replaced.add( var );
    }

    r   = str + strlen(str);
    * ( r++ )   = c;                                                            //  append c

    goto lab_loop;                                                              //  continue scan
    //  ........................................................................
lab_var_replace_and_end:
    * ( q )     = 0;                                                            //  terminate VAR string

    val = getenv(var);

    if (  ! val )                                                               //  no VAR in environment
    {
        strcat(r, "$");
        strcat(r, var);
        p_rsv_vars_not_found.add( var );
        LIBGWR_ERR("env::replace_shell_variables():$%s not found in environment", var);
    }
    else
    {
        strcat(r, val);
        p_rsv_vars_replaced.add( var );
    }

lab_end:
    return str;

}   //  str

}   //  str
}   //  env
//  ****************************************************************************
//
//  Divers
//
//  ****************************************************************************
template<>  guint32     TArray< guint32 >::Null =   0;
//  ============================================================================
HString::HString()
{
    a_h     = 0;
}
HString::HString(const gchar* _str)
{
    a_h     = gwr_str_hash( d_str );
}
HString::~HString()
{
}
void
HString::set(const gchar* _str)
{
    String::set( _str );
    a_h     = gwr_str_hash( d_str );
}
void
HString::vset(const gchar* _str_1, ...)
{
    //  ........................................................................
    va_list                 vl;
    const   gchar       *   p       = NULL;
            gchar       *   u       = NULL;
            gchar       *   v       = NULL;
    //  ........................................................................
    //  set us to 1st str
    libgwr::dstr::rst( d_str );

    //  append other strs
    u   =   g_strdup( _str_1 );                                                 //  init u because of g_free( v ) below

    va_start( vl, _str_1 );

    p = va_arg(vl, const gchar*); while ( p )
    {
        v = u;
        u = g_strconcat( u, p , NULL );
        g_free( v );
        p = va_arg(vl, const gchar*);
    }

    va_end(vl);

    d_str = u;

    a_h     = gwr_str_hash( d_str );
}
//  ============================================================================
        void        String::p0_clr()                    {   libgwr::dstr::clr(d_str);           }
        void        String::p0_del()                    {   libgwr::dstr::del(d_str);           }
        void        String::del()                       {   g_free_safe( d_str );               }
        void        String::rst()                       {   del() ; p0_clr();                   }
        void        String::rep(      gchar* _dyn_str)  {   del() ; d_str = _dyn_str;           }
        void        String::set(const gchar* _str)      {   del(); d_str   = g_strdup( _str );  }
const   gchar   *   String::get()                       {   return (const gchar*)d_str;         }

void
String::append(const gchar* _str_1, ...)
{
    //  ........................................................................
    va_list                 vl;
    const   gchar       *   p       = NULL;
            gchar       *   u       = NULL;
            gchar       *   v       = NULL;
    //  ........................................................................
    //  append 1st str to us

    u       =   d_str;
    d_str   =   g_strconcat( d_str, _str_1, NULL );
    g_free_safe(u);
    //u   =   g_strconcat( d_str, _str_1, NULL );
    //libgwr::dstr::rep( d_str, u );

    //  append other strs
    u   =   d_str;                                                              //  init u because of g_free( v ) below

    va_start( vl, _str_1 );

    p = va_arg(vl, const gchar*); while ( p )
    {
        v = u;
        u = g_strconcat( u, p , NULL );
        g_free( v );
        p = va_arg(vl, const gchar*);
    }

    va_end(vl);

    d_str = u;
}

void
String::vset(const gchar* _str_1, ...)
{
    //  ........................................................................
    va_list                 vl;
    const   gchar       *   p       = NULL;
            gchar       *   u       = NULL;
            gchar       *   v       = NULL;
    //  ........................................................................
    //  set us to 1st str
    libgwr::dstr::rst( d_str );

    //  append other strs
    u   =   g_strdup( _str_1 );                                                 //  init u because of g_free( v ) below

    va_start( vl, _str_1 );

    p = va_arg(vl, const gchar*); while ( p )
    {
        v = u;
        u = g_strconcat( u, p , NULL );
        g_free( v );
        p = va_arg(vl, const gchar*);
    }

    va_end(vl);

    d_str = u;
}

String::String()
{
    p0_clr();
}
String::String(const gchar* _str)
{
    p0_clr();
    set( _str );
}
String::~String()
{
    rst();
}
//  ****************************************************************************
//
//  GLib extensions
//
//  ****************************************************************************
guint32     g_ptr_array_find_first_free_index   (GPtrArray* _array, gboolean _allocate)
{
    g_return_val_if_fail(_array, 0);

    for ( guint32 i = 0 ; i != _array->len ; i++ )
        if ( _array->pdata[i] == NULL )
            return i;

    if ( ! _allocate )
        return G_MAXUINT32;

    g_ptr_array_add(_array, NULL);
    return ( _array->len - 1 );
}
gpointer    g_ptr_array_get                     (GPtrArray* _array, guint32 _index)
{
    g_return_val_if_fail( _array                        , NULL);
    g_return_val_if_fail( _index < (guint32)_array->len , NULL );

    return g_ptr_array_index(_array, _index);
}
gboolean    g_ptr_array_set                     (GPtrArray* _array, guint32  _index, gpointer _data)
{
    g_return_val_if_fail( _array                        , FALSE );
    g_return_val_if_fail( _index < (guint32)_array->len , FALSE );

    _array->pdata[_index] = _data;

    return TRUE;
}

GWR_NAMESPACE_END(libgwr)

