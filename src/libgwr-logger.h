/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-logger.h                                                         *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Helper for logging                                                      *
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


#ifndef __LIBGWR_LOGGER_H__
#define __LIBGWR_LOGGER_H__

//  ............................................................................
#include    "libgwr.h"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)

using namespace widget;
//  ............................................................................
#if LIBGWR_USE_LOGGER == 1
#define GWR_INF(format, ...)    libgwr::GD_libgwr_logger->log(GA_libgwr_logger_channel, 0, format, __VA_ARGS__)
#define GWR_WNG(format, ...)    libgwr::GD_libgwr_logger->log(GA_libgwr_logger_channel, 1, format, __VA_ARGS__)
#define GWR_ERR(format, ...)    libgwr::GD_libgwr_logger->log(GA_libgwr_logger_channel, 2, format, __VA_ARGS__)
#define GWR_TKI(format, ...)    libgwr::GD_libgwr_logger->log(GA_libgwr_logger_channel, 3, format, __VA_ARGS__)
#define GWR_TKW(format, ...)    libgwr::GD_libgwr_logger->log(GA_libgwr_logger_channel, 4, format, __VA_ARGS__)
#define GWR_TKE(format, ...)    libgwr::GD_libgwr_logger->log(GA_libgwr_logger_channel, 5, format, __VA_ARGS__)
#else
#define GWR_INF(format, ...)
#define GWR_WNG(format, ...)
#define GWR_ERR(format, ...)
#define GWR_TKI(format, ...)
#define GWR_TKW(format, ...)
#define GWR_TKE(format, ...)
#endif
/// ****************************************************************************
//!
//! \class  Logger
//!
//! \brief  Logger
//!
/// ****************************************************************************
class   Logger
{
public:
    class   Channel;
    class   SubChannel;
    //=========================================================================
    //	Types
    //=========================================================================
public:
    enum eLogFlavour
    {
        eLogInf         = 0x00  ,
        eLogWng         = 0x01  ,
        eLogErr         = 0x02  ,

        eLogTki         = 0x03  ,
        eLogTkw         = 0x04  ,
        eLogTke         = 0x05
    };
    enum    eOutput
    {
        eOutputConsole  =   0x01    ,
        eOutputTextView =   0x02
    };
    //=========================================================================
    //	Inner classes
    //=========================================================================
#include    "libgwr-logger-subchannel.hi"
#include    "libgwr-logger-channel.hi"
    //=========================================================================
    //	Members
    //=========================================================================
private:
    TArrayP < Channel >     *   d_channels;
    //=========================================================================
    //	Methods
    //=========================================================================
    //--------------------------------------------------------------------------
    //  ()~()
    //--------------------------------------------------------------------------
public:
    Logger(guint32 _channel_card_max, guint32 _channel_reallocs = 10);
    virtual ~Logger();
    //--------------------------------------------------------------------------
    //  get / set
    //--------------------------------------------------------------------------
public:
    inline  Channel *       channel(guint32 _channel)
    {
        Channel * cha = NULL;
        if ( d_channels->get( _channel, cha ) )
            return cha;
        return NULL;
    }
    //--------------------------------------------------------------------------
    //  mute / unmute / clear
    //--------------------------------------------------------------------------
public:
    void    mute    (guint32 _channel, guint32 _flavour, eOutput _o);
    void    unmute  (guint32 _channel, guint32 _flavour, eOutput _o);
    void    mute    (guint32 _channel, eOutput _o);
    void    unmute  (guint32 _channel, eOutput _o);
    /*
    inline  void            channel_clear   (guint32 _channel)
                            {
                                GwrTextView     *   gtv     = NULL;
                                Channel         *   c       = NULL;
                                //..............................................
                                c   =   d_channels->get(_channel);
                                g_return_if_fail(c);
                                //  ............................................
                                //  console
                                if ( c->get_output_console() )
                                {
                                    printf("clear\n");
                                }
                                //  ............................................
                                //  GwrTextView
                                gtv = c->get_output_gwr_text_view();
                                if ( gtv )
                                {
                                    gtv->clear();
                                }
                            }
    */
    //--------------------------------------------------------------------------
    //  add / remove channels
    //--------------------------------------------------------------------------
public:
    gboolean        channel_create(
        guint32                         _channel                    ,
        const   gchar               *   _header                     ,
        guint32                         _subchannel_card_max        ,
        guint32                         _subchannel_realloc         ,
        libgwr::widget::GwrTextView *   _textview           = NULL  ,
        int                             _fd                 = 0     ,
        int                             _fd_bin             = 0     );

    gboolean        channel_release(guint32 _channel);
    //--------------------------------------------------------------------------
    //  log functions
    //--------------------------------------------------------------------------
    #include    "libgwr-logger-log.hi"
};
/// ****************************************************************************
//!
//! \class  LogServer
//!
//! \brief  Logs server
//!
/// ****************************************************************************
class   LogServer
{
};

GWR_NAMESPACE_END(libgwr)

#endif //__LIBGWR_LOGGER_H__
