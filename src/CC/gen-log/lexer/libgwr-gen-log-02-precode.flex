%{
//  ============================================================================
//  INCLUDES
//  ============================================================================
// For lex invocation
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Token defined in kconfig.bison + common %union
#include    "CC/gen-log/parser/libgwr-gen-log.bison.defines.h"

#include    "libgwr.hh"
#include    "CC/gen-log/libgwr-gen-log-parsing.hh"
//  ============================================================================
//  LEX PARAMS
//  ============================================================================
//  ============================================================================
//  DEBUGGING
//  ============================================================================
//#define FLEX_DEBUG

#ifdef FLEX_DEBUG

    #define FLEX_INF(FORMAT, ...)                                               \
    {                                                                           \
        LIBGWR_GEN_LOG_INF("Lexer :" FORMAT , __VA_ARGS__);                     \
    }
    #define FLEX_WNG(FORMAT, ...)                                               \
    {                                                                           \
        LIBGWR_GEN_LOG_WNG("Lexer :" FORMAT , __VA_ARGS__);                     \
    }

#else

    #define FLEX_INF(FORMAT, ...)
    #define FLEX_WNG(FORMAT, ...)

#endif

#define FLEX_ERR(FORMAT, ...)                                                   \
{                                                                               \
    LIBGWR_GEN_LOG_ERR("Lexer :" FORMAT , __VA_ARGS__);                         \
}
//  ============================================================================
//  ERRORS - RETURNS
//  ============================================================================
static  int         ga_word_ix              = 0;                                //!< Index of current word on current line, starting from 0
//  ============================================================================
//  STATE_INITIAL variables & functions
//  ============================================================================
static  int         si_theme_card_current   = 0;                                //!< Log themes ( current namespace )

inline  void        si_reset();
inline  void        si_begin();
//  ============================================================================
//  STATE_ROL_STRING variables & functions
//  ============================================================================
static  gboolean                                ga_srol_string_flag;
static  gchar                                   ga_srol_string_chars   [256];
static  gchar                               *   ga_srol_string_p;
static  gchar                               *   gd_srol_string;

void    srol_string_reset   ();
void    srol_string_begin   ();
//  ============================================================================
//  STATE_ROL_SPLIT variables & functions
//  ============================================================================
static  libgwr::log::parsing::tStringArray   *   gd_srol_split_array;

void    srol_split_reset   ();
void    srol_split_begin   ();
//  ============================================================================
//  STATE_FIND_NAMESPACES_PREFIX variables & functions
//  ============================================================================
void    sfnp_reset      ();
void    sfnp_begin      ();
//  ============================================================================
//  STATE_FIND_NAMESPACE_NAME variables & functions
//  ============================================================================
enum eSfnnNspMode
{
    eSfnnNspEnter   =   1   ,
    eSfnnNspExit    =   2
};
static  int         sfnn_nsp_mode       = eSfnnNspEnter;

void    sfnn_reset      (eSfnnNspMode);
void    sfnn_begin      (eSfnnNspMode);
//  ============================================================================
//  STATE_THEME_SPECIFICATIONS variables & functions
//  ============================================================================
void    sts_reset();
void    sts_begin();
//  ============================================================================
//  STATE_SUBCHANNELS_SPECIFICATIONS variables & functions
//  ============================================================================
static  gchar                                       *   sss_name        =   NULL;
static  gchar                                       *   sss_color_fg    =   NULL;
static  gchar                                       *   sss_color_bg    =   NULL;
//static  libgwr::log::parsing::tStringArray          *   sss_attributes  =   NULL;

static  gboolean                                        sss_in_colors   =   FALSE;
static  gboolean                                        sss_in_attrs    =   FALSE;

void    sss_reset();
void    sss_begin();
//  ============================================================================
//  GLOBAL defines, variables & functions
//  ============================================================================
#define YYSTR(s)           yylval.vstr = strdup(s)
#define YYFSTR(format, ...)                                                     \
    sprintf(flex_str_dummy, format, __VA_ARGS__);                               \
    YYSTR(flex_str_dummy);

#define LIBGWR_GEN_LOG_FLEX_ERROR_JAMMED()                                              \
        libgwr::log::lexer::Status_set( libgwr::log::lexer::eStatus_error_jammed );     \
        return libgwr::log::lexer::Token(TOKEN_LEXER_ERROR_JAMMED);

#define LIBGWR_GEN_LOG_FLEX_ERROR_SYNTAX()                                              \
        libgwr::log::lexer::Status_set( libgwr::log::lexer::eStatus_error_syntax );     \
        return libgwr::log::lexer::Token(TOKEN_LEXER_ERROR_SYNTAX);

//! Duplicate ( malloc ) yytext
inline gchar *      g_yytext_dup()
{
    return g_strdup(yytext);
}

//! Cut last char of string
static  gchar       ga_cut_last_char                    [2048];
        gchar*      g_cut_last_char_and_dup(const gchar* _str)
{
    if ( ! _str )
    {
        FLEX_ERR("%s", "g_cut_last_char():NULL input");
        return NULL;
    }
    if ( _str[0] == 0 )
    {
        FLEX_ERR("%s", "g_cut_last_char():empty input");
        return NULL;
    }

    strcpy( ga_cut_last_char, _str );
    ga_cut_last_char[ strlen(_str) - 1 ] = 0;
    return g_strdup( ga_cut_last_char );
}

//! Cut last char of string
static  gchar       ga_cut_last_two_chars               [2048];
        gchar*      g_cut_last_two_chars_and_dup(const gchar* _str)
{
    if ( ! _str )
    {
        FLEX_ERR("%s", "ga_cut_last_two_chars():NULL input");
        return NULL;
    }
    if ( _str[0] == 0 )
    {
        FLEX_ERR("%s", "ga_cut_last_two_chars():empty input");
        return NULL;
    }
    if ( _str[1] == 0 )
    {
        FLEX_ERR("%s", "ga_cut_last_two_chars():one-char string");
        return NULL;
    }

    strcpy( ga_cut_last_two_chars, _str );
    ga_cut_last_two_chars[ strlen(_str) - 2 ] = 0;
    return g_strdup( ga_cut_last_two_chars );
}

%}
