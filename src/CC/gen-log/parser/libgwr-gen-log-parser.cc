/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-gen-log-parser.cc                                                *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libkconfig                                                      *
    *                                                                           *
    *   Copyright (C) 2011-2014 Guillaume Wardavoir                             *
    *   Copyright (C) ????      Professor Daniel J. Bernstein                   *
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
    *   Purpose : Parser for .def files                                         *
    *                                                                           *
    *****************************************************************************
*/

//  ............................................................................
#include    "libgwr-gen-log-parser.hh"
//  ............................................................................
        void            yyerror(const char* _msg)
{
    libgwr::log::parser::Stop();
}


GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(log)
GWR_NAMESPACE_START(parser)

#define BISON_STOP_LEXER_IF_FALSE(COND)                                         \
    if ( ! COND )                                                               \
    {                                                                           \
        g_stop_lexer();                                                         \
        BISON_ERR("%s", "Stopping.");                                           \
    }
//  ****************************************************************************
//   Defines
//  ****************************************************************************
#define     BISON_INF(FORMAT, ...)   LIBGWR_GEN_LOG_INF( "Parser:" FORMAT, __VA_ARGS__ );
#define     BISON_WNG(FORMAT, ...)   LIBGWR_GEN_LOG_WNG( "Parser:" FORMAT, __VA_ARGS__ );
#define     BISON_ERR(FORMAT, ...)   LIBGWR_GEN_LOG_ERR( "Parser:" FORMAT, __VA_ARGS__ );
//  ****************************************************************************
//   Enums / typedef
//  ****************************************************************************
enum
{
    eStatus_ok                          =       0                               ,
    eStatus_error_jammed                =       1                               ,
    eStatus_error_libkconfig            =       2                               ,
    eStatus_error_syntax                =       3                               ,
    eStatus_stopped_by_external         =       4                               ,
    eStatus_error_duplicate             =       5
};
//  ****************************************************************************
//   Vars / Funcs
//  ****************************************************************************
static  FILE            *   ga_parser_stream            =   NULL;               //!< Current Kconfig file input for parser
static  guint32             a_status                    =   parser::eStatus_ok;
static  gboolean            a_status_msg_initialized    =   FALSE;
static  gchar           *   Status_msg                  [10];
//  ****************************************************************************
static  void            Status_msg_init()
{
    if ( a_status_msg_initialized )
        return;

    Status_msg[parser::eStatus_ok                   ]   = (gchar*)"Success";
    Status_msg[parser::eStatus_error_jammed         ]   = (gchar*)"Jammed";
    Status_msg[parser::eStatus_error_libkconfig     ]   = (gchar*)"libkconfig error";
    Status_msg[parser::eStatus_error_syntax         ]   = (gchar*)"Unrecoverable syntax error";
    Status_msg[parser::eStatus_stopped_by_external  ]   = (gchar*)"Stopped explicitly by external code";
    Status_msg[parser::eStatus_error_duplicate      ]   = (gchar*)"Duplicate name / object error";
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
static  void            Stop_lexer()
{
    BISON_ERR("%s", "Stopping lexer.");
    lexer::Stop();
}
static  void            StopInternal(guint32 _status)
{
    BISON_ERR("%s", "Stopping.");
    Status_set( _status );
    Stop_lexer();
}
static  void            StopExternal()
{
    StopInternal( eStatus_stopped_by_external );
}
        void            Stop()
{
    StopExternal();
}
        gboolean        Reset()
{
    Status_msg_init();
    a_status                    =   parser::eStatus_ok;

    return TRUE;
}
//  ============================================================================
void            Lexer_error_syntax          ()
{
    Stop();
}
void            Lexer_error_jammed          ()
{
    Stop();
}
//  ############################################################################
//
//  Parsing details
//
//  ############################################################################
void            Namespaces_prefix           (const gchar* _str)
{
    if ( ! parsing::Namespaces_prefix_add(_str) )
        StopInternal( eStatus_error_syntax );

    BISON_INF( "Namespaces prefix:[%s]", _str );
}
void            Channels_prefix             (const gchar* _str)
{
    if ( ! parsing::Channels_prefix_set(_str) )
        StopInternal( eStatus_error_syntax );

    BISON_INF( "Channels prefix:[%s]", _str );
}
void            File_h                      (const gchar* _str)
{
    if ( ! parsing::File_h_set(_str) )
        StopInternal( eStatus_error_syntax );

    BISON_INF( "File_h:[%s]", _str );
}
void            File_cc                     (const gchar* _str)
{
    if ( ! parsing::File_cc_set(_str) )
        StopInternal( eStatus_error_syntax );

    BISON_INF( "File_cc:[%s]", _str );
}
void            Global_logger_pointer       (const gchar* _str)
{
    if ( ! parsing::Global_logger_pointer_set(_str) )
        StopInternal( eStatus_error_syntax );

    BISON_INF( "Global_logger_pointer:[%s]", _str );
}
//  ****************************************************************************
void            Nsp_enter                   (const gchar* _nsp_name)
{
    parsing::NSP    *       nsp_now     = NULL;
    parsing::NSP    *       nsp_new     = NULL;
    parsing::NSP    *       nsp_pushed  = NULL;
    //  ........................................................................
    BISON_INF("entering namespace [%s]", _nsp_name);
    parsing::ga_spacer.inc();

    nsp_now = parsing::Nsp_current();                                           //  get current nsp
    nsp_new = GWR_NEW_CAST( parsing::NSP, nsp_now, _nsp_name );                 //  create new nsp

    nsp_pushed = parsing::NSP::Get_by_name( nsp_new->nf() );                    //  see if a nsp with given name already exist
    if ( nsp_pushed )                                                           //  yes -> repush it
    {
        BISON_INF(      "... already exist, repushing [0x%p]", nsp_pushed);
        delete nsp_new;
        nsp_new = nsp_pushed;
    }
    else                                                                        // no -> push newly created nsp
    {
        BISON_INF("%s", "... new namespace");
        parsing::Nsp_add( nsp_new );
    }

    parsing::Nsp_push( nsp_new );
}

void            Nsp_exit                    (const gchar* _nsp_name)
{
    parsing::NSP    *       nsp_now     = parsing::Nsp_current();
    parsing::NSP    *       nsp_popped  = NULL;
    //  ........................................................................
    parsing::ga_spacer.dec();
    BISON_INF("exiting namespace [%s]", _nsp_name);

    if ( strcmp( nsp_now->n(), _nsp_name ) )                                    //  verify exit mismatch
    {
        BISON_ERR("namespace mismatch, current in stack is [%s]", nsp_now->n());
        Stop();
        return;
    }

    if ( ! parsing::Nsp_pop( nsp_popped ) )                                     //  pop
    {
        BISON_ERR("%s", "namespace pop failed");
        Stop();
        return;
    }

    if ( strcmp( nsp_now->n(), nsp_popped->n() ) )                              //  verify pop mismatch
    {
        BISON_ERR("namespace mismatch, popped [%s] instead of [%s]", nsp_popped->n(), nsp_now->n());
        Stop();
        return;
    }

    nsp_now = parsing::Nsp_current();
    BISON_INF("now in namespace [%s]", nsp_now ? nsp_now->n() : "not in a namespace" );
}

void            Nsp_macros_prefix           (const gchar* _mpfx)
{
    parsing::NSP    *       nsp         = NULL;
    //  ........................................................................
    BISON_INF("  namespace macros prefix [%s]", _mpfx);
    //  ........................................................................
    nsp = parsing::Nsp_current();
    if ( ! nsp )
    {
        BISON_ERR("%s", "... no current namespace");
        return;
    }

    nsp->mpfx_set( _mpfx );
}

void            Nsp_preformat_lst           (guint32 _id, parsing::tStringArray* _lst)
{

    parsing::NSP        *       nsp     = NULL;
    parsing::TPRF       *       tprf    = NULL;
    //  ........................................................................
    BISON_INF("nsp prf [%u] lst", _id);
    for ( guint32 i = 0 ; i != _lst->card() ; i++ )
    {
        BISON_INF("...%s", _lst->get(i));
    }
    //  ........................................................................
    nsp = parsing::Nsp_current();
    if ( ! nsp )
    {
        BISON_ERR("%s", "... no current namespace");
        return;
    }

    if ( nsp->tprf(_id) )
    {
        BISON_ERR("%s", "... PRF already exist");
        return;
    }

    BISON_INF("...creating preformat [%u]", _id);
    tprf = GWR_NEW_CAST( parsing::TPRF, _id );
    tprf->lst_set   ( _lst );
    nsp->tprf_set   ( _id, tprf );
}

void            Nsp_preformat_fmt           (guint32 _id, const gchar* _pf_fmt)
{
    parsing::NSP        *       nsp     = NULL;
    parsing::TPRF       *       tprf    = NULL;
    //  ........................................................................
    BISON_INF("nsp prf [%u] fmt [%s]", _id, _pf_fmt);
    //  ........................................................................
    nsp = parsing::Nsp_current();
    if ( ! nsp )
    {
        BISON_ERR("%s", "... no current namespace");
        return;
    }

    tprf = nsp->tprf(_id);

    if ( ! tprf )
    {
        BISON_ERR("%s", "... TPRF doesnt exist");
        return;
    }

    if ( tprf->fmt() )
    {
        BISON_ERR("%s", "... TPRF fmt already exist !");
        return;
    }

    tprf->fmt_set( _pf_fmt );
}

void            Nsp_preformat_arg           (guint32 _id, const gchar* _pf_arg)
{
    parsing::NSP        *       nsp     = NULL;
    parsing::TPRF       *       tprf    = NULL;
    //  ........................................................................
    BISON_INF("nsp prf [%u] arg [%s]", _id, _pf_arg);
    //  ........................................................................
    nsp = parsing::Nsp_current();
    if ( ! nsp )
    {
        BISON_ERR("%s", "... no current namespace");
        return;
    }

    tprf = nsp->tprf(_id);

    if ( ! tprf )
    {
        BISON_ERR("%s", "... TPRF doesnt exist");
        return;
    }

    if ( tprf->arg() )
    {
        BISON_ERR("%s", "... TPRF arg already exist !");
        return;
    }

    tprf->arg_set( _pf_arg );
}

void            Nsp_theme_create            (const gchar* _thm_name)
{
    parsing::NSP    *       nsp_now     = NULL;
    parsing::THM    *       thm         = NULL;
    //  ........................................................................
    BISON_INF("  namespace theme creation [%s]", _thm_name);

    nsp_now = parsing::Nsp_current();
    if ( ! nsp_now )
    {
        BISON_ERR("%s", "... no current namespace");
        return;
    }

    thm = nsp_now->theme_find( _thm_name );
    if ( thm )
    {
        BISON_ERR("%s", "... theme already exist");
        StopInternal( eStatus_error_duplicate );
        return;
    }

    thm = GWR_NEW_CAST( parsing::THM, _thm_name, parsing::Thm_card() );
    parsing::Thm_card_inc();
    parsing::Thm_current_set( thm );
    BISON_INF("theme [%s - %u] created & added to namespace [%s]", _thm_name, thm->gix(), nsp_now->n());

    nsp_now->theme_add( thm );
}
//  ****************************************************************************
void            Log_theme_parameter         (const gchar* _param_name)
{
    parsing::THM    *       thm         = NULL;
    //  ........................................................................
    BISON_INF("  log theme parameter [%s]", _param_name);

    thm = parsing::Thm_current();
    if ( ! thm )
    {
        BISON_ERR("%s", "... no current theme");
        return;
    }

    thm->params_add( _param_name );
}

void            Log_theme_channel           (const gchar* _channel_name)
{
    parsing::CHA    *       cha         = NULL;
    parsing::THM    *       thm         = NULL;
    //  ........................................................................
    BISON_INF("  log theme channel [%s]", _channel_name);

    thm = parsing::Thm_current();
    if ( ! thm )
    {
        BISON_ERR("%s", "... no current theme");
        return;
    }

    cha = parsing::Channel( _channel_name );
    thm->channel_set( cha );
}

void            Log_theme_macro             (const gchar* _macro_name)
{
    parsing::NSP    *       nsp_now     = NULL;
    parsing::THM    *       thm         = NULL;
    //  ........................................................................
    BISON_INF("  log theme macro [%s]", _macro_name);

    nsp_now = parsing::Nsp_current();
    if ( ! nsp_now )
    {
        BISON_ERR("%s", "... no current namespace");
        return;
    }

    thm = parsing::Thm_current();
    if ( ! thm )
    {
        BISON_ERR("%s", "... no current theme");
        return;
    }

    thm->macro_set( nsp_now->mpfx(), _macro_name );
}

void            Log_theme_flavour_new       (const gchar* _fla_name)
{
    parsing::CHA    *       cha         = NULL;
    parsing::THM    *       thm         = NULL;
    parsing::FLA    *       fla         = NULL;
    //  ........................................................................
    BISON_INF("  log theme flavour new [%s]", _fla_name);

    thm = parsing::Thm_current();
    if ( ! thm )
    {
        BISON_ERR("%s", "... no current theme");
        return;
    }

    cha = thm->channel();

    fla = GWR_NEW_CAST( parsing::FLA, _fla_name, cha->sc(), parsing::Fla_card() );
    parsing::Fla_card_inc();
    parsing::Fla_current_set( fla );

    thm->fla_add( fla );
    cha->sc() = 1 + cha->sc();

    BISON_INF("flavour [%s - %u] created & added to channel [%s] who has now [%03u] flavours", _fla_name, fla->gix(), cha->name_core(), cha->sc());
}

void            Log_theme_flavour_color_fg  (const gchar* _fg)
{
    parsing::FLA    *       fla         = NULL;
    //  ........................................................................
    BISON_INF("  log theme subchannel color fg [%s]", _fg);

    fla = parsing::Fla_current();
    if ( ! fla )
    {
        BISON_ERR("%s", "... no current subchannel");
        return;
    }

    fla->set_fg( _fg );
}

void            Log_theme_flavour_color_bg  (const gchar* _bg)
{
    parsing::FLA    *       fla         = NULL;
    //  ........................................................................
    BISON_INF("  log theme subchannel color bg [%s]", _bg);

    fla = parsing::Fla_current();
    if ( ! fla )
    {
        BISON_ERR("%s", "... no current subchannel");
        return;
    }

    fla->set_bg( _bg );
}

void            Log_theme_flavour_format    (const gchar* _format)
{
    parsing::FLA    *       fla         = NULL;
    //  ........................................................................
    BISON_INF("  log theme subchannel format [%s]", _format);

    fla = parsing::Fla_current();
    if ( ! fla )
    {
        BISON_ERR("%s", "... no current subchannel");
        return;
    }

    fla->fmt()->add( g_strdup(_format) );
}
//  ============================================================================
gboolean        Set_themes_preformats()
{
    guint32                         i           = 0;
    guint32                         j           = 0;

    parsing::NSP            *       nsp         = NULL;

    parsing::TPRF           *       tprf        = NULL;
    parsing::tStringArray   *       tprflst     = NULL;

    parsing::THM            *       thm         = NULL;
    const gchar             *       tn          = NULL;
    GSList                  *       sl1 = NULL;
    //..........................................................................
    BISON_INF("%s", "+---------------------------------------+");
    BISON_INF("%s", "|   Set_themes_preformats()             |");
    BISON_INF("%s", "+---------------------------------------+");

    sl1 = parsing::Namespaces_list() ; while ( sl1 )
    {
        nsp = static_cast < parsing::NSP* > ( sl1->data );

        BISON_INF("namespace [%s]", nsp->nf());

        for ( i = 0 ; i != 10 ; i++ )
        {
            tprf = nsp->tprf(i);

            if ( ! tprf )
                continue;

            tprflst = tprf->lst();

            //  for each theme named in preformat array...
            for ( j = 0 ; j != tprflst->card() ; j++ )
            {
                tn = tprflst->get(j);

                //  find it in namespace's themes
                thm = nsp->theme_find( tn );

                if ( ! thm )
                {
                    BISON_ERR("theme [%s] not found",tn);
                    return FALSE;
                }

                if ( thm->tprf() )
                {
                    BISON_ERR("theme [%s] has already a preformat",tn);
                    return FALSE;
                }

                thm->tprf_set( tprf );
                BISON_INF("  preformat set for theme [%s]", tn);
            }
        }

lab_nsp_next:
        sl1 = g_slist_next( sl1 );
    }

    return TRUE;
}

gboolean        Set_namespaces_prefix()
{
    parsing::NSP            *       nsp         = NULL;

    GSList                  *       sl1 = NULL;
    //..........................................................................
    BISON_INF("%s", "+---------------------------------------+");
    BISON_INF("%s", "|   Set_namespaces_prefix()             |");
    BISON_INF("%s", "+---------------------------------------+");

    sl1 = parsing::Namespaces_list() ; while ( sl1 )
    {
        nsp = static_cast < parsing::NSP* > ( sl1->data );

        BISON_INF("namespace [%s]", nsp->nf());

        nsp->set_namespaces_prefix( parsing::Namespaces_prefix() );

lab_nsp_next:
        sl1 = g_slist_next( sl1 );
    }

    return TRUE;
}



GWR_NAMESPACE_END(parser)
GWR_NAMESPACE_END(log)
GWR_NAMESPACE_END(libgwr)


