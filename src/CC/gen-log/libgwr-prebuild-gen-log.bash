#!/bin/bash
Root="$1"
source "${Root}/src/bash/libgwr.bash"
#   ----------------------------------------------------------------------------
function    build_parser
{
    bison "libgwr-gen-log.bison"                                                \
        --defines="libgwr-gen-log.bison.defines.h"                              \
        --report-file="libgwr-gen-log.bison.output"                             \
        -o "libgwr-gen-log.bison.parser.cc"
}
#   ----------------------------------------------------------------------------
function    build_lexer
{
    flex -o "libgwr-gen-log.flex.lexer.cc"                                      \
            "libgwr-gen-log.flex"
}
#   ############################################################################
echo "+------------------------------------------------------------------------+"
echo "| libgwr prebuild : generatig log lexer & parser                         |"
echo "+------------------------------------------------------------------------+"

cd "${Root}/src/CC/gen-log/parser"
#   ----------------------------------------------------------------------------
echo ">bison..."
#fn_libgwr_file_has_changed "libgwr-parser.bison"
#if [[ $(($?)) -eq 0 ]] ; then
#    echo ">  libgwr-parser.bison unchanged, doing nothing"
#else
#    echo ">  libgwr-parser.bison changed, building parser"
#    build_parser
#    cp  "libgwr-parser.bison" "libgwr-parser.bison.bak"
#fi

build_parser
#   ----------------------------------------------------------------------------
cd "${Root}/src/CC/gen-log/lexer"

echo "> concatening flex file..."

cat     libgwr-gen-log-01-header.flex               >   libgwr-gen-log.flex
cat     libgwr-gen-log-02-precode.flex              >>  libgwr-gen-log.flex
cat     libgwr-gen-log-03-regexp.flex               >>  libgwr-gen-log.flex
echo    "%%"                                        >>  libgwr-gen-log.flex
cat     libgwr-gen-log-04-rules.flex                >>  libgwr-gen-log.flex
cat     libgwr-gen-log-04-rules-01-rol.flex         >>  libgwr-gen-log.flex
#cat     libgwr-lexer-04-rules-02-help-v01.flex      >>  kconfig-lexer.flex
#cat     libgwr-lexer-04-rules-02-help-v02.flex      >>  kconfig-lexer.flex
#cat     libgwr-lexer-04-rules-02-help-v03.flex      >>  libgwr-lexer.flex
#cat     libgwr-lexer-04-rules-99-eof.flex           >>  libgwr-lexer.flex
echo    "%%"                                        >>  libgwr-gen-log.flex
cat     libgwr-gen-log-05-postcode.flex             >>  libgwr-gen-log.flex

echo ">flex..."
#fn_libgwr_file_has_changed "libgwr-lexer.flex"
#if [[ $(($?)) -eq 0 ]] ; then
#    echo ">  libgwr-lexer.flex unchanged, doing nothing"
#else
#    echo ">  libgwr-lexer.flex changed, building lexer"
#    build_lexer
#    cp  "libgwr-lexer.flex" "libgwr-lexer.flex.bak"
#fi

build_lexer


