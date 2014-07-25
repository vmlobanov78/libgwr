/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-gen-log-parsing.cc                                               *
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
    *   Class   : None                                                          *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *   Purpose : Class for parsing a linux kernel's configuration              *
    *                                                                           *
    *****************************************************************************
*/

#include    "CC/gen-log/libgwr-gen-log-parsing.hh"
#include    "CC/gen-log/lexer/libgwr-gen-log-lexer.hh"
#include    "CC/gen-log/parser/libgwr-gen-log-parser.hh"
//  ============================================================================
//  error handling
//
//  - In an action, you can cause immediate return from yyparse by using these macros:
//
//  YYACCEPT    Return immediately with value 0 (to report success).
//  YYABORT     Return immediately with value 1 (to report failure).
//
//  - Lexer return 0 => yyparse exit
//
//  Lexer errors
//      - internal                      :   STOP
//      - recoverable syntax error      :   CONT
//      - unrecoverale syntax error     :   STOP
//      - jam                           :   STOP
//
//  Parser errors
//
//  ============================================================================
#define     PARSING_INF(FORMAT, ...)   LIBGWR_GEN_LOG_INF( "Parsing:" FORMAT, __VA_ARGS__ );
#define     PARSING_WNG(FORMAT, ...)   LIBGWR_GEN_LOG_WNG( "Parsing:" FORMAT, __VA_ARGS__ );
#define     PARSING_ERR(FORMAT, ...)   LIBGWR_GEN_LOG_ERR( "Parsing:" FORMAT, __VA_ARGS__ );

GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(log)
GWR_NAMESPACE_START(parsing)
//  ****************************************************************************
//   Global Vars / Funcs
//  ****************************************************************************
static  const   gchar                       *       ga_str_NULL         = "NULL";
static  FILE                                *       ga_stream           = NULL;

static          GSList                      *       gd_slist_nsp            = NULL;
static          GSList                      *       gd_slist_cha            = NULL;
static          libgwr::StackGS < NSP *  >  *       gd_stack_nsp            = GWR_NEW_CAST( libgwr::StackGS < NSP * > );
static          guint32                             ga_thm_card             = 0;
static          THM                         *       ga_thm_current          = NULL;
static          guint32                             ga_fla_card             = 0;
static          FLA                         *       ga_fla_current          = NULL;

static          tStringArray                *       gd_namespaces_prefixes  =   GWR_NEW_CAST( tStringArray, 150, 2 );
static          gchar                       *       gd_namespaces_prefix    =   NULL;
static          gchar                       *       gd_channels_prefix      =   NULL;
static          gchar                       *       gd_file_def             =   NULL;
static          gchar                       *       gd_file_h               =   NULL;
static          gchar                       *       gd_file_cc              =   NULL;
static          gchar                       *       gd_global_logger_pointer=   NULL;

                libgwr::Spacer                      ga_spacer(1024, 4);         //!< Spacing for namespaces
//  ****************************************************************************
        GSList          *   Namespaces_list()
{
    return gd_slist_nsp;
}

        tStringArray    *   Namespaces_prefixes()
{
    return gd_namespaces_prefixes;
}

        const   gchar   *   Namespaces_prefix()
{
    return gd_namespaces_prefix;
}
//  ****************************************************************************
//   Classes
//  ****************************************************************************
parsing::CHA*
CHA::Get_by_name(const gchar* _cha_name)
{
    CHA         *   cha     = NULL;
    guint32         djb     = 0;
    GSList      *   slist   = gd_slist_cha;
    //  ........................................................................
    djb = gwr_str_hash( _cha_name );

    while ( slist )
    {
        cha = static_cast < CHA* >( slist->data );

        if ( cha->djb() == djb )
            return cha;

        slist = g_slist_next( slist );
    }

    return NULL;
}

