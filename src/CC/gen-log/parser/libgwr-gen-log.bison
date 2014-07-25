/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-log-generation-parsing.bison                                     *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libkconfig                                                      *
    *                                                                           *
    *   Copyright (C) 2011-2014 Guillaume Wardavoir                             *
    *   Copyright (C) 2002      Roman Zippel        <zippel@linux-m68k.org>     *
    *   Copyright (C) xxxx-xxxx Linux kernel's documentation team               *
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
    *   Class   : Main                                                          *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *   Purpose : bison file for creating a kconfig parser                      *
    *                                                                           *
    *   Notes   :                                                               *
    *                                                                           *
    *       - The actions are written in C++, but the parser himself is _NOT_   *
    *         a "C++ parser"                                                    *
    *                                                                           *
    *****************************************************************************
*/

%{
//  ============================================================================
//  INCLUDES
//  ============================================================================

// Use glib ( dont include libgwr here, not needed )
#include    <glib-2.0/glib.h>

// C++ parser class, for avoiding bunch of code here
#include    "CC/gen-log/libgwr-gen-log-parsing.hh"

//  ============================================================================
//  exp (nonterminal target) variables
//  ============================================================================

//  ============================================================================
//  EXTERNs
//  ============================================================================

// stuff from flex that bison needs to know about:
    //extern  int         yylex();
    //extern  int         yyparse();
    //extern  FILE    *   yyin;
    //void                yyerror(char *s);
extern      int         yylex();
extern      void        yyerror(const char*);

// C++ parser instance, for avoiding bunch of code here
// unuseful cf option %parse-param below
// extern libkconfig::Parser  *   parser;

// some macros

//  ============================================================================
//  DEBUGGING
//  ============================================================================

// built in bison debug mechanism
//#define YYDEBUG 1
extern int yydebug;

// custom
//#define LIBKCONFIG_DEBUG_BISON


//  ============================================================================
//  FLEX / BISON struct
//  ============================================================================

int bison_entry_count = 0;

%}



/*
 *
 *******************************************************************************
 *
 *                              DECLARATIONS
 *
 *******************************************************************************
 *
 */
                                                                                /*
%type   = non-terminal  symbol
%token  = terminal      symbol
                                                                                */

//%define api pure
%code requires                                                                  {
        # include  "libgwr.hh"
    }

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union                                                                          {
        gchar                           *   vstr;
        gint                                vint;
        void                            *   vvoid;
        gchar                               vchr;
    }

%debug

//  ============================================================================
//  Terminals
//  ============================================================================
%token  <vstr>  TOKEN_STD_STRING
%token  <vvoid> TOKEN_STD_STRING_ARRAY

// Lexer errors
%token          TOKEN_LEXER_ERROR_SYNTAX
%token          TOKEN_LEXER_ERROR_JAMMED

%token          TOKEN_LEXER_EXIT
%token          TOKEN_EOF
%token          TOKEN_NL

%token  <vstr>  TOKEN_NAMESPACES_PREFIX
%token          TOKEN_CHANNELS_PREFIX
%token          TOKEN_FILE_H
%token          TOKEN_FILE_CC
%token          TOKEN_GLOBAL_LOGGER_POINTER

%token  <vstr>  TOKEN_NSP_ENTER
%token  <vstr>  TOKEN_NSP_EXIT
%token  <vstr>  TOKEN_NSP_MACROS_PREFIX

%token  <vint>  TOKEN_LOG_THEME_PREFORMAT_LST
%token  <vint>  TOKEN_LOG_THEME_PREFORMAT_FMT
%token  <vint>  TOKEN_LOG_THEME_PREFORMAT_ARG

%token  <vstr>  TOKEN_LOG_THEME_NEW
%token  <vstr>  TOKEN_LOG_THEME_PARAMETER
%token  <vstr>  TOKEN_LOG_THEME_CHANNEL
%token  <vstr>  TOKEN_LOG_THEME_MACRO

%token  <vstr>  TOKEN_LOG_THEME_SUBCHANNEL_NEW
%token  <vstr>  TOKEN_LOG_THEME_SUBCHANNEL_COLOR_FG
%token  <vstr>  TOKEN_LOG_THEME_SUBCHANNEL_COLOR_BG
%token  <vstr>  TOKEN_LOG_THEME_SUBCHANNEL_FORMAT

%token  <vvoid> TOKEN_LOG_THEME_SUBCHANNEL_SPECIFICATION
//  ============================================================================
//  Non-terminals ( = grammar helpers for creating the tree branches )
//  ============================================================================
//%type   <vstr>  ksymname
//%type   <vstr>  kconst_num
//%type   <vstr>  exp
//%type   <vstr>  exp1
//%type   <vstr>  conditional
/*
 *
 *******************************************************************************
 *
 *                              RULES
 *
 *******************************************************************************
 *
 */
 %%

target:             /* empty string */
                |   target  lexer_error
                |   target  global
                |   target  nsp_enter
                |   target  nsp_exit
                |   target  nsp_macros_prefix
                |   target  log_theme
                |   target  log_theme_subchannel
;
                                                                                /*
    ****************************************************************************
    Lexer errors
    ****************************************************************************
                                                                                */
lexer_error:        TOKEN_LEXER_ERROR_SYNTAX
    {
        libgwr::log::parser::Lexer_error_syntax();
    }

                |   TOKEN_LEXER_ERROR_JAMMED
    {
        libgwr::log::parser::Lexer_error_jammed();
    }
;                                                                                /*
    ****************************************************************************
    Global
    ****************************************************************************
                                                                                */
