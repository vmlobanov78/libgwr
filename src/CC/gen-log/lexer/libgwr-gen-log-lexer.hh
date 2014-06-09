/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-gen-log-lexer.h                                                  *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libkconfig                                                      *
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
    *   Class   : Lexer                                                         *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *   Purpose : Class for lexing linux kernel's configuration. Since I have   *
    *   begun to write the parser ( bison ), the only utility of this class     *
    *   is only to debug the lexer.                                             *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_GEN_LOG_LEXER_H__
#define     __LIBGWR_GEN_LOG_LEXER_H__

//  ............................................................................
#include    "CC/gen-log/libgwr-gen-log-parsing.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(log)
GWR_NAMESPACE_START(lexer)
//  ****************************************************************************
//   Enums / typedef
//  ****************************************************************************
enum
{
    eStatus_ok                          =       0                               ,
    eStatus_error_jammed                =       1                               ,
    eStatus_error_internal              =       2                               ,
    eStatus_error_syntax                =       3                               ,
    eStatus_stopped_by_external         =       4
};
//  ****************************************************************************
//   Vars / Funcs
//  ****************************************************************************
extern  gchar   *           Status();
extern  void                Status_set(guint32 _status);

extern  void                Stop();

extern  int                 Token(int _token);

extern  int                 Token_EOF();
extern  void                Token_EOF_set_to_zero();

extern  void                Fatal_error(const gchar* msg);                      //  YY_FATAL_ERROR

extern  gboolean            Reset();

GWR_NAMESPACE_END(lexer)
GWR_NAMESPACE_END(log)
GWR_NAMESPACE_END(libgwr)


#endif                                                                          // #define __LIBGWR_GEN_LOG_LEXER_H__
