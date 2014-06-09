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
