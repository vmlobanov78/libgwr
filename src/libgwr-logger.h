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
                                return d_channels->p2_get( _channel );
                            }
    inline  const gchar *   header(guint32 _channel)
                            {
                                Channel* c = NULL;
                                //..............................................
                                c = d_channels->p2_get(_channel);
                                return  c ? c->get_header() : NULL;
                            }
    //--------------------------------------------------------------------------
    //  mute / unmute / clear
    //--------------------------------------------------------------------------
    public:
    inline  void            channel_mute    (guint32 _channel, eLogFlavour _flavour)
                            {
                                Channel* c = NULL;
                                //..............................................
                                c = d_channels->p2_get(_channel);

                                c->mute(_flavour);
                            }
    inline  void            channel_unmute  (guint32 _channel, eLogFlavour _flavour)
                            {
                                Channel* c = NULL;
                                //..............................................
                                c = d_channels->p2_get(_channel);

                                c->unmute(_flavour);
                            }
    inline  void            channel_clear   (guint32 _channel)
                            {
                                GwrTextView     *   gtv     = NULL;
                                Channel         *   c       = NULL;
                                //..............................................
                                c   =   d_channels->p2_get(_channel);
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
    //--------------------------------------------------------------------------
    //  add / remove channels
    //--------------------------------------------------------------------------
    private:
    gboolean        p_channel_create(
                                gboolean            _create_defaults            ,
                                guint32             _channel                    ,
                                guint32             _subchannel_card_max        ,
                                guint32             _subchannel_reallocs        ,
                                const gchar*        _header                     ,
                                gboolean            _output_console             ,
                                GwrTextView*        _output_textview            ,
                                int                 _output_fd                  ,
                                int                 _output_fd_bin              );
    public:
    gboolean        channel_create(
                                guint32             _channel                                ,
                                const gchar*        _header                                 ,
                                guint32             _subchannel_card_max=10                 ,
                                guint32             _subchannel_reallocs=10                 ,
                                gboolean            _output_console     = TRUE              ,
                                GwrTextView*        _output_textview    = NULL              ,
                                int                 _output_fd          = NULL              ,
                                int                 _output_fd_bin      = 0                 );
    gboolean        channel_create_nodefaults(
                                guint32             _channel                                ,
                                const gchar*        _header                                 ,
                                guint32             _subchannel_card_max=10                 ,
                                guint32             _subchannel_reallocs=10                 ,
                                gboolean            _output_console     = TRUE              ,
                                GwrTextView*        _output_textview    = NULL              ,
                                int                 _output_fd          = 0                 ,
                                int                 _output_fd_bin      = 0                 );
    gboolean        channel_release(guint32 _channel);
    //--------------------------------------------------------------------------
    //  log
    //--------------------------------------------------------------------------
    static char			s1  [4096];
    static char			s2  [4096];
    public:
    inline   void
    log(guint32 _channel, guint32 _subchannel, const char* _format, ...)
    {
        va_list             val;
        GwrTextView     *   gtv     = NULL;
        //int                 fd      = 0;
        Channel         *   c       = NULL;
        SubChannel      *   s       = NULL;
        //.........................................................................
        c   =   channel(_channel);
        g_return_if_fail(c);

        s   = c->subchannel(_subchannel);
        if ( ! s )
        {
            printf("tt\n");
            return;
        }
        g_return_if_fail(s);

        if ( s->is_muted() )
            return;

        va_start(val, _format); vsprintf(s1, _format, val); va_end(val);
        //  ....................................................................
        //  console
        if ( c->get_output_console() )
        {
            if ( c->get_header() )
            {
                if ( s->get_header() )
                {
                    sprintf(s2, "%s:%s%s:\033[0m%s\n", c->get_header(), s->get_console_attributes(), s->get_header(), s1);
                }
                else
                {
                    sprintf(s2, "%s:\033[0m%s\n", c->get_header(), s1);
                }
            }
            else
            {
                if ( s->get_header() )
                {
                    sprintf(s2, "%s%s:\033[0m%s\n", s->get_console_attributes(), s->get_header(), s1);
                }
                else
                {
                    sprintf(s2, "%s\n", s1);
                }
            }
            printf(s2);
        }
        //  ....................................................................
        //  GwrTextView
        gtv = c->get_output_gwr_text_view();
        if ( gtv )
        {
            // channel header
            if ( c->get_header() )
            {
                sprintf(s2, "%s:", c->get_header());
                gtv->append     (s2, libgwr::TextAttributes().flags());
            }

            // subchannel header
            if ( s->get_header() )
            {
                sprintf(s2, "%s:", s->get_header());
                gtv->append         (s2, s->flags());
            }

            // text
            sprintf(s2, "%s\n", s1);
            gtv->append         (s2 , s->flags());
        }
        //  ....................................................................
        //  Stream
    /*
        fd = c->get_output_fd();
        if ( fd )
        {
            // NOT CODED
        }
        //  ....................................................................
        //  Binary stream
        fd = c->get_output_fd_bin();
        if ( fd )
        {
            if ( (c->get_header())[0] )                                         // NOTE optim for gkconfig
                sprintf(s2, "%s:%s:%s", c->get_header(), s->get_header(), s1);
            else
                sprintf(s2, "%s:%s", s->get_header(), s1);

            gchar   buffer      [4];
            int     len         = strlen(s2);


            buffer[0]                   = (guint8)_channel;
            buffer[1]                   = (guint8)_subchannel;
            //// *( (guint16*)(&buffer[2]) ) = (guint16)len;

            write(fd, buffer, 4);
            write(fd, s2, (size_t)len);
        }
        */
    }
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
