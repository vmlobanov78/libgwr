/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 2068 of yacc.c  */
#line 117 "libgwr-gen-log.bison"

        # include  "libgwr.hh"
    


/* Line 2068 of yacc.c  */
#line 44 "libgwr-gen-log.bison.defines.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_STD_STRING = 258,
     TOKEN_STD_STRING_ARRAY = 259,
     TOKEN_LEXER_ERROR_SYNTAX = 260,
     TOKEN_LEXER_ERROR_JAMMED = 261,
     TOKEN_LEXER_EXIT = 262,
     TOKEN_EOF = 263,
     TOKEN_NL = 264,
     TOKEN_NAMESPACES_PREFIX = 265,
     TOKEN_CHANNELS_PREFIX = 266,
     TOKEN_FILE_H = 267,
     TOKEN_FILE_CC = 268,
     TOKEN_GLOBAL_LOGGER_POINTER = 269,
     TOKEN_NSP_ENTER = 270,
     TOKEN_NSP_EXIT = 271,
     TOKEN_NSP_MACROS_PREFIX = 272,
     TOKEN_LOG_THEME_PREFORMAT_LST = 273,
     TOKEN_LOG_THEME_PREFORMAT_FMT = 274,
     TOKEN_LOG_THEME_PREFORMAT_ARG = 275,
     TOKEN_LOG_THEME_NEW = 276,
     TOKEN_LOG_THEME_PARAMETER = 277,
     TOKEN_LOG_THEME_CHANNEL = 278,
     TOKEN_LOG_THEME_MACRO = 279,
     TOKEN_LOG_THEME_SUBCHANNEL_NEW = 280,
     TOKEN_LOG_THEME_SUBCHANNEL_COLOR_FG = 281,
     TOKEN_LOG_THEME_SUBCHANNEL_COLOR_BG = 282,
     TOKEN_LOG_THEME_SUBCHANNEL_FORMAT = 283,
     TOKEN_LOG_THEME_SUBCHANNEL_SPECIFICATION = 284
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 126 "libgwr-gen-log.bison"

        gchar                           *   vstr;
        gint                                vint;
        void                            *   vvoid;
        gchar                               vchr;
    


/* Line 2068 of yacc.c  */
#line 99 "libgwr-gen-log.bison.defines.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


