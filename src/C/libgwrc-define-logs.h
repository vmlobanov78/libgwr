/*
    *****************************************************************************
    *                                                                           *
    *   libgwrc-define-logs.h                                                   *
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
    *   Class   : Object                                                        *
    *                                                                           *
    *   Purpose :                                                               *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWRC_DEFINE_LOGS_H__
#define     __LIBGWRC_DEFINE_LOGS_H__

//  ****************************************************************************
//  core log settings
//  ****************************************************************************
#define     LIBGWR_LOG_LEVEL            3

#define     LIBGWR_LOG_WIDGET_LOG_TUNE  3
//  ****************************************************************************
//  core log functions
//  ****************************************************************************
#define     LIBGWR_LOG(FORMAT, ...)     printf( FORMAT, __VA_ARGS__ );

#if  ( LIBGWR_LOG_LEVEL > 0 )
    #define     LIBGWR_INF(FORMAT, ...)     LIBGWR_LOG( "\x1b[32m" "INF:"            FORMAT "\n", __VA_ARGS__ );
#else
    #define     LIBGWR_INF(FORMAT, ...)
#endif

#if  ( LIBGWR_LOG_LEVEL > 1 )
    #define     LIBGWR_WNG(FORMAT, ...)     LIBGWR_LOG( "\x1b[33m" "WNG:" "\x1b[32m" FORMAT "\n", __VA_ARGS__ );
#else
    #define     LIBGWR_WNG(FORMAT, ...)
#endif

#if  ( LIBGWR_LOG_LEVEL > 2 )
    #define     LIBGWR_ERR(FORMAT, ...)     LIBGWR_LOG( "\x1b[31m" "ERR:" "\x1b[32m" FORMAT "\n", __VA_ARGS__ );
#else
    #define     LIBGWR_ERR(FORMAT, ...)
#endif








#endif                                                                          //  __LIBGWRC_DEFINE_LOGS_H__
