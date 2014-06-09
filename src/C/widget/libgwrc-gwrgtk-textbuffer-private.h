/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-gwrgtk-textbuffer-private.h                                     *
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
    *   Purpose : Private data for GwrGtkTextBuffer                             *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWRC_GWRGTK_TEXT_BUFFER_PRIVATE_H__
#define __LIBGWRC_GWRGTK_TEXT_BUFFER_PRIVATE_H__
//  ............................................................................
#include    "C/libgwrc-common.h"
#include    "C/libgwrc-array-data-multi.h"
//  ............................................................................

typedef struct  _GwrGtkTextBufferLineDesc   GwrGtkTextBufferLineDesc;

struct  _GwrGtkTextBufferLineDesc
{
            guint32     a_attributes;                                           //!< see libgwr-text-attributes.h
}   __attribute__ ((packed));

typedef struct  _GwrGtkTextBufferPrivate    GwrGtkTextBufferPrivate;

struct  _GwrGtkTextBufferPrivate
{
    GwrCArrayDataMulti  *   d_lines_text;                                       //!< Contain all lines text
    GwrCArrayDataMulti  *   d_lines_desc;                                       //!< Contain all lines metadata

    guint32                 lines_card;
    guint32                 lines_max_len;                                      //!< Longest line length ( in char ) in buffer
};

#endif                                                                          //  __LIBGWRC_GWRGTK_TEXT_BUFFER_PRIVATE_H__
