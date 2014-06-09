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
