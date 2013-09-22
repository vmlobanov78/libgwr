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
//  ............................................................................
class Logger
{
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
    /// ************************************************************************
    //!
    //! \class  SubChannel
    //!
    //! \brief  Channel
    //!         |
    //!         +----- SubChannel #1
    //!         |
    //!         +----- SubChannel #2
    //!         |
    //!         +----- etc...
    //!
    /// ************************************************************************
    private:
    class SubChannel
    {
        private:
        gchar               *   d_header;
        gboolean                a_muted;
        TextAttributes          a_att;
        gchar                   a_att_str[16];

        public:
                SubChannel(const gchar* _header, TextAttributes _att) :
                    a_muted(FALSE), a_att(_att)
                {
                    d_header    = _header ? g_strdup(_header) : NULL;

                    bzero( (void*)a_att_str, 16 );
                    libgwr::text::G_console_add_attributes(a_att_str, _att);
                }
        virtual ~SubChannel()
                {
                    g_free_safe(d_header);
                }

        inline  const   gchar *     get_header()    {   return d_header;        }
        inline  void                mute()          {   a_muted = TRUE;         }
        inline  void                unmute()        {   a_muted = FALSE;        }
        inline  gboolean            is_muted()      {   return a_muted;         }
        //----------------------------------------------------------------------
        inline  guint32             flags()         {   return a_att.flags();   }
        inline  guint32             fgcol()         {   return a_att.fgcol();   }
        inline  guint32             bgcol()         {   return a_att.bgcol();   }
        inline  const gchar *       get_console_attributes()
                                    {
                                        return a_att_str;
                                    }
    };
    /// ************************************************************************
    //!
    //! \class  Channel
    //!
    //! \brief  Logger
    //!         |
    //!         +----- Channel #1
    //!         |
    //!         +----- Channel #2
    //!         |
    //!         +----- etc...
    //!
    /// ************************************************************************
    private:
    class Channel
    {
        private:
        guint32             a_index;

        SubChannel      **  d_subchannels;
        guint32             a_subchannels_card;

        gchar           *   d_header;
        gboolean            a_output_console;
        GwrTextView     *   a_gwr_text_view;
        int                 a_fd;
        int                 a_fd_bin;

        public:
        Channel(guint32 _index, gboolean _create_default = TRUE);
        ~Channel();
        //----------------------------------------------------------------------
        void                set_header              (const gchar* _header);
        const gchar*        get_header              ();
        void                set_output_console      (gboolean _b);
        gboolean            get_output_console      ();
        void                set_output_gwr_text_view(GwrTextView* _gtv);
        GwrTextView*        get_output_gwr_text_view();
        void                set_output_fd           (int);
        int                 get_output_fd           ();
        void                set_output_fd_bin       (int);
        int                 get_output_fd_bin       ();
        //----------------------------------------------------------------------
        inline  guint32         create_subchannel(const gchar* _header, TextAttributes _att)
                                {
                                    g_return_val_if_fail( a_subchannels_card < 20, 3 );

                                    d_subchannels[a_subchannels_card] = GWR_NEW_CAST( SubChannel, _header, _att );

                                    return a_subchannels_card++;
                                }
        inline  SubChannel  *   subchannel(guint32 _index)
                                {
                                    g_return_val_if_fail( _index < a_subchannels_card, NULL );
                                    return d_subchannels[_index];
                                }
        inline  void            unmute(guint32 _flavour)
                                {
                                    subchannel(_flavour)->unmute();
                                }
        inline  void            mute(guint32 _flavour)
                                {
                                    subchannel(_flavour)->mute();
                                }
        inline  gboolean        is_muted(guint32 _flavour)
                                {
                                    return subchannel(_flavour)->is_muted();
                                }
    };
    //=========================================================================
    //	Members
    //=========================================================================
    private:
    guint32         a_channel_card;
    Channel     **  d_channel;
    //=========================================================================
    //	Methods
    //=========================================================================
    //--------------------------------------------------------------------------
    //  ()~()
    //--------------------------------------------------------------------------
    public:
    Logger(guint32 _channel_card)
    {
        a_channel_card  = _channel_card;
        d_channel       = (Channel**)g_try_malloc0( sizeof(Channel*) * _channel_card );
    }
    ~Logger()
    {
        guint32 i = 0;
        //.....................................................................
        for( i = 0 ; i != channel_card() ; i++ )
            if ( channel(i) )
                delete (Object<Channel>*)channel(i);

        g_free(d_channel);
    }
    //--------------------------------------------------------------------------
    //  get / set
    //--------------------------------------------------------------------------
    public:
    inline  guint32         channel_card()
                            {
                                return a_channel_card;
                            }
    inline  Channel *       channel(guint32 _channel)
                            {
                                g_return_val_if_fail( ( _channel < a_channel_card ), NULL );
                                return d_channel[_channel];
                            }
    inline  const gchar *   header(guint32 _channel)
                            {
                                g_return_val_if_fail( ( _channel < a_channel_card ), NULL );
                                return channel(_channel)->get_header();
                            }
    //--------------------------------------------------------------------------
    //  mute / unmute / clear
    //--------------------------------------------------------------------------
    public:
    inline  void            channel_mute    (guint32 _channel, eLogFlavour _flavour)
                            {
                                g_return_if_fail( ( _channel < a_channel_card ) );
                                channel(_channel)->mute   (_flavour);
                            }
    inline  void            channel_unmute  (guint32 _channel, eLogFlavour _flavour)
                            {
                                g_return_if_fail( ( _channel < a_channel_card ) );
                                channel(_channel)->unmute (_flavour);
                            }
    inline  void            channel_clear   (guint32 _channel)
                            {
                                GwrTextView     *   gtv     = NULL;
                                Channel         *   c       = NULL;
                                //..............................................
                                g_return_if_fail( ( _channel < a_channel_card ) );
                                c   =   channel(_channel);
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
                                guint32             _channel                                ,
                                const gchar*        _header             = NULL              ,
                                gboolean            _output_console     = TRUE              ,
                                GwrTextView*        _output_textview    = NULL              ,
                                int                 _output_fd          = NULL              ,
                                int                 _output_fd_bin      = 0                 );
    public:
    gboolean        channel_create(
                                guint32             _channel                                ,
                                const gchar*        _header             = NULL              ,
                                gboolean            _output_console     = TRUE              ,
                                GwrTextView*        _output_textview    = NULL              ,
                                int                 _output_fd          = NULL              ,
                                int                 _output_fd_bin      = 0                 );
    gboolean        channel_create_nodefaults(
                                guint32             _channel                                ,
                                const gchar*        _header             = NULL              ,
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

GWR_NAMESPACE_END(libgwr)

#endif //__LIBGWR_LOGGER_H__
