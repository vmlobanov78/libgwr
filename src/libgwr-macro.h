/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-macro.h                                                          *
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
    *   Class   : None                                                          *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWR_MACRO_H__
#define __LIBGWR_MACRO_H__

//  ***************************************************************************
//
//  Some macros
//
//  ***************************************************************************
#define GWR_STRINGIZE(a)    #a
#define GWR_LABEL           __attribute__ ((unused)) asm("nop");
#define g_free_safe(p)      if ( p ) g_free(p);

#define _GWR_BREAK_                                                             \
    __asm__("int $3");                                                          \
    __asm__("nop");

#define GWR_NAMESPACE_START(NAME)                                               \
namespace NAME                                                                  \
{

#define GWR_NAMESPACE_END(NAME)                                                 \
}

#define _gwr_virtual_

#include    "libgwr-macro-bits.h"
#include    "libgwr-macro-console.h"

#define LIBGWR_GTK_EVENT_KEY_PROPAGATE_YES      FALSE
#define LIBGWR_GTK_EVENT_KEY_PROPAGATE_NO       TRUE





#endif //__LIBGWR_MACRO_H__
