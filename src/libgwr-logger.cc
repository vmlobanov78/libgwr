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
namespace libgwr
{

//  ****************************************************************************
//
//                              Static
//
//  ****************************************************************************
gchar   Logger::s1  [4096];
gchar   Logger::s2  [4096];

//  ############################################################################
//
//  Channel
//
//  ############################################################################
Logger::Channel::Channel(gboolean _create_defaults)
{
    a_index                 = -1;

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
    create_subchannel("INF", libgwr::TextAttributes(ts::Fg          , color::Grn    ));
    create_subchannel("WNG", libgwr::TextAttributes(ts::Fg          , color::Ora    ));
    create_subchannel("ERR", libgwr::TextAttributes(ts::Fg          , color::Red    ));
    create_subchannel("TKI", libgwr::TextAttributes(ts::Fg          , color::Grey6  ));
    create_subchannel("TKW", libgwr::TextAttributes(ts::Fg          , color::Blu1   ));
    create_subchannel("TKE", libgwr::TextAttributes(ts::Fg          , color::Blu    ));
}
Logger::Channel::~Channel()
{
    if ( d_header )
        g_free(d_header);
}
//  ============================================================================
void                Logger::Channel::set_header(const gchar* _header)
{
    d_header = g_strdup(_header);
}
const gchar*        Logger::Channel::get_header()
{
    return d_header;
}
void                Logger::Channel::set_output_console(gboolean _b)
{
    a_output_console = _b;
}
gboolean            Logger::Channel::get_output_console()
{
    return a_output_console;
}
void                Logger::Channel::set_output_gwr_text_view(GwrTextView* _gtv)
{
    a_gwr_text_view = _gtv;
}
GwrTextView*        Logger::Channel::get_output_gwr_text_view()
{
    return a_gwr_text_view;
}
void                Logger::Channel::set_output_fd(int _fd)
{
    a_fd = _fd;
}
int                 Logger::Channel::get_output_fd()
{
    return a_fd;
}
void                Logger::Channel::set_output_fd_bin(int _fd_bin)
{
    a_fd_bin = _fd_bin;
}
int                 Logger::Channel::get_output_fd_bin()
{
    return a_fd_bin;
}
void                Logger::Channel::set_output_mq(svipc::MsgQueue* _mq)
{
    a_mq = _mq;
}
svipc::MsgQueue*    Logger::Channel::get_output_mq()
{
    return a_mq;
}

}       // namespace libgwr












