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

