/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-fast-text-buffer-private.h                                      *
    *                                                                           *
    *   --------------------------------------------------------------------    *
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
    *   Purpose : Private data for GwrFastTextBuffer                             *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWRC_FAST_TEXT_BUFFER_PRIVATE_H__
#define __LIBGWRC_FAST_TEXT_BUFFER_PRIVATE_H__
//  ............................................................................
#include    "C/arrays/libgwrc-array-data-multi.h"
//  ............................................................................

//  f   =   foreground
//  b   =   background
//  s   =   style
//  l   =   length  max = 2^9 - 1 = 511
//
//  76543210 76543210 76543210 76543210
//  00000000 00000000 00000000 00000000
//  00000000 0lllllll llssssbb bbbfffff
//  0sssssss ssssslll llllllbb bbbfffff
enum
{
    eGFTB_LD_FG_MASK    =   (guint32)   0x0000001f      ,
    eGFTB_LD_FG_OFFSET  =   (guint32)   0               ,

    eGFTB_LD_BG_MASK    =   (guint32)   0x000003e0      ,
    eGFTB_LD_BG_OFFSET  =   (guint32)   5               ,

    eGFTB_LD_ST_MASK    =   (guint32)   0x00003c00      ,
    eGFTB_LD_ST_OFFSET  =   (guint32)   10              ,

    eGFTB_LD_LN_MASK    =   (guint32)   0x0007fc00      ,
    eGFTB_LD_LN_OFFSET  =   (guint32)   10
};

typedef struct  _GwrFastTextBufferLineDesc   GwrFastTextBufferLineDesc;

struct  _GwrFastTextBufferLineDesc
{
    guint32     a_data1;
}   __attribute__ ((packed));

typedef struct  _GwrFastTextBufferPrivate    GwrFastTextBufferPrivate;

struct  _GwrFastTextBufferPrivate
{
    GwrCArrayDataMulti  *   d_lines_text;                                       //!< Contain all lines text
    GwrCArrayEqual      *   d_lines_desc;                                       //!< Contain all lines metadata

    guint32                 lines_card;
    guint32                 lines_max_len;                                      //!< Longest line length ( in char ) in buffer
};

#endif                                                                          //  __LIBGWRC_FAST_TEXT_BUFFER_PRIVATE_H__
