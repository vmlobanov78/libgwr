/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-gen-log.flex                                                     *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libkconfig                                                      *
    *                                                                           *
    *   Copyright (C) 2011-2013 Guillaume Wardavoir                             *
    *   Copyright (C) 2002      Roman Zippel        <zippel@linux-m68k.org>     *
    *   Copyright (C) xxxx-xxxx Linux kernel's documentation team               *
    *                                                                           *
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
    *   Purpose : flex file for creating the log-genarating lexer               *
    *                                                                           *
    *   Notes   :                                                               *
    *                                                                           *
    *       - The actions are written in C++, but the lexer himself is _NOT_    *
    *         a "C++ lexer"                                                     *
    *                                                                           *
    *       - All patterns start in the <INITIAL> state, and complex sequences  *
    *         matching use some additional states ; the main difficulty is that *
    *         we cant differentiate a kernel symbol from some other keywords,   *
    *         for example "if". But we could proceed by elimination ???         *
    *                                                                           *
    *****************************************************************************
*/

/*
 *
 *******************************************************************************
 *
 *                              DEFINITIONS
 *
 *******************************************************************************
 *
 */

    //  %option nodefault   : supress default rule, and exit(2) when unmatched input

%option backup nostdinit noyywrap never-interactive full ecs
%option 8bit backup

    //  allow pushing & popping states
%option stack

    //  pointer / array
%option pointer

    //  keep yylineno valid
%option yylineno

    // use exclusive states ( why not ? )

%x  STATE_FIND_WORD

%x  STATE_ROL_STRING
%x  STATE_ROL_SPLIT


%x  STATE_FIND_NAMESPACES_PREFIX
%x  STATE_FIND_NAMESPACE_NAME

%x  STATE_MACRO_PREFIX
%x  STATE_THEME_SPECIFICATIONS
%x  STATE_SUBCHANNEL_SPECIFICATIONS
