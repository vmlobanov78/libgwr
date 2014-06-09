/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-logger-log.hi                                                    *
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

//  ............................................................................
#include    "libgwr.hh"

//  ............................................................................
GWR_NAMESPACE_START(libgwr)

//  ----------------------------------------------------------------------------
void
Logger::p0_log(Channel* c, SubChannel* s, const char* _text)
{
    GwrTextView     *   gtv     = NULL;
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
                gtv->append     (s2, libgwr::TextAttributes().flags());
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
Logger::p0_lognl(Channel* c, SubChannel* s, const char* _text)
{
    GwrTextView     *   gtv     = NULL;
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
                gtv->append     (s2, libgwr::TextAttributes().flags());
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
Logger::p0_write(Channel* c, SubChannel* s, const char* _text)
{
    GwrTextView     *   gtv     = NULL;
    //  ........................................................................
    //  console
    if ( s->enabled_console() )
    {
        printf("%s", _text);
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


GWR_NAMESPACE_END(libgwr)

