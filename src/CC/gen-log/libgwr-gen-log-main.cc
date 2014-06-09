/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-gen-log-main.cc                                                  *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libkconfig                                                      *
    *                                                                           *
    *   Copyright (C) 2011-2013 Guillaume Wardavoir                             *
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
    *   Class   :                                                               *
    *                                                                           *
    *   Purpose : main() for gen-log debug                                      *
    *                                                                           *
    *****************************************************************************
*/

/*
extern  libgwr::log::Root   LibGwrLogRoot();



libgwr::log::Root   ga_libgwr_log_root;

libgwr::log::Root   LibGwrLogRoot()
{
    return ga_libgwr_log_root;
}

void                LibGwrLogInit()
{
    ...
}
*/
//  ............................................................................
#include    "libgwr-gen-log-parsing.hh"
//..............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(log)

//  ############################################################################
//                              GENERATE
//  ############################################################################
#define     F_APPEND(FI, FORMAT, ...)                                           \
    sprintf( FI##_tmp, FORMAT, __VA_ARGS__ );                                   \
    libgwr::str::append( FI, FI##_tmp, NULL);
//  ----------------------------------------------------------------------------
//  Generate_namespaces_prefixes_enter / exit()
//  ----------------------------------------------------------------------------
void        Generate_namespaces_prefixes_enter(
    FILE                *   _f)
{
    guint32                     i           = 0;
    parsing::tStringArray   *   npf         = parsing::Namespaces_prefixes();
    //  ........................................................................
    if ( npf->card() )
    {
        fprintf(_f, "%s", "//  ############################################################################\n");
        for ( i = 0 ; i != npf->card() ; i++ )
        {
            fprintf( _f, "namespace %s { ", npf->get( i ) );
        }
        fprintf( _f, "\n" );
    }
}
void        Generate_namespaces_prefixes_exit(
    FILE                *   _f)
{
    guint32                     i           = 0;
    parsing::tStringArray   *   npf         = parsing::Namespaces_prefixes();
    //  ........................................................................
    if ( npf->card() )
    {
        fprintf(_f, "%s", "//  ############################################################################\n");

        for ( i = 0 ; i != npf->card() ; i++ )
        {
            fprintf( _f, "}");
        }
        fprintf( _f, "\n" );
    }
}
//  ----------------------------------------------------------------------------
//  Generate_nsp_enter / exit()
//  ----------------------------------------------------------------------------
void        Generate_nsp_enter(
    FILE                *   _f,
    parsing::NSP        *   _nsp)
{
    parsing::NSP        *   parent  = NULL;
    //  ........................................................................
    parent = _nsp->parent();

    if ( parent )
    {
        Generate_nsp_enter(_f, parent);
    }

    fprintf( _f, "namespace %s { ", _nsp->n() );
}
void        Generate_nsp_exit(
    FILE                *   _f,
    parsing::NSP        *   _nsp)
{
    parsing::NSP        *   parent  = NULL;
    //  ........................................................................
    parent = _nsp->parent();

    if ( parent )
    {
        Generate_nsp_exit(_f, parent);
    }

    fprintf( _f, "}");
}
//  ----------------------------------------------------------------------------
//  generate - mono
//  ----------------------------------------------------------------------------
#include    "libgwr-gen-log-main-generate-mono.cci"
//  ----------------------------------------------------------------------------
//  generate - multi
//  ----------------------------------------------------------------------------
#include    "libgwr-gen-log-main-generate-multi.cci"

GWR_NAMESPACE_END(log)
GWR_NAMESPACE_END(libgwr)
//  ############################################################################
//                                  MAIN
//  ############################################################################
int main(int argc, char** argv)
{
    //  ........................................................................
    //  .def filename
    if ( argc < 2 )
        return 1;

    libgwr::log::parsing::File_def_set( argv[1] );

    //  printf("G_SLICE=%s\n", getenv("G_SLICE") );

    //  ........................................................................
    //  Go !
    libgwr::log::parsing::Parse();

    /*
    libgwr::log::Generate_mono_module_header();
    libgwr::log::Generate_mono_module_code_themes_and_flavours();
    libgwr::log::Generate_mono_module_code_functions();
    */

    libgwr::log::Generate_multi_module_header();
    libgwr::log::Generate_multi_module_code_functions();

    libgwr::log::parsing::Free();
    //libgwr::log::parsing::Reset();

    return 0;
}





