/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-logger.hh                                                        *
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
    *   Purpose :                                                               *
    *                                                                           *
    *****************************************************************************
*/


#ifndef __LIBGWR_LOGGER_HH__
#define __LIBGWR_LOGGER_HH__

//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)

using namespace widget;
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
    //==========================================================================
    //	Types
    //==========================================================================
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
    //==========================================================================
    //	Inner classes
    //==========================================================================
    public:
    struct  _tFifoMsgHeader
    {
        static  const   guint32 Size    =   10;

        guint16             magic;                                              //  0xfed4;
        guint16             sub;
        guint32             fmt;
        guint16             size;
    };
    typedef struct _tFifoMsgHeader tFifoMsgHeader;

    #include    "libgwr-logger-subchannel.hhi"
    #include    "libgwr-logger-channel.hhi"
    //==========================================================================
    //	Channels
    //==========================================================================
private:
    TArrayP < Channel >     *   d_channels;

    //==========================================================================
    //	Thread
    //==========================================================================
private:
    GThread                 *   a_thread_ref;

private:
            GThread         *   thread_ref()    { return a_thread_ref;  }

            GList           *   d_list_channels;
            GMutex              d_list_channels_mtx;

    static  gpointer            ThreadFunc(gpointer);
    //==========================================================================
    //	Methods
    //==========================================================================
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
        gboolean                        _fifo_buffered      = FALSE );

    gboolean        channel_release(guint32 _channel);
    //--------------------------------------------------------------------------
    //  log functions
    //--------------------------------------------------------------------------
    private:
    void    p0_log          (Channel*, SubChannel*, const gchar* _txt);
    void    p0_lognl        (Channel*, SubChannel*, const gchar* _txt);
    void    p0_write        (Channel*, SubChannel*, const gchar* _txt);
    void    p0_write_html   (Channel*, SubChannel*, const gchar* _txt);

    void    p0_blog     (Channel*, SubChannel*, guint32 _sub_id, const gchar* _txt);
    void    p0_blognl   (Channel*, SubChannel*, guint32 _sub_id, const gchar* _txt);
    void    p0_bwrite   (Channel*, SubChannel*, guint32 _sub_id, const gchar* _txt);

    public:
    #include    "libgwr-logger-log.hhi"
};

GWR_NAMESPACE_END(libgwr)

#endif                                                                          //  __LIBGWR_LOGGER_HH__
