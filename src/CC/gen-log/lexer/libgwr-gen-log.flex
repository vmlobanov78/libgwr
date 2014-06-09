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
                                                                                /*
    ----------------------------------------------------------------------------
    Useful regexps
    lstring         \"[^\n\"]+\"
    lstring         \" ( \\ .|[^\ \" ] ) * \"
    lstringdq       \"[^\n\"]*\"
    ----------------------------------------------------------------------------
                                                                                */
nl              \n
esc_nl          \\\n
chr32           [ ]
tab             [\t]
wsp             [ \t]
blank           {wsp}|{nl}
underscore      _
minus           -
dot             \.
obrace          \{
cbrace          \}
brace           {obrace}|{cbrace}

comment         \/\/.*

R_chr_dec       [0-9]
R_chr_alpha     [A-Za-z]
R_chr_1         {R_chr_dec}|{R_chr_alpha}
R_chr_2         {R_chr_dec}|{R_chr_alpha}|{underscore}
R_chr_3         {R_chr_dec}|{R_chr_alpha}|{underscore}|:
R_chr_4         {R_chr_dec}|{R_chr_alpha}|{underscore}|{dot}

R_colon         :
R_comma         ,

R_word_1        {R_chr_1}+
R_word_2        {R_chr_2}+

R_key_nsp_prefix        \(namespaces-prefix\)
R_key_channels_prefix   \(channels-prefix\)
R_key_file_h            \(file\.h\)
R_key_file_cc           \(file\.cc\)
R_key_global_lp         \(global-logger-pointer\)

