/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-logger.cc                                                        *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libgwr                                                          *
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
    *   Class   : Logger                                                        *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/


//  ............................................................................
#include    "libgwr.h"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
//  ****************************************************************************
//
//                              Static
//
//  ****************************************************************************
gchar   Logger::s1  [4096];
gchar   Logger::s2  [4096];
//  ############################################################################
//
//  Logger
//
//  ############################################################################
Logger::Logger(
    guint32     _channel_card_max,
    guint32     _channel_reallocs)
{
    d_channels      = GWR_NEW_CAST( TArrayP < Channel >, _channel_card_max, _channel_reallocs);
}
Logger::~Logger()
{
    guint32         i = 0;
    Channel     *   c = NULL;
    //.....................................................................
    for( i = 0 ; i != d_channels->card() ; i++ )
    {
        d_channels->get(i, c);

        if ( c )
            delete c;
    }

    delete d_channels;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean
Logger::channel_create(
            guint32                         _channel_index          ,
    const   gchar                       *   _header                 ,
            guint32                         _subchannel_card_max    ,
            guint32                         _subchannel_realloc     ,
            libgwr::widget::GwrTextView *   _textview               ,               // = NULL
            int                             _fd                     ,               // = 0
            int                             _fd_bin                 )               // = 0
{
    Channel     *   c   = NULL;
    //..........................................................................
    c = GWR_NEW(Channel, _channel_index, _header, _subchannel_card_max, _subchannel_realloc);

    c->set_header               (_header);
    c->set_output_gwr_text_view (_textview);
    c->set_output_fd            (_fd);
    c->set_output_fd_bin        (_fd_bin);

    if ( d_channels->set( _channel_index, c ) )
        return TRUE;

    delete c;
    return FALSE;
}
gboolean
Logger::channel_release(guint32 _channel)
{
    Channel     *   c = NULL;
    //..........................................................................
    d_channels->get( _channel, c );

    if ( ! c )
        return FALSE;

    d_channels->clr( _channel );
    delete c;
    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
Logger::mute(guint32 _channel, guint32 _flavour, eOutput _o)
{
    Channel     * c = NULL;
    SubChannel  * s = NULL;
    //..............................................
    d_channels->get(_channel, c);
    g_return_if_fail( c );

    s = c->subchannel( _flavour );
    g_return_if_fail( c );

    s->enable( _o, FALSE );
}
void
Logger::unmute(guint32 _channel, guint32 _flavour, eOutput _o)
{
    Channel     * c = NULL;
    SubChannel  * s = NULL;
    //..............................................
    d_channels->get(_channel, c);
    g_return_if_fail( c );

    s = c->subchannel( _flavour );
    g_return_if_fail( c );

    s->enable( _o, TRUE );
}
void
Logger::mute(guint32 _channel, eOutput _o)
{
    Channel     * c = NULL;
    //..............................................
    d_channels->get(_channel, c);
    g_return_if_fail( c );

    c->enable( _o, FALSE );
}
void
Logger::unmute(guint32 _channel, eOutput _o)
{
    Channel     * c = NULL;
    //..............................................
    d_channels->get(_channel, c);
    g_return_if_fail( c );

    c->enable( _o, TRUE );
}

//  ############################################################################
//
//  Channel
//
//  ############################################################################
Logger::Channel::Channel(
        guint32                         _index                  ,
const   gchar                       *   _header                 ,
        guint32                         _subchannels_card_max   ,
        guint32                         _subchannels_realloc    ,
        libgwr::widget::GwrTextView *   _textview               ,               // = NULL
        int                             _fd                     ,               // = 0
        int                             _fd_bin                 )               // = 0
{
    a_index = _index;

    set_header                  ( _header   );
    set_output_gwr_text_view    ( _textview );
    set_output_fd               ( _fd       );
    set_output_fd_bin           ( _fd_bin   );

    // create subchannels storage
    d_subchannels           = GWR_NEW_CAST( TArrayP< SubChannel >, _subchannels_card_max, _subchannels_realloc);
}
Logger::Channel::~Channel()
{
    g_free_safe(d_header);

    g_free( d_subchannels );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void                Logger::Channel::set_header(const gchar* _header)               {   d_header = _header ? g_strdup(_header) : NULL; }
const gchar*        Logger::Channel::get_header()                                   {   return d_header;            }
void                Logger::Channel::set_output_gwr_text_view(GwrTextView* _gtv)    {    a_gwr_text_view = _gtv;    }
GwrTextView*        Logger::Channel::get_output_gwr_text_view()                     {    return a_gwr_text_view;    }
void                Logger::Channel::set_output_fd(int _fd)                         {   a_fd = _fd;                 }
int                 Logger::Channel::get_output_fd()                                {   return a_fd;                }
void                Logger::Channel::set_output_fd_bin(int _fd_bin)                 {   a_fd_bin = _fd_bin;         }
int                 Logger::Channel::get_output_fd_bin()                            {   return a_fd_bin;            }
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean
Logger::Channel::create_subchannel(
            guint32             _ix             ,
    const   gchar           *   _header         ,
            TextAttributes      _att            ,
            gboolean            _output_console ,
            gboolean            _output_textview)
{
    SubChannel * sub = NULL;
    //..........................................
    sub = GWR_NEW_CAST( SubChannel, _header, _att, _output_console, _output_textview);

    if ( ! d_subchannels->set( _ix, sub ) )
    {
        delete sub;
        return FALSE;
    }

    return TRUE;
}
void
Logger::Channel::create_default_subchannels(
    gboolean    _output_console     ,
    gboolean    _output_textview    )
{
    create_subchannel(0, "INF", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Grn   ), _output_console, _output_textview );
    create_subchannel(1, "WNG", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Ora   ), _output_console, _output_textview );
    create_subchannel(2, "ERR", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Red   ), _output_console, _output_textview );
    create_subchannel(3, "TKI", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Grey6 ), _output_console, _output_textview );
    create_subchannel(4, "TKW", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Blu1  ), _output_console, _output_textview );
    create_subchannel(5, "TKE", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Blu   ), _output_console, _output_textview );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
Logger::Channel::enable(
    Logger::eOutput     _o  ,
    gboolean            _b  )
 {
    guint32                 i   = 0;
    Logger::SubChannel  *   s   = NULL;
    //  ........................................................................
    for ( i = 0 ; i != d_subchannels->size_current() ; i++ )
    {
        d_subchannels->get( i, s );

        if ( !s )
            continue;

        s->enable( _o, _b );
    }
}
//  ############################################################################
//
//  SubChannel
//
//  ############################################################################
void
Logger::SubChannel::enable(
    Logger::eOutput     _o  ,
    gboolean            _b  )
{
    switch ( _o )
    {
    case    Logger::eOutputConsole      :   a_enabled_console   = _b;   break;
    case    Logger::eOutputTextView     :   a_enabled_textview  = _b;   break;
    default                             :                               break;
    }
}

GWR_NAMESPACE_END(libgwr)












