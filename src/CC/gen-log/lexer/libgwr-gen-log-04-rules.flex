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