R_key_nsp_enter         \(\+
R_key_nsp_exit          \(-
R_key_nsp_macros_prefix \(macro-prefix\)

R_key_preformat_lst     \(pref-lst-{R_chr_dec}\)
R_key_preformat_fmt     \(pref-fmt-{R_chr_dec}\)
R_key_preformat_arg     \(pref-arg-{R_chr_dec}\)

R_nsp_name              {R_chr_2}+
R_theme_item            {R_chr_2}+
R_theme_param           {R_chr_2}+
R_theme_sub_name        {R_chr_2}+
R_theme_sub_spec        {R_chr_2}+

%%
                                                                                /*
    ****************************************************************************
    *
    *                              RULES
    *
    ****************************************************************************
                                                                                */

    //FLEX_INF("flex [          :%04i] *** RULE START ***\n", yylineno);
                                                                                /*
    ****************************************************************************
    STATE_INITIAL
    ****************************************************************************
                                                                                */
<INITIAL>
{
                                                                                /*
    ============================================================================
    INITIAL : Get rid of whitespaces, newlines, and comments
    ============================================================================
                                                                                */
    /*
     *  swallow whitespaces
     */
{wsp}+                                                                          {
    }
    /*
     *  newline : reset word index
     */
{nl}                                                                            {
        //FLEX_INF("%s", "INITIAL - NL");
        ga_word_ix = 0;
    }
    /*
     *  brace : ignore
     */
{brace}                                                                         {
    }


    /*
     *  comments
     */
{comment}                                                                       {
        // nothing
    }
                                                                                /*
    ============================================================================
    INITIAL : Globals
    ============================================================================
                                                                                */
{R_key_nsp_prefix}                                                              {

    //srol_string_begin();
    //return  libgwr::log::lexer::Token( TOKEN_NAMESPACES_PREFIX );
    sfnp_begin();
}

{R_key_channels_prefix}                                                         {

    srol_string_begin();
    return  libgwr::log::lexer::Token( TOKEN_CHANNELS_PREFIX );

}

{R_key_file_h}                                                                  {

    srol_string_begin();
    return  libgwr::log::lexer::Token( TOKEN_FILE_H );

}

{R_key_file_cc}                                                                 {

    srol_string_begin();
    return  libgwr::log::lexer::Token( TOKEN_FILE_CC );

}
{R_key_global_lp}                                                               {

    srol_string_begin();
    return  libgwr::log::lexer::Token( TOKEN_GLOBAL_LOGGER_POINTER );

}
                                                                                /*
    ============================================================================
    INITIAL : namespace enter / exit
    ============================================================================
                                                                                */
    /*
     *  enter new namespace
     */
{R_key_nsp_enter}                                                               {
        sfnn_begin(eSfnnNspEnter);
}
    /*
     *  exit current namespace
     */
{R_key_nsp_exit}                                                                {
        sfnn_begin(eSfnnNspExit);
    }
                                                                                /*
    ============================================================================
    INITIAL : namespace macro prefix
    ============================================================================
                                                                                */
{R_key_nsp_macros_prefix}                                                       {

    if ( ga_word_ix != 0 )
    {
        FLEX_ERR("key [!macro_prefix] but channel item is [%02u] not [#00]", ga_word_ix);
        LIBGWR_GEN_LOG_FLEX_ERROR_JAMMED();
    }

    FLEX_INF("%s", "key (macro-prefix)");

    ga_word_ix++;

    srol_string_begin();
    return TOKEN_NSP_MACROS_PREFIX;

}
                                                                                /*
    ============================================================================
    INITIAL : log theme preformats
    ============================================================================
                                                                                */
    /*
     *  keywords : preformat theme list
     */
{R_key_preformat_lst}                                                           {

    if ( ga_word_ix != 0 )
    {
        FLEX_ERR("key [!pref_lst] but channel item is [%02u] not [0]", ga_word_ix);
        LIBGWR_GEN_LOG_FLEX_ERROR_JAMMED();
    }

    ga_word_ix++;

    srol_split_begin();

    yylval.vint = (gint)( yytext[10] - 48 ); //stp_id;
    return TOKEN_LOG_THEME_PREFORMAT_LST;

}
    /*
     *  keywords : preformat format
     */
{R_key_preformat_fmt}                                                           {

    if ( ga_word_ix != 0 )
    {
        FLEX_ERR("key [!pref_fmt] but channel item is [%02u] not [0]", ga_word_ix);
        LIBGWR_GEN_LOG_FLEX_ERROR_JAMMED();
    }

    ga_word_ix++;

    srol_string_begin();
    //stp_begin((guint32)yytext[10], TOKEN_LOG_THEME_PREFORMAT_FMT);

    yylval.vint = (gint)yytext[10] - 48;
    return TOKEN_LOG_THEME_PREFORMAT_FMT;

}
    /*
     *  keywords : preformat args
     */
{R_key_preformat_arg}                                                           {

    if ( ga_word_ix != 0 )
    {
        FLEX_ERR("key [!pref_arg] but channel item is [%02u] not [0]", ga_word_ix);
        LIBGWR_GEN_LOG_FLEX_ERROR_JAMMED();
    }

    ga_word_ix++;

    srol_string_begin();

    yylval.vint = (gint)yytext[10] - 48;
    return TOKEN_LOG_THEME_PREFORMAT_ARG;

}
    /*
     *  keywords : params
     */
                                                                                /*
    ============================================================================
    INITIAL : log theme definitions
    ============================================================================
                                                                                */
    /*
     *  keywords : theme name
     */
{R_theme_item}                                                                  {

    if ( ga_word_ix != 0 )
    {
        FLEX_ERR("Theme item [%s] but channel item is [%02u] not [#00]", yytext, ga_word_ix);
        LIBGWR_GEN_LOG_FLEX_ERROR_JAMMED();
    }

    ga_word_ix++;

    sts_begin();
    yylval.vstr = g_yytext_dup();
    return libgwr::log::lexer::Token( TOKEN_LOG_THEME_NEW );
}
                                                                                /*
    ============================================================================
    INITIAL : the rest
    ============================================================================
                                                                                */
    /*
     *  EOF
     */
<<EOF>>                                                                         {
        return 0;
    }

    /*
     *  the rest = error
     */
.                                                                               {
    // error...
    FLEX_ERR("JAM [%c]", yytext[0]);
}


}   /* <INITIAL> */
                                                                                /*
    ****************************************************************************
    STATE_FIND_NAMESPACES_PREFIX

    Find the global namespaces-prefix
    ****************************************************************************
                                                                                */
<STATE_FIND_NAMESPACES_PREFIX>
{
    /*  whitespaces   */
{wsp}+                                                                          {

}
    /*  NAMESPACE   */
{R_nsp_name}                                                                    {

    si_begin();
    yylval.vstr     =   g_yytext_dup();
    return  libgwr::log::lexer::Token( TOKEN_NAMESPACES_PREFIX );

}
    /*  NAMESPACE:: */
{R_nsp_name}::                                                                  {

    yylval.vstr     =   g_cut_last_two_chars_and_dup( yytext );
    return  libgwr::log::lexer::Token( TOKEN_NAMESPACES_PREFIX );

}

    /*  anything else = error */
.|{nl}                                                                          {
    LIBGWR_GEN_LOG_FLEX_ERROR_SYNTAX();
}

}   /* <STATE_FIND_NAMESPACES_PREFIX> */
                                                                                /*
    ****************************************************************************
    STATE_FIND_NAMESPACE_NAME

    Find the namespace name. Separate state for differentiating with subchannels
    speciications.
    ****************************************************************************
                                                                                */
<STATE_FIND_NAMESPACE_NAME>
{
    /*
     *  found namespace name
     */
{R_nsp_name}                                                                    {

        //  entering a namespace
        if ( sfnn_nsp_mode == eSfnnNspEnter )
        {
            FLEX_INF("entering namespace [%s]", yytext);
            yylval.vstr = g_yytext_dup();
            return libgwr::log::lexer::Token( TOKEN_NSP_ENTER );
        }
        //  exiting a namespace
        if ( sfnn_nsp_mode == eSfnnNspExit )
        {
            FLEX_INF("exiting namespace [%s]", yytext);
            yylval.vstr = g_yytext_dup();
            return libgwr::log::lexer::Token( TOKEN_NSP_EXIT );
        }
}

    /*
     *  ending parenthesis
     */
\)                                                                              {
            si_begin();
    }

    /*
     *  anything else = error
     */
.|{nl}                                                                          {
        //LSLF_ERR("flex [          :%04i]:STATE_FIND_NAMESPACE_NAME:syntax error", yylineno);
        printf("line:[%i] text:[%s]\n", yylineno, yytext ? yytext : "NULL text pointer");
        LIBGWR_GEN_LOG_FLEX_ERROR_JAMMED();
    }

}   /* <STATE_FIND_NAMESPACE_NAME> */
                                                                                /*
    ****************************************************************************
    STATE_THEME_SPECIFICATIONS
    ****************************************************************************
                                                                                */
<STATE_THEME_SPECIFICATIONS>
{
    /*
     *  whitespace : drop
     */
{wsp}+                                                                          {

}
    /*
     *  theme : parameter ( ga_word_ix remains unchanged )
     */
@{R_theme_param}                                                                {

    FLEX_INF("STS:theme parameter [%s]", yytext + 1);                           //  +1 : drop the '@'
    yylval.vstr = g_strdup( yytext + 1 );
    return libgwr::log::lexer::Token( TOKEN_LOG_THEME_PARAMETER );

}
    /*
     *  theme : macro name, channel
     */
{R_theme_item}                                                                  {

    //FLEX_INF("SSS:theme item [%s]", yytext);

    switch ( ga_word_ix )
    {
        case 1  :                                                               //  theme macro name / keyword

            FLEX_INF("STS:item #01:channel [%s]", yytext);
            ga_word_ix++;

            yylval.vstr = g_yytext_dup();
            return libgwr::log::lexer::Token( TOKEN_LOG_THEME_MACRO );

        case 2  :                                                               //  theme channel

            FLEX_INF("STS:item #02:macro [%s]", yytext);
            ga_word_ix++;

            //sss_begin();                                                        //  begin subchannels scan

            yylval.vstr = g_yytext_dup();
            return libgwr::log::lexer::Token( TOKEN_LOG_THEME_CHANNEL );

        default             :

            FLEX_ERR("STS:item #[%i] [%s]", ga_word_ix, yytext );
            LIBGWR_GEN_LOG_FLEX_ERROR_JAMMED();

    }

}
    /*
     *  theme : subchanel
     */
{R_theme_sub_name}\:                                                            {

    sss_name = g_cut_last_char_and_dup( yytext );                               //  drop the ':'
    FLEX_INF("STS:subchannel spec [%s]", sss_name);

    yylval.vstr = sss_name;
    sss_begin();
    return libgwr::log::lexer::Token( TOKEN_LOG_THEME_SUBCHANNEL_NEW );

}
    /*
     *  any char except \n and {wsp}
     */
.                                                                               {

    FLEX_ERR("STS:JAMMED [%c]", yytext[0]);
    LIBGWR_GEN_LOG_FLEX_ERROR_JAMMED();

}
    /*
     *  newline
     */
{nl}                                                                            {

    yyless(0);                                                                  //  make ga_word_ix still valid
    si_begin();

}

}   /* <STATE_THEME_SPECIFICATIONS> */
                                                                                /*
    ****************************************************************************
    STATE_SUBCHANNEL_SPECIFICATIONS

    ( The most complicated rules, due to colors & attributes )
    ****************************************************************************
                                                                                */
<STATE_SUBCHANNEL_SPECIFICATIONS>
{
    /*
     *  whitespace : drop
     */
{wsp}+                                                                          {

}
    /*
     *  subchannel : specification
     */
    /*  XXX,    */
{R_theme_sub_spec},                                                             {

    if ( sss_in_colors )
    {
        if ( sss_color_fg )
        {
            FLEX_ERR("%s", "SSS:too much colors given");
            LIBGWR_GEN_LOG_FLEX_ERROR_SYNTAX();
        }
        sss_color_fg    =   g_cut_last_char_and_dup( yytext );
        yylval.vstr     =   sss_color_fg;
        FLEX_INF("SSS:color fg [%s]", sss_color_fg);
        return libgwr::log::lexer::Token( TOKEN_LOG_THEME_SUBCHANNEL_COLOR_FG );

    }

    if ( sss_in_attrs )
    {
        gchar   *   temp    =   g_cut_last_char_and_dup( yytext );
        //sss_attributes->add( temp );
        yylval.vstr         =   temp;
        FLEX_INF("SSS:attribute [%s]", temp);
        return libgwr::log::lexer::Token( TOKEN_LOG_THEME_SUBCHANNEL_FORMAT );
    }

}
    /*  XXX:    */
{R_theme_sub_spec}:                                                             {

    if ( sss_in_attrs )
    {
        FLEX_ERR("%s", "SSS:subchannel doesnt admit specifications after attributes");
        LIBGWR_GEN_LOG_FLEX_ERROR_SYNTAX();
    }

    if ( sss_in_colors )
    {
        sss_color_bg    =   g_cut_last_char_and_dup( yytext );
        yylval.vstr     =   sss_color_bg;
        FLEX_INF("SSS:color bg [%s]", sss_color_bg);
        sss_in_colors   =   FALSE;
        sss_in_attrs    =   TRUE;
        return libgwr::log::lexer::Token( TOKEN_LOG_THEME_SUBCHANNEL_COLOR_BG );
    }

}
    /*  XXX     */
{R_theme_sub_spec}                                                              {

    if ( sss_in_attrs )
    {
        gchar   *   temp    =   g_yytext_dup();
        //sss_attributes->add( temp );
        yylval.vstr         =   temp;
        FLEX_INF("SSS:attribute ( end ) [%s]", temp);
        sts_begin();
        return libgwr::log::lexer::Token( TOKEN_LOG_THEME_SUBCHANNEL_FORMAT );
    }

    if ( sss_in_colors )
    {
        if ( sss_color_fg )
        {
            sss_color_bg    =   g_yytext_dup();
            yylval.vstr     =   sss_color_bg;
            FLEX_INF("SSS:color bg ( end )[%s]", sss_color_bg);
            sts_begin();
            return libgwr::log::lexer::Token( TOKEN_LOG_THEME_SUBCHANNEL_COLOR_BG );
        }
        else
        {
            sss_color_fg    =   g_yytext_dup();
            yylval.vstr     =   sss_color_fg;
            FLEX_INF("SSS:color fg ( end ) [%s]", sss_color_fg);
            sts_begin();
            return libgwr::log::lexer::Token( TOKEN_LOG_THEME_SUBCHANNEL_COLOR_FG );
        }
    }

}
    /*
     *  any char except \n and {wsp}
     */
.                                                                               {

    FLEX_ERR("STS:JAMMED [%c]", yytext[0]);
    LIBGWR_GEN_LOG_FLEX_ERROR_JAMMED();

}
    /*
     *  newline
     */
{nl}                                                                            {

    yyless(0);                                                                  //  make ga_word_ix still valid
    si_begin();

}

}   /* <STATE_SUBCHANNEL_SPECIFICATIONS> */
                                                                                /*
    ****************************************************************************
    STATE_ROL_STRING

    Return the Rest Of the current Line as a string
        - delimiter left id {wsp}
        - delimiter right is \n

    This state is always pushed, so at the end we pop our future state
    ****************************************************************************
                                                                                */
<STATE_ROL_STRING>
{
        /*  wsp : swallow or add */
{wsp}                                                                           {

    //printf("(1)\n");
    //  if in text, write wsp
    if ( ga_srol_string_flag )
    {
        *( ga_srol_string_p++ ) = yytext[0];
        *( ga_srol_string_p   ) = 0;
    }
    //  else drop it
}

        /*  non wsp */
[^ \t\n]                                                                        {

    //printf("(2)\n");
    ga_srol_string_flag = TRUE;
    *( ga_srol_string_p++ ) = yytext[0];
    *( ga_srol_string_p   ) = 0;

}

        /* newline */
{nl}                                                                            {

    //printf("<3>\n");
    gd_srol_string  =   g_strdup( ga_srol_string_chars );
    yylval.vstr     =   gd_srol_string;
    yy_pop_state();
    yyless(0);                                                                  //  make ga_word_ix still valid
    return TOKEN_STD_STRING;
}

}   /* <STATE_ROL_STRING> */
                                                                                /*
    ****************************************************************************
    STATE_ROL_SPLIT

    Split the Rest Of the current Line as a string array
        - delimiter left is whitespasce
        - delimiter right is whitespasce

    This state is always pushed, so at the end we pop our future state
    ****************************************************************************
                                                                                */
<STATE_ROL_SPLIT>
{
     /*  whitespace : ignore */
{wsp}+                                                                          {

}
     /*  theme item */
{R_theme_item}                                                                  {

    FLEX_INF("STATE_ROL_SPLIT:adding string [%s]", yytext);
    gd_srol_split_array->add( yytext );

}

    /*  newline */
{nl}                                                                            {

    yy_pop_state();
    yyless(0);                                                                  //  make ga_word_ix still valid
    yylval.vvoid = gd_srol_split_array;
    return TOKEN_STD_STRING_ARRAY;

    }

}   /* <STATE_ROL_SPLIT> */
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

//  ============================================================================
//  STATE_INITIAL variables & functions
//  ============================================================================
void    si_reset()
{
    si_theme_card_current   = 0;

    ga_word_ix              = 0;
}
void    si_begin()
{
    si_reset();
    BEGIN(INITIAL);
}
//  ============================================================================
//  STATE_ROL_STRING variables & functions
//  ============================================================================
void    srol_string_reset   ()
{
    ga_srol_string_flag     =   FALSE;
    ga_srol_string_chars[0] =   0;
    ga_srol_string_p        =   ga_srol_string_chars;
    gd_srol_string          =   NULL;
}
void    srol_string_begin   ()
{
    srol_string_reset();
    yy_push_state( STATE_ROL_STRING );
}
//  ============================================================================
//  STATE_ROL_SPLIT variables & functions
//  ============================================================================
void    srol_split_reset   ()
{
    gd_srol_split_array     =   GWR_NEW_CAST( libgwr::log::parsing::tStringArray, 150, 2);
}
void    srol_split_begin   ()
{
    srol_split_reset();
    yy_push_state( STATE_ROL_SPLIT );
}
//  ============================================================================
//  STATE_FIND_NAMESPACES_PREFIX variables & functions
//  ============================================================================
void    sfnp_reset      ()
{
}
void    sfnp_begin      ()
{
    sfnp_reset();
    BEGIN( STATE_FIND_NAMESPACES_PREFIX );
}
//  ============================================================================
//  STATE_FIND_NAMESPACE_NAME variables & functions
//  ============================================================================
void    sfnn_reset(eSfnnNspMode _mode)
{
    sfnn_nsp_mode   = _mode;
}
void    sfnn_begin(eSfnnNspMode _mode)
{
    sfnn_reset(_mode );
    BEGIN(STATE_FIND_NAMESPACE_NAME);
}
//  ============================================================================
//  STATE_THEME_SPECIFICATIONS variables & functions
//  ============================================================================
void    sts_reset()
{
}
void    sts_begin()
{
    sts_reset();
    BEGIN(STATE_THEME_SPECIFICATIONS);
}
//  ============================================================================
//  STATE_SUBCHANNELS_SPECIFICATIONS variables & functions
//  ============================================================================
void    sss_reset()
{
    sss_in_colors   =   TRUE;
    sss_in_attrs    =   FALSE;

    sss_color_fg    =   NULL;
    sss_color_bg    =   NULL;

    //sss_attributes  =   GWR_NEW_CAST( libgwr::log::parsing::tStringArray, 150, 2 );

    //sss_lss         =   GWR_NEW_CAST( libkconfig::generated::log::LexerSubSpec );
}
void    sss_begin()
{
    sss_reset();
    BEGIN(STATE_SUBCHANNEL_SPECIFICATIONS);
}
