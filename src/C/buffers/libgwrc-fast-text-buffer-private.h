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
    *   Purpose : Private data for GwrFastTextBuffer                            *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWRC_FAST_TEXT_BUFFER_PRIVATE_H__
#define __LIBGWRC_FAST_TEXT_BUFFER_PRIVATE_H__
//  ............................................................................
#include    "C/arrays/libgwrc-array-data-multi.h"
//  ............................................................................
enum
{
    //  GwrFastTextBufferLineDesc
    //
    //  f   =   foreground  ( max 32 colors )
    //  b   =   background  ( max 32 colros )
    //  s   =   style       ( max 4 flags )
    //  x   =   extra data    : index size
    //  g   =   segments data : index size
    //
    //  76543210 76543210 76543210 76543210
    //  00000000 00000000 00000000 00000000
    //  xxxggg00 00000000 00ssssbb bbbfffff
    eGFTB_LD_FG_MASK    =   (guint32)   0x0000001f      ,
    eGFTB_LD_FG_OFFSET  =   (guint32)   0               ,

    eGFTB_LD_BG_MASK    =   (guint32)   0x000003e0      ,
    eGFTB_LD_BG_OFFSET  =   (guint32)   5               ,

    eGFTB_LD_ST_MASK    =   (guint32)   0x0000c000      ,
    eGFTB_LD_ST_OFFSET  =   (guint32)   10              ,

    eGFTB_LD_DS_MASK    =   (guint32)   0xe0000000      ,
    eGFTB_LD_DS_OFFSET  =   (guint32)   29              ,

    eGFTB_LD_US_MASK    =   (guint32)   0x1c000000      ,
    eGFTB_LD_US_OFFSET  =   (guint32)   26              ,
    //  ........................................................................
    //  GwrFastTextBufferXduDesc
    //
    //  o   =   offset  ( max 512 )
    //  l   =   len     ( max 128 chars )
    //
    //  76543210 76543210
    //  00000000 00000000
    //  lllllllo oooooooo
    eGFTB_XDU_OF_MASK   =   (guint16)   0x01ff          ,
    eGFTB_XDU_OF_OFFSET =   (guint32)   0               ,

    eGFTB_XDU_LN_MASK   =   (guint16)   0xfe00          ,
    eGFTB_XDU_LN_OFFSET =   (guint32)   9
};
//  ............................................................................
typedef struct  _GwrFastTextBufferLineDesc      GwrFastTextBufferLineDesc;

struct  _GwrFastTextBufferLineDesc
{
    guint32     a_data1;
}   __attribute__ ((packed));
//  ............................................................................
typedef struct  _GwrFastTextBufferSegmentDesc   GwrFastTextBufferSegmentDesc;

struct  _GwrFastTextBufferSegmentDesc
{
    guint32     a_data1;
}   __attribute__ ((packed));
//  ............................................................................
typedef struct  _GwrFastTextBufferXduDesc       GwrFastTextBufferXduDesc;

struct  _GwrFastTextBufferXduDesc
{
    guint16     a_data1;
}   __attribute__ ((packed));
//  ............................................................................
typedef struct  _GwrFastTextBufferPrivate       GwrFastTextBufferPrivate;

struct  _GwrFastTextBufferPrivate
{
    GwrCArrayDataMulti  *   d_lines_text;                                       //!< Contain all lines text
    GwrCArrayEqualSimple*   d_lines_desc;                                       //!< Contain all lines lines metadata
    GwrCArrayDataMulti  *   d_extra_data;                                       //!< Contain all lines extra datas
    GwrCArrayEqual      *   d_xdu_offsets;                                      //!< Contain all lines urls offsets

    guint32                 lines_card;
    guint32                 lines_max_len;                                      //!< Longest line length ( in char ) in buffer

    //GwrCArrayDataMulti  *   d_segments_data;                                    //!< Contain all lines segments metadatas
    //struct
    //{
    //    gchar                   a_line          [512];                          //!< Segmented line beeing constructed
    //    guint32                 a_line_len;                                     //!< Length of a_line

    //    guint32                 a_descs[8];                                     //!< Maximum 8 segments in one line

    //    guint32                 a_state;
    //}
    //                        segments;
};

#endif                                                                          //  __LIBGWRC_FAST_TEXT_BUFFER_PRIVATE_H__