parsing::NSP*
NSP::Get_by_name(const gchar* _name)
{
    NSP         *   nsp     = NULL;
    guint32         djb     = 0;
    GSList      *   sl      = gd_slist_nsp;
    //  ........................................................................
    djb = gwr_str_hash( _name );

    while ( sl )
    {
        nsp = static_cast < NSP* >( sl->data );

        if ( nsp->djb() == djb )
            return nsp;

        sl = g_slist_next( sl );
    }

    return NULL;
}
//  ****************************************************************************
        const   gchar   *   Channels_prefix         ()
{
    return gd_channels_prefix;
}
        const   gchar   *   File_def                ()
{
    return gd_file_def;
}
        const   gchar   *   File_h                  ()
{
    return gd_file_h;
}
        const   gchar   *   File_cc                 ()
{
    return gd_file_cc;
}
        const   gchar   *   Global_logger_pointer   ()
{
    return gd_global_logger_pointer;
}
//  ****************************************************************************
//   Runtime
//  ****************************************************************************
void        Show_all()
{
    NSP     *   nsp = NULL;
    THM     *   thm = NULL;
    FLA     *   fla = NULL;

    GSList  *   sl1 = NULL;
    GSList  *   sl2 = NULL;
    GSList  *   sl3 = NULL;

    gchar   *   tmp = NULL;
    //  ........................................................................

    sl1 = gd_slist_nsp ; while( sl1 )
    {
        nsp = static_cast < NSP * > ( sl1->data );

        PARSING_INF("namespace %s", nsp->nf());

        sl2 = nsp->slist(); while ( sl2 )
        {
            thm = static_cast < THM * > ( sl2->data );

            libgwr::dstr::append(tmp, "  log theme [", thm->name(), "] ( ", NULL );

            sl3 = thm->slist(); while ( sl3 )
            {
                fla = static_cast < FLA * > ( sl3->data );

                libgwr::dstr::append(tmp, fla->name(), " ", NULL);

                sl3 = g_slist_next( sl3 );
            }

            libgwr::dstr::append(tmp, ")", NULL );

            PARSING_INF("%s", tmp);
            libgwr::dstr::rst( tmp );

            sl2 = g_slist_next( sl2 );
        }

        sl1 = g_slist_next( sl1 );
    }
}
//  ****************************************************************************
        NSP*                Nsp_current()
{
    NSP     *   nsp = NULL;
    //  ........................................................................
    if ( gd_stack_nsp->peek( nsp ) )
        return nsp;

    return NULL;
}

        void                Nsp_add         (NSP*   _nsp)
{
    gd_slist_nsp = g_slist_append( gd_slist_nsp, (gpointer)_nsp );
}

        void                Nsp_push        (NSP*   _nsp)
{
    gd_stack_nsp->push( _nsp );
}

        gboolean            Nsp_pop         (NSP*&  __nsp)
{
    return gd_stack_nsp->pop( __nsp );
}

        guint32             Thm_card()
{
    return ga_thm_card;
}

        void                Thm_card_inc()
{
    ga_thm_card++;
}

        THM*                Thm_current()
{
    return ga_thm_current;
}

        void                Thm_current_set(THM* _thm)
{
    ga_thm_current = _thm;
}

        guint32             Fla_card()
{
    return ga_fla_card;
}

        void                Fla_card_inc()
{
    ga_fla_card++;
}

        FLA*                Fla_current()
{
    return ga_fla_current;
}

        void                Fla_current_set(FLA* _fla)
{
    ga_fla_current = _fla;
}

        CHA             *   Channel(const gchar* _channel_name)
{
    CHA     *   cha     =   NULL;
    //  ........................................................................
    cha = CHA::Get_by_name(_channel_name);
    if ( ! cha )
    {
        cha = GWR_NEW_CAST( CHA, parsing::Channels_prefix(), _channel_name );
        gd_slist_cha = g_slist_append( gd_slist_cha, (gpointer)cha );
    }
    //BISON_INF("  channel [%s][0x%p]", cha->name(), cha);

    return cha;
}
//  ============================================================================
        gboolean            Namespaces_prefix_add       (const gchar* _str)
{
    //  append to the list of nsp prefixes
    gd_namespaces_prefixes->add( const_cast < gchar* > ( _str ) );

    //  build nsp prefixes string
    if ( gd_namespaces_prefix )
    {
        libgwr::dstr::append( gd_namespaces_prefix, "::" , _str, NULL );
    }
    else
    {
        libgwr::dstr::append( gd_namespaces_prefix, _str, NULL );
    }

    return TRUE;
}
        gboolean            Channels_prefix_set         (const gchar* _str)
{
    if ( gd_channels_prefix ) return FALSE;

    gd_channels_prefix = g_strdup( _str );
    return TRUE;
}
        gboolean            File_def_set                (const gchar* _str)
{
    if ( gd_file_def ) return FALSE;

    gd_file_def = g_strdup( _str );
    return TRUE;
}
        gboolean            File_h_set                  (const gchar* _str)
{
    if ( gd_file_h ) return FALSE;

    gd_file_h = g_strdup( _str );
    return TRUE;
}
        gboolean            File_cc_set                 (const gchar* _str)
{
    if ( gd_file_cc ) return FALSE;

    gd_file_cc = g_strdup( _str );
    return TRUE;
}
        gboolean            Global_logger_pointer_set   (const gchar* _str)
{
    if ( gd_global_logger_pointer ) return FALSE;

    gd_global_logger_pointer = g_strdup( _str );
    return TRUE;
}
//  ============================================================================
        gboolean            Free()
{
    GSList      *       l       =   NULL;
    NSP         *       nsp     =   NULL;
    CHA         *       cha     =   NULL;
    //  ........................................................................
    //  free all dynamic vars
    delete  gd_namespaces_prefixes;
    g_free_safe(    gd_namespaces_prefix        );
    g_free_safe(    gd_channels_prefix          );
    g_free_safe(    gd_file_def                 );
    g_free_safe(    gd_file_h                   );
    g_free_safe(    gd_file_cc                  );
    g_free_safe(    gd_global_logger_pointer    );


    //  static          GSList                      *       gd_slist_nsp            = NULL;
    //  static          GSList                      *       gd_slist_cha            = NULL;
    //  static          libgwr::StackGS < NSP *  >  *       gd_stack_nsp            = GWR_NEW_CAST( libgwr::StackGS < NSP * > );

    l = gd_slist_nsp ; while ( l )
    {
        nsp = static_cast < NSP* > ( l->data );

        delete nsp;

        l = g_slist_next( l );
    }
    g_slist_free( gd_slist_nsp );

    l = gd_slist_cha ; while ( l )
    {
        cha = static_cast < CHA* > ( l->data );

        delete cha;

        l = g_slist_next( l );
    }
    g_slist_free( gd_slist_cha );

    return TRUE;
}
        gboolean            Reset()
{
    //  reset lexer & parser
    lexer::Reset();
    parser::Reset();

    //  reset the stream
    ga_stream               =   NULL;

    //  reset all dynamic vars
    gd_namespaces_prefixes  =   GWR_NEW_CAST( tStringArray, 150, 2 );
    gd_channels_prefix      =   NULL;
    gd_file_h               =   NULL;
    gd_file_cc              =   NULL;

    ga_thm_card             =   0;
    ga_fla_card             =   0;

    return TRUE;
}
//  ----------------------------------------------------------------------------
        gboolean            Parse()
{
    libgwr::Chrono  chrono;
    int             yyret   =   0;

    yydebug                 =   0;
    //  ........................................................................
    //  stream
    g_return_val_if_fail( ! ga_stream,   FALSE );                               //  ensure no stream is defined

    // eventually create - erase current content
	ga_stream = fopen(File_def(), "r");

	// make sure it is valid:
	if ( ! ga_stream )
    {
        PARSING_ERR("Could not open file [%s]\n", File_def());
		return FALSE;
	}

    //  ........................................................................
	// set flex to read from it instead of defaulting to STDIN:
	yyin = ga_stream;

    PARSING_INF("%s", "Parsing...");

    // go !
    yyparse();

    parser::Set_themes_preformats();
    parser::Set_namespaces_prefix();

    PARSING_INF("Parsing finished in  %i:%i", chrono.es(), chrono.ems());
    //PARSING_INF("  Lexer status :%s", lexer::Status());
    //PARSING_INF("  Parser status:%s", parser::Status());

    fclose(ga_stream);
    fflush(stdin);

    Show_all();

    //  return the error indicator
    return ( yyret != 0 );
}
//  ----------------------------------------------------------------------------
        gboolean            Lex()
{
    /*
    libgwr::Chrono  chrono;
    int             yyret   =   0;
    //  ........................................................................
    g_return_val_if_fail( ! A_kconfig_stream,   FALSE );                        //  ensure no stream is defined
    //  ........................................................................
    //  kconfig stream

    // eventually create - erase current content
	A_kconfig_stream = fopen(globals::g_kernel_kconfig_abspath, "r");

	// make sure it is valid:
	if ( ! A_kconfig_stream )
    {
        PARSING_FILES_ERR("Could not open file [%s]", globals::g_kernel_kconfig_abspath);
		return FALSE;
	}

    Kconfig_array_add(globals::g_kernel_kconfig_relpath);                       //  add 1st Kconfig file to the Kconfig files array, now size is 1, and index 0
    a_stack_kconfig_file_index.push(0);                                         //  push that index on the stack

    //  ........................................................................
	// set flex to read from it instead of defaulting to STDIN:
	yyin = A_kconfig_stream;

    PARSING_STD_INF("%s", "Parsing started...");

    // go !

    //  init libkconfig::object logging :
    //      - log along Bison log
    //      - warning on str -> int conversion, but it should not happend,
    //        parsing just build the structures and doesnt affect values
    libkconfig::object::G_log_wrap(LIBKCONFIG_LOG_CHANNEL_PARSING_BISON, libkconfig::parser::P_container_spacer());
    chrono.start();
    yyret = yyparse();
    chrono.time();
    libkconfig::object::G_log_unwrap();

    fclose(A_kconfig_stream);

    fflush(stdin);

    PARSING_STD_INF("Parsing finished in  %i:%i", chrono.es(), chrono.ems());
    PARSING_STD_INF("  Lexer status :%s", lexer::Status());
    PARSING_STD_INF("  Parser status:%s", parser::Status());

    PARSER_BISON_STD_INF("Symbols   :%i", libkconfig::object::factory::g_card_symbol()   );
    PARSER_BISON_STD_INF("Containers:%i", libkconfig::object::factory::g_card_container());
    PARSER_BISON_STD_INF("Items     :%i", libkconfig::object::factory::g_card_item()     );
    PARSER_BISON_STD_INF("Hash      :%i", libkconfig::object::factory::g_ht_size()       );

    //  return the error indicator
    return ( yyret != 0 );
    */
    return FALSE;
}

GWR_NAMESPACE_END(parsing)
GWR_NAMESPACE_END(log)
GWR_NAMESPACE_END(libgwr)





