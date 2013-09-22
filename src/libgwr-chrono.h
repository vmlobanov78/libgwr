/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-chrono.h                                                         *
    *                                                                           *
    *   --------------------------------------------------------------------    *
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
    *   Class   : Object                                                        *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_CHRONO_H__
#define     __LIBGWR_CHRONO_H__
//  ...........................................................................
namespace libgwr
{
//  ***************************************************************************
//
//							Chrono
//
//  ***************************************************************************
#define LIBGWR_CHRONO( PC, DO_SOMETHING )                                       \
    (PC)->start();                                                              \
    DO_SOMETHING;                                                               \
    (PC)->stop();

//! \class  Chrono
//! \brief  For measuring time
class Chrono
{
    private:
    GTimeVal a_gtv_start;
    GTimeVal a_gtv_stop;

    guint32     a_es;
    guint32     a_ems;
    glong       a_eus;

    public:
    void    start()
    {
        g_get_current_time( &a_gtv_start );
    }
    void stop()
    {
        g_get_current_time( &a_gtv_stop );

        a_es = (guint32)( a_gtv_stop.tv_sec - a_gtv_start.tv_sec );

        if ( a_es == 0 )
        {
            a_eus   = a_gtv_stop.tv_usec - a_gtv_start.tv_usec;
            a_ems   = a_eus / (guint32)1000;
            return;
        }

        //  a_es >= 1
        a_eus   = (glong)1000000 - a_gtv_start.tv_usec + a_gtv_stop.tv_usec;
        a_ems   = a_eus / (guint32)1000;

        a_es    = a_es - 1;
        a_es    = a_es + a_ems / (guint32)1000;
        a_ems   = a_ems % (guint32)1000;
    }

    guint32 es()        { return a_es;  }
    guint32 ems()       { return a_ems; }
    //glong   eus()       { return a_eus; }

    public:
             Chrono()   {}
    virtual ~Chrono()   {}
};










}   // libgwr

#endif
