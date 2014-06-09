/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-gen-log-parsing.h                                                *
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
    *   Class   : None                                                          *
    *                                                                           *
    *   Purpose : Class for parsing a linux kernel's configuration              *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_GEN_LOG_PARSING_H__
#define     __LIBGWR_GEN_LOG_PARSING_H__
//  ............................................................................
#include    "libgwr.hh"
#include    "CC/gen-log/libgwr-gen-log-parsing-classes.hh"
#include    "CC/gen-log/lexer/libgwr-gen-log-lexer.hh"
#include    "CC/gen-log/parser/libgwr-gen-log-parser.hh"
//  ############################################################################
//
//  Some flex stuff that we need to know about
//
//  ############################################################################
extern      int                 yylex(void);
extern      FILE            *   yyin;
extern      int                 yylineno;
//  ############################################################################
//
//  Some bison stuff that we need to know about
//
//  ############################################################################
extern      int                 yydebug;
extern      void                yyerror(const char* _msg);
extern      int                 yyparse();
struct      yy_buffer_state;
extern      yy_buffer_state *   yy_create_buffer(FILE *file,int size);
extern      void                yypush_buffer_state (yy_buffer_state* new_buffer);
extern      void                yypop_buffer_state  ();
//  ############################################################################
//
//  libkconfig::parsing
//
//  ############################################################################
/*
inline      void    LIBGWR_GEN_LOG_LOG(const gchar* FORMAT, ...)
{
    va_list args;

    va_start    (args, FORMAT);
    vprintf     (FORMAT, args);
    va_end      (args);
}
*/
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(log)
GWR_NAMESPACE_START(parsing)
//  ****************************************************************************
//   Defines
//  ****************************************************************************
#define     LIBGWR_GEN_LOG_LOG(FORMAT, ...)     printf( FORMAT, __VA_ARGS__);


#define     LIBGWR_GEN_LOG_INF(FORMAT, ...)     LIBGWR_GEN_LOG_LOG( "\x1b[32m" "INF:"            "%s" FORMAT "\n", libgwr::log::parsing::ga_spacer.get(), __VA_ARGS__ );
#define     LIBGWR_GEN_LOG_WNG(FORMAT, ...)     LIBGWR_GEN_LOG_LOG( "\x1b[33m" "WNG:" "\x1b[32m" "%s" FORMAT "\n", libgwr::log::parsing::ga_spacer.get(), __VA_ARGS__ );
#define     LIBGWR_GEN_LOG_ERR(FORMAT, ...)     LIBGWR_GEN_LOG_LOG( "\x1b[31m" "ERR:" "\x1b[32m" "%s" FORMAT "\n", libgwr::log::parsing::ga_spacer.get(), __VA_ARGS__ );
//  ****************************************************************************
//   Enums / typedef
//  ****************************************************************************
//  ****************************************************************************
//   Vars / Func
//  ****************************************************************************
extern          libgwr::Spacer      ga_spacer;                                  //!< Spacing for namespaces

extern          GSList                      *       Namespaces_list();
extern          tStringArray                *       Namespaces_prefixes();
extern  const   gchar                       *       Namespaces_prefix();
//  ============================================================================
extern          gboolean                            Namespaces_prefix_add   (const gchar* _str);
extern  const   gchar                       *       Channels_prefix         ();
extern          gboolean                            Channels_prefix_set     (const gchar* _str);
extern  const   gchar                       *       File_def                ();
extern  const   gchar                       *       File_h                  ();
extern  const   gchar                       *       File_cc                 ();
extern  const   gchar                       *       Global_logger_pointer   ();
extern          gboolean                            File_def_set                (const gchar* _str);
extern          gboolean                            File_h_set                  (const gchar* _str);
extern          gboolean                            File_cc_set                 (const gchar* _str);
extern          gboolean                            Global_logger_pointer_set   (const gchar* _str);

extern          NSP                         *       Nsp_current();
extern          void                                Nsp_add(NSP*);
extern          void                                Nsp_push(NSP*);
extern          gboolean                            Nsp_pop(NSP*&);

extern          guint32                             Thm_card();
extern          void                                Thm_card_inc();
extern          THM                         *       Thm_current();
extern          void                                Thm_current_set(THM*);

extern          guint32                             Fla_card();
extern          void                                Fla_card_inc();
extern          FLA                         *       Fla_current();
extern          void                                Fla_current_set(FLA*);

extern          CHA                         *       Channel(const gchar* _channel_name);

extern          gboolean                Free();
extern          gboolean                Reset();
extern          gboolean                Parse();
extern          gboolean                Lex();

GWR_NAMESPACE_END(parsing)
GWR_NAMESPACE_END(log)
GWR_NAMESPACE_END(libgwr)

#endif                                                                          // __LIBGWR_GEN_LOG_PARSING_H__




