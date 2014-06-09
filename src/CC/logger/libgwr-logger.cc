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
#include    "libgwr.hh"
#include    <fcntl.h>
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
//  ****************************************************************************
//
//                              Static
//
//  ****************************************************************************
//gchar   Logger::s2  [4096];
//  ############################################################################
//
//  Logger
//
//  ############################################################################
Logger::Logger(
    guint32     _channel_card_max,
    guint32     _channel_reallocs)
{
    //GError  *   ge  =   NULL;
    //  ........................................................................
    d_channels          =   GWR_NEW_CAST( TArrayP < Channel >, _channel_card_max, _channel_reallocs);

    d_list_channels     =   NULL;
    g_mutex_init( & d_list_channels_mtx );

    //a_thread_ref        =   g_thread_create(
    //                            libgwr::Logger::ThreadFunc  ,
    //                            (gpointer)this              ,
    //                            TRUE                        ,
    //                            &ge                         );
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
gpointer
Logger::ThreadFunc(gpointer _data)
{
    Logger          *   THIS    =   NULL;

    GList           *   l       =   NULL;
    Channel         *   cha     =   NULL;

    size_t              rd      =   0;

    static  tFifoMsgHeader      mh;
    static  gchar               msg [1024];
    //  ........................................................................
    THIS    =   (Logger*)_data;

    //  ........................................................................
lab_main:

    g_mutex_lock( & THIS->d_list_channels_mtx );


    //printf("ThreadFunc:%i\n", g_list_length( THIS->d_list_channels ) );

    l = g_list_first( THIS->d_list_channels );

    //  ........................................................................
lab_chan_loop:

    if ( ! l )
        goto lab_chan_finished;

    cha = (Channel*)(l->data);

    rd= fread( (void*)(&mh), tFifoMsgHeader::Size, 1, cha->fifo_fl_r() );
    if ( rd != 1 )
    {
        goto lab_chan_next;
    }
    if ( mh.magic != 0xfed4 )
    {
        printf("ThreadFunc [%p]:bad magic magic [%x]", THIS, mh.magic);
        goto lab_chan_next;
    }

    rd= fread( (void*)(msg), mh.size, 1, cha->fifo_fl_r() );
    if ( rd != 1 )
    {
        printf("ThreadFunc [%p]:could not read msg of %i bytes\n", THIS, mh.size);
        perror("  error:");
        goto lab_chan_next;
    }

    msg[ mh.size ] = 0;
    THIS->p0_log( cha, cha->subchannel(mh.sub), msg );

lab_chan_next:
    l = g_list_next( l );

    goto lab_chan_loop;
    //  ........................................................................
lab_chan_finished:

    g_mutex_unlock( & THIS->d_list_channels_mtx );

    usleep( 1000 * 1000 );

    goto lab_main;

    return NULL;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean
Logger::channel_create(
            guint32                         _channel_index          ,
    const   gchar                       *   _header                 ,
            guint32                         _subchannel_card_max    ,
            guint32                         _subchannel_realloc     ,
            gboolean                        _fifo_buffered          )
{
    Channel     *   c   = NULL;
    //..........................................................................
    c = GWR_NEW(Channel, _channel_index, _header, _subchannel_card_max, _subchannel_realloc, _fifo_buffered);

    if ( ! d_channels->set( _channel_index, c ) )
    {
        delete c;
        return FALSE;
    }

    if ( ! c->fifo_buffered() )
    {
        if ( _fifo_buffered )
        {
            printf("err:requested fifo-buffered feature failed for channel %i\n", _channel_index);
            return FALSE;
        }
        return TRUE;
    }

    //  fifo buffering ? Add to thread list
    g_mutex_lock( & d_list_channels_mtx );

    d_list_channels = g_list_append( d_list_channels, (gpointer)c );

    g_mutex_unlock( & d_list_channels_mtx );

    return TRUE;
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
//  ----------------------------------------------------------------------------
void
Logger::p0_log          (Channel* c, SubChannel* s, const char* _text)
{
    GwrTextView         *   gtv     =   NULL;
    GwrGtkTextBuffer    *   b       =   NULL;
    //..........................................................................
    //  console
    if ( s->enabled_console() )
    {
        if ( c->get_header() )
        {
            if ( s->get_header() )
            {
                sprintf(s2, "%s:%s%s:\033[0m%s", c->get_header(), s->get_console_attributes(), s->get_header(), _text);
            }
            else
            {
                sprintf(s2, "%s:\033[0m%s", c->get_header(), _text);
            }
        }
        else
        {
            if ( s->get_header() )
            {
                sprintf(s2, "%s%s:\033[0m%s", s->get_console_attributes(), s->get_header(), _text);
            }
            else
            {
                sprintf(s2, "%s", _text);
            }
        }
        printf(s2);
    }
    //  ........................................................................
    //  GwrGtkTextView
    b   = c->get_output_gwrgtk_text_buffer();
    if ( b )
    {
        if ( s->enabled_textview() )
        {
            // channel header
            //if ( c->get_header() )
            //{
                //sprintf(s2, "%s:", c->get_header());
            //}

            // subchannel header
            if ( s->get_header() )
            {
                sprintf(s2, "%s:%s", s->get_header(), _text);
            }
            else
            {
                sprintf(s2, "%s", _text);
            }

            gwrgtk_text_buffer_add_line(b, s->flags(), _text);
            return;
        }
    }
    //  ........................................................................
    //  GwrTextView
    gtv = c->get_output_gwr_text_view();
    if ( gtv )
    {
        if ( s->enabled_textview() )
        {
            // channel header
            if ( c->get_header() )
            {
                sprintf(s2, "%s:", c->get_header());
                gtv->append     (s2, libgwr::TextAttributes::StdFlags);
            }

            // subchannel header
            if ( s->get_header() )
            {
                sprintf(s2, "%s:", s->get_header());
                gtv->append         (s2, s->flags());
            }

            // text
            sprintf(s2, "%s", _text);
            gtv->append         (s2 , s->flags());
        }
    }
    //  ........................................................................
    //  Stream
}
//  ----------------------------------------------------------------------------
void
Logger::p0_lognl        (Channel* c, SubChannel* s, const char* _text)
{
    GwrTextView         *   gtv     =   NULL;
    GwrGtkTextBuffer    *   b       =   NULL;
    //..........................................................................
    //  console
    if ( s->enabled_console() )
    {
        if ( c->get_header() )
        {
            if ( s->get_header() )
            {
                sprintf(s2, "%s:%s%s:\033[0m%s\n", c->get_header(), s->get_console_attributes(), s->get_header(), _text);
            }
            else
            {
                sprintf(s2, "%s:\033[0m%s\n", c->get_header(), _text);
            }
        }
        else
        {
            if ( s->get_header() )
            {
                sprintf(s2, "%s%s:\033[0m%s\n", s->get_console_attributes(), s->get_header(), _text);
            }
            else
            {
                sprintf(s2, "%s\n", _text);
            }
        }
        printf(s2);
    }
    //  ........................................................................
    //  GwrGtkTextView
    b   = c->get_output_gwrgtk_text_buffer();
    if ( b )
    {
        if ( s->enabled_textview() )
        {
            // channel header
            //if ( c->get_header() )
            //{
                //sprintf(s2, "%s:", c->get_header());
            //}

            // subchannel header
            if ( s->get_header() )
            {
                sprintf(s2, "%s:%s", s->get_header(), _text);
            }
            else
            {
                sprintf(s2, "%s", _text);
            }

            gwrgtk_text_buffer_add_line(b, s->flags(), _text);
            return;
        }
    }
    //  ........................................................................
    //  GwrTextView
    gtv = c->get_output_gwr_text_view();
    if ( gtv )
    {
        if ( s->enabled_textview() )
        {
            // channel header
            if ( c->get_header() )
            {
                sprintf(s2, "%s:", c->get_header());
                gtv->append     (s2, libgwr::TextAttributes::StdFlags);
            }

            // subchannel header
            if ( s->get_header() )
            {
                sprintf(s2, "%s:", s->get_header());
                gtv->append         (s2, s->flags());
            }

            // text
            sprintf(s2, "%s\n", _text);
            gtv->append         (s2 , s->flags());
        }
    }
}
//  ----------------------------------------------------------------------------
void
Logger::p0_write        (Channel* c, SubChannel* s, const char* _text)
{
    GwrTextView         *   gtv     =   NULL;
    GwrGtkTextBuffer    *   b       =   NULL;
    //  ........................................................................
    //  console
    if ( s->enabled_console() )
    {
        printf("%s", _text);
    }
    //  ........................................................................
    //  GwrGtkTextView
    b   = c->get_output_gwrgtk_text_buffer();
    if ( b )
    {
        if ( s->enabled_textview() )
        {
            // channel header
            //if ( c->get_header() )
            //{
                //sprintf(s2, "%s:", c->get_header());
            //}

            // subchannel header
            if ( s->get_header() )
            {
                sprintf(s2, "%s:%s", s->get_header(), _text);
            }
            else
            {
                sprintf(s2, "%s", _text);
            }

            gwrgtk_text_buffer_add_line(b, s->flags(), _text);
            return;
        }
    }
    //  ........................................................................
    //  GwrTextView
    gtv = c->get_output_gwr_text_view();
    if ( gtv )
    {
        if ( s->enabled_textview() )
        {
            // text
            gtv->append         (_text , s->flags());
        }
    }
}
//  ----------------------------------------------------------------------------
void
Logger::p0_write_html   (Channel* c, SubChannel* s, const char* _text)
{
    GwrTextView         *   gtv     =   NULL;
    GwrGtkTextBuffer    *   b       =   NULL;
    //  ........................................................................
    //  console
    if ( s->enabled_console() )
    {
        printf("<html>%s</html>", _text);
    }
    //  ........................................................................
    //  GwrGtkTextView
    b   = c->get_output_gwrgtk_text_buffer();
    if ( b )
    {
        if ( s->enabled_textview() )
        {
            printf("*** p0_write_html() unavailable for GwrGtkTextView ***\n");
        }
    }
    //  ........................................................................
    //  GwrTextView
    gtv = c->get_output_gwr_text_view();
    if ( gtv )
    {
        if ( s->enabled_textview() )
        {
            // text
            gtv->append_html( _text );
        }
    }
}
//  ----------------------------------------------------------------------------
void
Logger::p0_blog(Channel* c, SubChannel* s, guint32 _sub_id, const char* _text)
{
    tFifoMsgHeader      mh;
    //..........................................................................
    mh.magic=   0xfed4;
    mh.sub  =   (guint16)   ( _sub_id           & 0x0000ffff );
    mh.fmt  =   0;
    mh.size =   (guint16)   ( strlen( _text )   & 0x0000ffff );

    fwrite( (void*)(&mh     )   ,   tFifoMsgHeader::Size    ,   1   ,   c->fifo_fl_w());
    fwrite( (void*)(_text   )   ,   mh.size                 ,   1   ,   c->fifo_fl_w());
    fflush(c->fifo_fl_w());
}
//  ----------------------------------------------------------------------------
void
Logger::p0_blognl(Channel* c, SubChannel* s, guint32 _sub_id, const char* _text)
{
    /*
    tFifoMsgHeader      mh;
    //..........................................................................
    mh.magic=   0xfed4;
    mh.sub  =   (guint16)   ( _sub_id           & 0x0000ffff );
    mh.fmt  =   0;
    mh.size =   (guint16)   ( strlen( _text )   & 0x0000ffff );

    fwrite( (void*)(&mh     )   ,   tFifoMsgHeader::Size    ,   1   ,   c->fifo_fl_w());
    fwrite( (void*)(_text   )   ,   mh.size                 ,   1   ,   c->fifo_fl_w());
    fflush(c->fifo_fl_w());
    */
}
//  ----------------------------------------------------------------------------
void
Logger::p0_bwrite(Channel* c, SubChannel* s, guint32 _sub_id, const char* _text)
{
    /*
    tFifoMsgHeader      mh;
    //..........................................................................
    mh.magic=   0xfed4;
    mh.sub  =   (guint16)   ( _sub_id           & 0x0000ffff );
    mh.fmt  =   0;
    mh.size =   (guint16)   ( strlen( _text )   & 0x0000ffff );

    fwrite( (void*)(&mh     )   ,   tFifoMsgHeader::Size    ,   1   ,   c->fifo_fl_w());
    fwrite( (void*)(_text   )   ,   mh.size                 ,   1   ,   c->fifo_fl_w());
    fflush(c->fifo_fl_w());
    */
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
        gboolean                        _fifo_buffered          )
{
    a_index             =   _index;

    a_fifo_buffered     =   _fifo_buffered;
    a_fd_fifo_r         =   0;
    a_fd_fifo_w         =   0;
    a_fl_fifo_r         =   NULL;
    a_fl_fifo_w         =   NULL;
    d_fifo_location     =   NULL;

    //  set outputs to NULL
    set_header                      ( _header   );
    set_output_gwr_text_view        ( NULL      );
    set_output_gwrgtk_text_buffer   ( NULL      );
    set_output_fd                   ( 0         );
    set_output_fd_bin               ( 0         );

    // create subchannels storage
    d_subchannels           = GWR_NEW_CAST( TArrayP< SubChannel >, _subchannels_card_max, _subchannels_realloc);

    //  create fifo buffer ???
    if ( ! fifo_buffered() )
        return;

    if ( ! create_fifo() )
    {
        a_fifo_buffered =   FALSE;
        a_fd_fifo_r     =   0;
        a_fd_fifo_w     =   0;
        a_fl_fifo_r     =   NULL;
        a_fl_fifo_w     =   NULL;
    }
}
Logger::Channel::~Channel()
{
    g_free_safe (d_header);
    g_free_safe (d_fifo_location);

    g_free( d_subchannels );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
guint32             Logger::Channel::index()                                        {   return a_index;             }
void                Logger::Channel::set_header(const gchar* _header)               {   d_header = _header ? g_strdup(_header) : NULL; }
const gchar*        Logger::Channel::get_header()                                   {   return d_header;            }
void                Logger::Channel::set_output_gwr_text_view(GwrTextView* _gtv)    {    a_gwr_text_view = _gtv;    }
GwrTextView*        Logger::Channel::get_output_gwr_text_view()                     {    return a_gwr_text_view;    }
void                Logger::Channel::set_output_gwrgtk_text_buffer(GwrGtkTextBuffer* _b)    {    a_gwrgtk_text_buffer = _b;     }
GwrGtkTextBuffer*   Logger::Channel::get_output_gwrgtk_text_buffer()                        {    return a_gwrgtk_text_buffer;   }
void                Logger::Channel::set_output_fd(int _fd)                         {   a_fd = _fd;                 }
int                 Logger::Channel::get_output_fd()                                {   return a_fd;                }
void                Logger::Channel::set_output_fd_bin(int _fd_bin)                 {   a_fd_bin = _fd_bin;         }
int                 Logger::Channel::get_output_fd_bin()                            {   return a_fd_bin;            }

gboolean            Logger::Channel::fifo_buffered()                                {   return a_fifo_buffered;     }
const gchar *       Logger::Channel::fifo_location()                                {   return d_fifo_location;     }
FILE        *       Logger::Channel::fifo_fl_r()                                    {   return a_fl_fifo_r;         }
FILE        *       Logger::Channel::fifo_fl_w()                                    {   return a_fl_fifo_w;         }
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
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gboolean
Logger::Channel::create_fifo()
 {
    gchar                   str_fifo_location   [1024];
    //  ........................................................................
    //if ( get_header() )
    //    sprintf(str_fifo_location, "./gwrlogger-channel-%s.fifo", get_header());
    //else
        sprintf(str_fifo_location, "./gwrlogger-channel-%03i.fifo", a_index);

    d_fifo_location = g_strdup( str_fifo_location );

    if ( mkfifo(fifo_location(), S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH ) )
        if ( errno != EEXIST )
            return FALSE;

    a_fd_fifo_r = open( fifo_location(),  O_RDONLY | O_NONBLOCK );
    if ( ! a_fd_fifo_r )
        return FALSE;

    a_fd_fifo_w = open( fifo_location(), O_WRONLY | O_NONBLOCK );
    if ( ! a_fd_fifo_w )
        return FALSE;

    a_fl_fifo_r =   fdopen( a_fd_fifo_r, "r" );
    if ( ! a_fl_fifo_r )
        return FALSE;

    a_fl_fifo_w =   fdopen( a_fd_fifo_w, "w" );
    if ( ! a_fl_fifo_w )
        return FALSE;

    return TRUE;
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












