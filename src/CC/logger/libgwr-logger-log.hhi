/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-logger-log.hhi                                                   *
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
    *   Purpose : log functions for libgwr::Logger                              *
    *                                                                           *
    *****************************************************************************
*/

    private:
            gchar   a_str1  [4096];
            gchar   s2      [4096];

    public:
    inline   void
    log(guint32 _channel, guint32 _subchannel, const char* _format, ...)
    {
        va_list             val;
        Channel         *   c       = NULL;
        SubChannel      *   s       = NULL;
        //.........................................................................
        c   =   channel(_channel);
        if ( ! c )
        {
            printf("%s\n", "no channel");
        }
        g_return_if_fail(c);

        s   = c->subchannel(_subchannel);
        if ( ! s )
        {
            printf("%s\n", "no subchannel");
        }
        g_return_if_fail(s);

        va_start(val, _format);
        vsprintf(a_str1, _format, val);
        va_end(val);

        if ( c->fifo_buffered() )
            p0_blog     ( c, s, _subchannel, a_str1 );
        else
            p0_log      ( c, s, a_str1 );
    }
    //  ------------------------------------------------------------------------
    inline  void
    lognl(guint32 _channel, guint32 _subchannel, const char* _format, ...)
    {
        va_list             val;
        Channel         *   c       = NULL;
        SubChannel      *   s       = NULL;
        //.........................................................................
        c   =   channel(_channel);
        g_return_if_fail(c);

        s   = c->subchannel(_subchannel);
        g_return_if_fail(s);

        va_start(val, _format);
        vsprintf(a_str1, _format, val);
        va_end(val);

        if ( c->fifo_buffered() )
            p0_blognl   ( c, s, _subchannel, a_str1 );
        else
        {
            p0_lognl    ( c, s, a_str1 );
            c = NULL;
        }
    }
    //  ------------------------------------------------------------------------
    inline  void
    write(guint32 _channel, guint32 _subchannel, const char* _format, ...)
    {
        va_list             val;
        Channel         *   c       = NULL;
        SubChannel      *   s       = NULL;
        //.........................................................................
        c   =   channel(_channel);
        g_return_if_fail(c);

        s   = c->subchannel(_subchannel);
        g_return_if_fail(s);

        va_start(val, _format);
        vsprintf(a_str1, _format, val);
        va_end(val);

        if ( c->fifo_buffered() )
            p0_bwrite   ( c, s, _subchannel, a_str1 );
        else
            p0_write    ( c, s, a_str1 );
    }
    //  ------------------------------------------------------------------------
    inline  void
    write_html(guint32 _channel, guint32 _subchannel, const char* _format, ...)
    {
        va_list             val;
        Channel         *   c       = NULL;
        SubChannel      *   s       = NULL;
        //.........................................................................
        c   =   channel(_channel);
        g_return_if_fail(c);

        s   = c->subchannel(_subchannel);
        g_return_if_fail(s);

        va_start(val, _format);
        vsprintf(a_str1, _format, val);
        va_end(val);

        if ( c->fifo_buffered() )
        {
            //p0_bwrite_html   ( c, s, _subchannel, a_str1 );
        }
        else
        {
            p0_write_html    ( c, s, a_str1 );
        }
    }

