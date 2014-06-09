/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-gen-log-parser.h                                                 *
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
    *   Class   : Parser                                                        *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *   Purpose : Class for parsing a linux kernel's configuration              *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_GEN_LOG_PARSER_H__
#define     __LIBGWR_GEN_LOG_PARSER_H__

//  ............................................................................
#include    "CC/gen-log/libgwr-gen-log-parsing.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(log)
GWR_NAMESPACE_START(parser)

//  ****************************************************************************
//   Enums / typedef
//  ****************************************************************************
//  ****************************************************************************
//   Vars / Funcs
//  ****************************************************************************
extern  gchar       *       Status();
extern  gboolean            Reset();
extern  void                Stop();

extern  gboolean            Set_themes_preformats();
extern  gboolean            Set_namespaces_prefix();
//  ----------------------------------------------------------------------------
extern          void                    Lexer_error_syntax  ();
extern          void                    Lexer_error_jammed  ();

extern          void                    Namespaces_prefix           (const gchar* _str);
extern          void                    Channels_prefix             (const gchar* _str);
extern          void                    File_h                      (const gchar* _str);
extern          void                    File_cc                     (const gchar* _str);
extern          void                    Global_logger_pointer       (const gchar* _str);

extern          void                    Nsp_enter                   (const gchar*);
extern          void                    Nsp_exit                    (const gchar*);
extern          void                    Nsp_macros_prefix           (const gchar*);

extern          void                    Nsp_preformat_lst           (guint32, parsing::tPrfLst*);
extern          void                    Nsp_preformat_fmt           (guint32, const gchar*);
extern          void                    Nsp_preformat_arg           (guint32, const gchar*);

extern          void                    Nsp_theme_create            (const gchar* _thm_name);

extern          void                    Log_theme_parameter         (const gchar* _param_name);
extern          void                    Log_theme_channel           (const gchar* _cha_name);
extern          void                    Log_theme_macro             (const gchar* _macro_name);

extern          void                    Log_theme_flavour_new       (const gchar* _sub_name);
extern          void                    Log_theme_flavour_color_fg  (const gchar* _fg);
extern          void                    Log_theme_flavour_color_bg  (const gchar* _bg);
extern          void                    Log_theme_flavour_format    (const gchar* _format);

GWR_NAMESPACE_END(parser)
GWR_NAMESPACE_END(log)
GWR_NAMESPACE_END(libgwr)

#endif                                                                          //  __LIBGWR_GEN_LOG_PARSER_H__