global:
                    TOKEN_NAMESPACES_PREFIX
    {
        libgwr::log::parser::Namespaces_prefix($1);

        g_free( $1 );
    }
                |   TOKEN_CHANNELS_PREFIX       TOKEN_STD_STRING
    {
        libgwr::log::parser::Channels_prefix($2);

        g_free( $2 );
    }
                |   TOKEN_FILE_H                TOKEN_STD_STRING
    {
        libgwr::log::parser::File_h($2);

        g_free( $2 );
    }
                |   TOKEN_FILE_CC               TOKEN_STD_STRING
    {
        libgwr::log::parser::File_cc($2);

        g_free( $2 );
    }
                |   TOKEN_GLOBAL_LOGGER_POINTER TOKEN_STD_STRING
    {
        libgwr::log::parser::Global_logger_pointer($2);

        g_free( $2 );
    }
;
                                                                                /*
    ****************************************************************************
    Namespace start / end / name
    ****************************************************************************
                                                                                */
                                                                                /*
    ============================================================================
    nsp_start
    ============================================================================
                                                                                */
nsp_enter:          TOKEN_NSP_ENTER
    //              gchar*
    {
        libgwr::log::parser::Nsp_enter($1);

        g_free( $1 );
    }
;
                                                                                /*
    ============================================================================
    nsp_end
    ============================================================================
                                                                                */
nsp_exit:           TOKEN_NSP_EXIT
    //              gchar*
    {
        libgwr::log::parser::Nsp_exit($1);

        g_free( $1 );
    }
;
                                                                                /*
    ============================================================================
    nsp macro prefix
    ============================================================================
                                                                                */
nsp_macros_prefix:  TOKEN_NSP_MACROS_PREFIX     TOKEN_STD_STRING
    //              gchar*
    {
        libgwr::log::parser::Nsp_macros_prefix( $2 );

        g_free( $2 );
    }
;
                                                                                /*
    ****************************************************************************
    Log theme
    ****************************************************************************
                                                                                */
log_theme:          lt_new
                |   lt_parameter
                |   lt_channel
                |   lt_macro
                |   lt_preformat
;

                                                                                /*
    ============================================================================
    lt_new
    ============================================================================
                                                                                */
lt_new:             TOKEN_LOG_THEME_NEW
    //              gchar*
    {
        libgwr::log::parser::Nsp_theme_create($1);

        g_free( $1 );
    }
;
                                                                                /*
    ============================================================================
    lt_params : log theme parameter
    ============================================================================
                                                                                */
lt_parameter:       TOKEN_LOG_THEME_PARAMETER
    //              gchar*
    {
        libgwr::log::parser::Log_theme_parameter( $1 );

        g_free( $1 );
    }
;
                                                                                /*
    ============================================================================
    lt_channel
    ============================================================================
                                                                                */
lt_channel:         TOKEN_LOG_THEME_CHANNEL
    //              gchar*
    {
        libgwr::log::parser::Log_theme_channel( $1 );

        g_free( $1 );
    }
;
                                                                                /*
    ============================================================================
    lt_macro
    ============================================================================
                                                                                */
lt_macro:           TOKEN_LOG_THEME_MACRO
    //              gchar*
    {
        libgwr::log::parser::Log_theme_macro( $1 );

        g_free( $1 );
    }
;
                                                                                /*
    ============================================================================
    lt_preformat
    ============================================================================
                                                                                */
lt_preformat:       lt_preformat_lst
                |   lt_preformat_fmt
                |   lt_preformat_arg
;

lt_preformat_lst:   TOKEN_LOG_THEME_PREFORMAT_LST   TOKEN_STD_STRING_ARRAY
    //              guint32                         tStringArray
    {
        libgwr::log::parser::Nsp_preformat_lst($1, (libgwr::log::parsing::tStringArray*)$2);
    }
;

lt_preformat_fmt:   TOKEN_LOG_THEME_PREFORMAT_FMT   TOKEN_STD_STRING
    //              guint32                         gchar*
    {
        libgwr::log::parser::Nsp_preformat_fmt($1, $2);

        g_free( $2 );
    }
;

lt_preformat_arg:   TOKEN_LOG_THEME_PREFORMAT_ARG   TOKEN_STD_STRING
    //              guint32                         gchar*
    {
        libgwr::log::parser::Nsp_preformat_arg($1, $2);

        g_free( $2 );
    }
;
                                                                                /*
    ****************************************************************************
    Log theme subchannel
    ****************************************************************************
                                                                                */
log_theme_subchannel:

                    TOKEN_LOG_THEME_SUBCHANNEL_NEW
    //              gchar*
    {
        libgwr::log::parser::Log_theme_flavour_new( $1 );

        g_free($1);
    }
                |   TOKEN_LOG_THEME_SUBCHANNEL_COLOR_FG
    //              gchar*
    {
        libgwr::log::parser::Log_theme_flavour_color_fg( $1 );

        g_free($1);
    }
                |   TOKEN_LOG_THEME_SUBCHANNEL_COLOR_BG
    //              gchar*
    {
        libgwr::log::parser::Log_theme_flavour_color_bg( $1 );

        g_free($1);
    }
                |   TOKEN_LOG_THEME_SUBCHANNEL_FORMAT
    //              tStringArray
    {
        libgwr::log::parser::Log_theme_flavour_format( $1 );

        g_free($1);
    }
;

%%
/*
 *
 *******************************************************************************
 *
 *                              CODE
 *
 *******************************************************************************
 *
 */

// nothing yet






