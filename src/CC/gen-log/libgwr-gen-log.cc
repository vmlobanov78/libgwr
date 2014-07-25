/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-log-generation.cc                                                *
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
    *   Class   :                                                               *
    *                                                                           *
    *   Purpose : Classes for organizing logging                                *
    *                                                                           *
    *****************************************************************************
*/

//  ............................................................................
#include    "libgwr.hh"
//..............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(log)
//  ############################################################################
//
//  libkconfig::log::Flavour
//
//  ############################################################################
Flavour::Flavour(
    const   gchar       *               _name                   ,
            guint32                     _global_index           ,
            guint32                     _log_subchannel_index   ,
            libgwr::TextAttributes      _text_attributes    )
{
    a_name.set          ( _name                 );
    set_gix             ( _global_index         );

    set_six_redirected  ( FALSE                 );
    set_six_mem         ( 0                     );
    set_six_core        ( _log_subchannel_index );
    set_six             ( _log_subchannel_index );

    a_text_attributes_flags         =   _text_attributes.flags();
}
Flavour::~Flavour()
{
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
Flavour::redirect(guint32 _other_subchannel)
{
    g_return_if_fail( ! redirected() );

    set_six_mem         ( six()             );
    set_six             ( _other_subchannel );
    set_six_redirected  ( TRUE              );
}
void
Flavour::unredirect()
{
    g_return_if_fail( redirected() );

    set_six             ( six_mem()         );
    set_six_mem         ( 0                 );
    set_six_redirected  ( FALSE              );
}
gboolean
Flavour::redirected()
{
    return six_redirected();
}
//  ############################################################################
//
//  libkconfig::log::Theme
//
//  ############################################################################
Theme::Theme(
        const gchar*    _name                   ,
        guint32         _global_index           ,
        guint32         _log_channel_index      )
{
    //  ....................................................................
    set_name            ( _name );
    set_gix             ( _global_index );

    set_cix_redirected  ( FALSE );
    set_cix_mem         ( 0 );
    set_cix_core        ( _log_channel_index );
    set_cix             ( _log_channel_index );

    d_flavours_ix_array             = GWR_NEW_CAST( libgwr::TArray< guint32 >, 20, 2 );
}
Theme::~Theme()
{
    delete d_flavours_ix_array;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    void                            unredirect  ();
    gboolean                        redirected  ();
void
Theme::redirect(guint32 _other_channel)
{
    g_return_if_fail( ! redirected() );

    set_cix_mem         ( cix()             );
    set_cix_redirected  ( TRUE              );
    set_cix             ( _other_channel    );
}
void
Theme::unredirect()
{
    g_return_if_fail( redirected() );

    set_cix             ( cix_mem()         );
    set_cix_mem         ( 0                 );
    set_cix_redirected  ( FALSE             );
}
gboolean
Theme::redirected()
{
    return cix_redirected();
}
//  ############################################################################
//
//  libkconfig::log::Namespace
//
//  ############################################################################
Namespace::Namespace(const gchar* _name)
{
    set_name( _name );

    d_themes_ix_array   = GWR_NEW_CAST( libgwr::TArray < guint32 >   , 30, 10 );
}
Namespace::~Namespace()
{
    delete d_themes_ix_array;
}
//  ############################################################################
//
//  libkconfig::log::Root
//
//  ############################################################################
Root::Root()
{
    d_namespaces_prefixes   =   GWR_NEW_CAST( libgwr::TArrayP< gchar >                  , 2  , 2 );
    d_namespaces            =   GWR_NEW_CAST( libgwr::TArrayP< libgwr::log::Namespace > , 50 , 2 );
    d_themes                =   GWR_NEW_CAST( libgwr::TArrayP< libgwr::log::Theme     > , 150, 2 );
    d_flavours              =   GWR_NEW_CAST( libgwr::TArrayP< libgwr::log::Flavour   > , 300, 2 );
}
Root::~Root()
{
    delete d_namespaces;
    delete d_themes;
    delete d_flavours;

    delete d_namespaces_prefixes;
    g_free_safe( d_namespaces_prefix );
}
//  ============================================================================
gboolean    Root::nsp_add( libgwr::log::Namespace*  _nsp  )
{
    return d_namespaces->add( _nsp );
}
gboolean    Root::thm_set( guint32 _ix, libgwr::log::Theme* _thm )
{
    return d_themes->set( _ix, _thm );
}
gboolean    Root::fla_set( guint32 _ix, libgwr::log::Flavour* _fla )
{
    return d_flavours->set( _ix, _fla );
}
//  ============================================================================
gboolean    Root::redirect(
    libgwr::log::Theme  *       _src_theme  ,
    libgwr::log::Theme  *       _dst_theme  ,
    guint32                     _dst_sub    )
{
    guint32                         i       =   0;
    libgwr::log::Flavour        *   fla     =   NULL;
    //  ........................................................................
    _src_theme->redirect( _dst_theme->cix() );

    for ( i = 0 ; i != _src_theme->fla_card() ; i++ )
    {
        fla =   this->fla( _src_theme->fla_ix( i ) );

        if ( ( _dst_sub + i ) >= _dst_theme->fla_card() )
        {
            LIBGWR_ERR("%s", "libgwr::log::Root::redirect():not enough flavours on destination channel");
            return FALSE;
        }
        else
        {
            fla->redirect( _dst_sub + i );
        }
    }

    return TRUE;
}
gboolean    Root::unredirect(
    libgwr::log::Theme  *       _src_theme  )
{
    guint32                         i       =   0;
    libgwr::log::Flavour        *   fla     =   NULL;
    //  ........................................................................
    _src_theme->unredirect();

    for ( i = 0 ; i != _src_theme->fla_card() ; i++ )
    {
        fla =   this->fla( _src_theme->fla_ix( i ) );
        fla->unredirect();
    }

    return TRUE;
}

GWR_NAMESPACE_END(log)
GWR_NAMESPACE_END(libkconfig)


