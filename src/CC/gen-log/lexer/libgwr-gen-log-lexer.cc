/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-gen-log-lexer.cc                                                 *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libkconfig                                                      *
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

#include    "CC/gen-log/lexer/libgwr-gen-log-lexer.hh"

// Token defined in kconfig.bison + common %union
#include    "CC/gen-log/parser/libgwr-gen-log.bison.defines.h"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(log)
GWR_NAMESPACE_START(lexer)
//  ****************************************************************************
//   Defines
//  ****************************************************************************
#define     FLEX_INF(FORMAT, ...)   LIBGWR_GEN_LOG_INF( FORMAT, __VA_ARGS__ );
#define     FLEX_WNG(FORMAT, ...)   LIBGWR_GEN_LOG_WNG( FORMAT, __VA_ARGS__ );
#define     FLEX_ERR(FORMAT, ...)   LIBGWR_GEN_LOG_ERR( FORMAT, __VA_ARGS__ );
//  ****************************************************************************
//   Enums / typedef
//  ****************************************************************************
//  ****************************************************************************
//   Vars / Funcs
//  ****************************************************************************
static  guint32         a_status                    =   lexer::eStatus_ok;
static  gboolean        a_status_msg_initialized    =   FALSE;
static  gchar       *   Status_msg                  [10];

static  gboolean        a_stop                      =   FALSE;

static  int             a_token_eof                 =   TOKEN_EOF;
//  ****************************************************************************
static  void            Status_msg_init()
{
    if ( a_status_msg_initialized )
        return;

    Status_msg[lexer::eStatus_ok                    ]   = (gchar*)"Success";
    Status_msg[lexer::eStatus_error_jammed          ]   = (gchar*)"Jammed";
    Status_msg[lexer::eStatus_error_internal        ]   = (gchar*)"Internal error";
    Status_msg[lexer::eStatus_error_syntax          ]   = (gchar*)"Unrecoverable syntax error";
    Status_msg[lexer::eStatus_stopped_by_external   ]   = (gchar*)"Stopped explicitly by external code";
}
        gchar   *       Status()
{
    return  Status_msg[ a_status ];
}
        void            Status_set(guint32 _status)
{
    a_status    =   _status;
}
//  ============================================================================
        void            StopInternal(guint32 _status)
{
    a_stop  = TRUE;
    Status_set( _status );
}
        void            StopExternal()
{
    StopInternal( eStatus_stopped_by_external );
}
        void            Stop()
{
    StopExternal();
}
//  ============================================================================
        int             Token(int _token)
{
    if ( ! a_stop )
        return _token;

    return 0;
}
//  ============================================================================
        int             Token_EOF()
{
    return a_token_eof;
}
//  When lexer encounters EOF, it returns g_lexer_eof(). This value is set
//  to TOKEN_EOF, which allows the parser to change the lexer input.
//  When parser detect the last file, it sets g_lexer_eof() to return 0,
//  which stops the parser itself.
        void            Token_EOF_set_to_zero()
{
    a_token_eof = 0;
}
//  ============================================================================
        void            Fatal_error(const gchar* msg)                           //  YY_FATAL_ERROR
{
    FLEX_ERR        ("%s", "--- LEXER INTERNAL ERROR---\n");
}
//  ============================================================================
        gboolean        Reset()
{
    Status_msg_init();
    a_status                    =   lexer::eStatus_ok;
    a_stop                      =   FALSE;
    a_token_eof                 =   TOKEN_EOF;

    return TRUE;
}

GWR_NAMESPACE_END(lexer)
GWR_NAMESPACE_END(log)
GWR_NAMESPACE_END(libgwr)






