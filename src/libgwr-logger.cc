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
    for( i = 0 ; i != d_channels->p2_card() ; i++ )
    {
        c = d_channels->p2_get(i);

        if ( c )
            delete c;
    }

    delete d_channels;
}
//  ============================================================================
gboolean
Logger::p_channel_create(
    gboolean            _create_defaults                        ,
    guint32             _channel_index                          ,
    guint32             _subchannel_card_max                    ,
    guint32             _subchannel_reallocs                    ,
    const gchar*        _header                                 ,
    gboolean            _output_console                         ,
    GwrTextView*        _output_textview                        ,
    int                 _output_fd                              ,
    int                 _output_fd_bin                          )
{
    Channel     *   c   = NULL;
    //..........................................................................
    c = GWR_NEW(Channel, _channel_index, _subchannel_card_max, _subchannel_reallocs, _create_defaults);

    c->set_header               (_header);
    c->set_output_console       (_output_console);
    c->set_output_gwr_text_view (_output_textview);
    c->set_output_fd            (_output_fd);
    c->set_output_fd_bin        (_output_fd_bin);

    if ( d_channels->p2_set( _channel_index, c ) )
        return TRUE;

    delete c;
    return FALSE;
}
gboolean
Logger::channel_create(
    guint32             _channel            ,
    const gchar*        _header             ,
    guint32             _subchannel_card_max,
    guint32             _subchannel_reallocs,
    gboolean            _output_console     ,
    GwrTextView*        _output_textview    ,
    int                 _output_fd          ,
    int                 _output_fd_bin      )
{
    return p_channel_create(TRUE, _channel, _subchannel_card_max, _subchannel_reallocs, _header, _output_console, _output_textview, _output_fd, _output_fd_bin);
}
gboolean
Logger::channel_create_nodefaults(
    guint32             _channel            ,
    const gchar*        _header             ,
    guint32             _subchannel_card_max,
    guint32             _subchannel_reallocs,
    gboolean            _output_console     ,
    GwrTextView*        _output_textview    ,
    int                 _output_fd          ,
    int                 _output_fd_bin      )
{
    return p_channel_create(FALSE, _channel, _subchannel_card_max, _subchannel_reallocs, _header, _output_console, _output_textview, _output_fd, _output_fd_bin);
}
gboolean
Logger::channel_release(guint32 _channel)
{
    Channel     *   c = NULL;
    //..........................................................................
    c = d_channels->p2_get( _channel );

    if ( ! c )
        return FALSE;

    d_channels->p2_clr( _channel );
    delete c;
    return TRUE;
}
//  ############################################################################
//
//  Channel
//
//  ############################################################################
Logger::Channel::Channel(
    guint32     _index                  ,
    guint32     _subchannels_card_max   ,
    guint32     _subchannels_reallocs   ,
    gboolean    _create_defaults        )
{
    a_index                 = _index;

    d_header                = NULL;
    a_output_console        = TRUE;
    a_gwr_text_view         = NULL;
    a_fd                    = 0;

    // create subchannels storage
    d_subchannels           = GWR_NEW_CAST( TArrayP< SubChannel >, _subchannels_card_max, _subchannels_reallocs);

    if ( ! _create_defaults )
        return;

    // build predefined subchannels
    create_default_subchannels();
}
Logger::Channel::~Channel()
{
    g_free_safe(d_header);

    g_free( d_subchannels );
}

void
Logger::Channel::create_default_subchannels()
{
    create_subchannel(0, "INF", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Grn   )   );
    create_subchannel(1, "WNG", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Ora   )   );
    create_subchannel(2, "ERR", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Red   )   );
    create_subchannel(3, "TKI", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Grey6 )   );
    create_subchannel(4, "TKW", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Blu1  )   );
    create_subchannel(5, "TKE", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Blu   )   );
}
//  ============================================================================
void                Logger::Channel::set_header(const gchar* _header)               {   d_header = _header ? g_strdup(_header) : NULL; }
const gchar*        Logger::Channel::get_header()                                   {   return d_header;            }
void                Logger::Channel::set_output_console(gboolean _b)                {   a_output_console = _b;      }
gboolean            Logger::Channel::get_output_console()                           {   return a_output_console;    }
void                Logger::Channel::set_output_gwr_text_view(GwrTextView* _gtv)    {    a_gwr_text_view = _gtv;    }
GwrTextView*        Logger::Channel::get_output_gwr_text_view()                     {    return a_gwr_text_view;    }
void                Logger::Channel::set_output_fd(int _fd)                         {   a_fd = _fd;                 }
int                 Logger::Channel::get_output_fd()                                {   return a_fd;                }
void                Logger::Channel::set_output_fd_bin(int _fd_bin)                 {   a_fd_bin = _fd_bin;         }
int                 Logger::Channel::get_output_fd_bin()                            {   return a_fd_bin;            }

GWR_NAMESPACE_END(libgwr)












