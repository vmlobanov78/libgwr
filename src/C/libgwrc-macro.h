/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-macro.h                                                         *
    *                                                                           *
    *   --------------------------------------------------------------------    *
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
    *   Purpose : Some macros + include all libgwrc macro headers               *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWRC_MACRO_H__
#define __LIBGWRC_MACRO_H__

#include    "libgwrc-macro-bits.h"
#include    "libgwrc-macro-console.h"

//  ----------------------------------------------------------------------------
#define GWR_GTK_VERSION_GE(X,Y)                                                 \
    (                                                                           \
        ( GTK_MAJOR_VERSION >= X )      &&                                      \
        ( GTK_MINOR_VERSION >= Y )                                              \
    )

#define GWR_STRINGIZE(a)    #a

#define _GWR_BREAK_                                                             \
    __asm__("int $3");                                                          \
    __asm__("nop");

#define LIBGWR_GTK_EVENT_KEY_PROPAGATE_YES      FALSE
#define LIBGWR_GTK_EVENT_KEY_PROPAGATE_NO       TRUE


#define GWR_RETURN_VAL_IF_FAIL( CONDITION, VALUE )                              \
    if ( ! ( CONDITION ) )                                                      \
    {                                                                           \
        LIBGWR_ERR( "libgwr:condition [%s] failed", #CONDITION )                \
        return VALUE;                                                           \
    }

#define GWR_IF_FALSE( CODE, CONDITION )                                         \
    CODE;                                                                       \
    if ( ! ( CONDITION ) )

#define GWR_IF_TRUE( CODE, CONDITION )                                          \
    CODE;                                                                       \
    if ( CONDITION )




#endif                                                                          //  __LIBGWRC_MACRO_H__
