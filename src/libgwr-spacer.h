/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-spacer.h                                                         *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Gnome Universal Logger Library                                          *
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
    *   Class   : None                                                          *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWR_SPACER_H__
#define __LIBGWR_SPACER_H__

namespace libgwr
{

class   Spacer
{
    private:
    gchar   *   d_spaces;
    gint        a_size;
    gchar   *   a_pointer;
    gint        a_step;

    public:
    Spacer(gint _n, gint _step)
    {
        a_size      =   _n;
        a_step      =   _step;

        d_spaces    =   (gchar*)g_malloc0(1 + a_size);
        a_pointer   =   d_spaces + a_size;

        for ( gint i = 0 ; i < a_size ; i++ )
            d_spaces[i] = ' ';
        d_spaces[a_size] = 0;
    }
    virtual ~Spacer()
    {
        g_free( d_spaces );
    }

    public:
    inline  void    inc()
    {
        if ( (a_pointer - a_step) >= d_spaces )
            a_pointer -= a_step;
    }
    inline  void    dec()
    {
        if ( (a_pointer + a_step) <= (d_spaces + a_size) )
            a_pointer += a_step;
    }
    inline  void    reset()
    {
        a_pointer   =   d_spaces + a_size;
    }

    inline  const gchar*    get()
    {
        return a_pointer;
    }
};

}   //  namespace libgwr

#endif //__LIBGWR_SPACER_H__
