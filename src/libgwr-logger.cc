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
gboolean
Logger::p_channel_create(
    gboolean            _create_defaults                        ,
    guint32             _channel                                ,
    const gchar*        _header                                 ,
    gboolean            _output_console                         ,
    GwrTextView*        _output_textview                        ,
    int                 _output_fd                              ,
    int                 _output_fd_bin                          )
{
    g_return_val_if_fail( _channel < channel_card(),                        FALSE );
    g_return_val_if_fail( ! channel(_channel),                              FALSE );
    g_return_val_if_fail(       _output_console     || ( _output_textview != NULL )     ||
                                ( _output_fd != 0 ) || ( _output_fd_bin != 0 ),    FALSE );

    d_channel[_channel] = GWR_NEW(Channel, _channel, _create_defaults);

    channel(_channel)->set_header(_header);
    channel(_channel)->set_output_console       (_output_console);
    channel(_channel)->set_output_gwr_text_view (_output_textview);
    channel(_channel)->set_output_fd            (_output_fd);
    channel(_channel)->set_output_fd_bin        (_output_fd_bin);

    return TRUE;
}
gboolean
Logger::channel_create(
    guint32             _channel            ,
    const gchar*        _header             ,
    gboolean            _output_console     ,
    GwrTextView*        _output_textview    ,
    int                 _output_fd          ,
    int                 _output_fd_bin      )
{
    g_return_val_if_fail( _channel < a_channel_card ,   FALSE );
    g_return_val_if_fail( ! channel(_channel)       ,   FALSE );

    return p_channel_create(TRUE, _channel, _header, _output_console, _output_textview, _output_fd, _output_fd_bin);
}
gboolean
Logger::channel_create_nodefaults(
    guint32             _channel            ,
    const gchar*        _header             ,
    gboolean            _output_console     ,
    GwrTextView*        _output_textview    ,
    int                 _output_fd          ,
    int                 _output_fd_bin      )
{
    g_return_val_if_fail( _channel < a_channel_card ,   FALSE );
    g_return_val_if_fail( ! channel(_channel)       ,   FALSE );

    return p_channel_create(FALSE, _channel, _header, _output_console, _output_textview, _output_fd, _output_fd_bin);
}
gboolean
Logger::channel_release(guint32 _channel)
{
    g_return_val_if_fail( _channel < a_channel_card ,   FALSE );
    g_return_val_if_fail( channel(_channel)         ,   FALSE );

    delete d_channel[_channel];

    d_channel[_channel] = NULL;

    return TRUE;
}
//  ############################################################################
//
//  Channel
//
//  ############################################################################
Logger::Channel::Channel(guint32 _index, gboolean _create_defaults)
{
    a_index                 = _index;

    d_subchannels           = NULL;
    a_subchannels_card      = 0;

    d_header                = NULL;
    a_output_console        = TRUE;
    a_gwr_text_view         = NULL;
    a_fd                    = 0;

    // create subchannels storage
    d_subchannels           = g_try_new0(SubChannel*, 10);

    if ( ! _create_defaults )
        return;

    // build predefined subchannels
    create_subchannel("INF", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Grn   )   );
    create_subchannel("WNG", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Ora   )   );
    create_subchannel("ERR", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Red   )   );
    create_subchannel("TKI", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Grey6 )   );
    create_subchannel("TKW", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Blu1  )   );
    create_subchannel("TKE", libgwr::TextAttributes(libgwr::text::attributes::FgColor, libgwr::color::Blu   )   );
}
Logger::Channel::~Channel()
{
    g_free_safe(d_header);
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












