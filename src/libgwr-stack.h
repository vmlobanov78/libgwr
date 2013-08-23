/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-stack.h                                                          *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Stack implementation                                                    *
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
    *   Class   : Stack                                                         *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_STACK_H__
#define     __LIBGWR_STACK_H__
//  ...........................................................................
GWR_NAMESPACE_START(libgwr)

enum eFreeMode
{
    e_free_none     = 0,
    e_free_g_free   = 1,
    e_free_delete   = 2
};

//! \class  Stack
//!
//! \brief  Simple template stack class.
template < typename T >
class Stack
{
    protected:
    GArray      *   d_array;
    guint32         a_card;

    public:
    inline      void        reset   ();

    inline      guint32     card    ();
    inline      void        push    (T _t);
    inline      gboolean    pop     (T& __t);
    inline      gboolean    peek    (T& __t);

    public:
    inline    Stack()
    {
        d_array = g_array_new( FALSE, FALSE, sizeof(T) );
        a_card  = 0;
    }
    virtual ~Stack()
    {
        g_array_free( d_array, TRUE );
    }
};
//! \class  PStack
//!
//! \brief  Simple template stack class. The reset method free all elements in
//!     the stack.
template < typename T >
class PStack : public libgwr::Stack< T >                                        //  _GWR_TODO_ : use GPtrArray
{
    public:
    inline      void        reset   (eFreeMode);
};
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          void
Stack< T >::reset()
{
    if ( ! a_card )
        return;

    g_array_remove_range( d_array, 0, a_card );
    a_card = 0;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          void
PStack< T >::reset(eFreeMode _fm)
{
    guint32 i = 0;
    //  ........................................................................
    if ( _fm == e_free_none )
        goto lab_reset;

    if ( _fm == e_free_g_free )
        for ( guint32 i = 0 ; i != this->a_card ; i ++ )                        //  _GWR_GCC_BUG_ 'this->' is mandatory !
            g_free( (void*)g_array_index( this->d_array, T, i ) );

    if ( _fm == e_free_delete )
        for ( guint32 i = 0 ; i != this->a_card ; i ++ )
            delete g_array_index( this->d_array, T, i );

    Stack< T >::reset();
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          guint32
Stack< T >::card()
{
    return a_card;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          void
Stack< T >::push(T _t)
{
    g_array_append_val( d_array, _t );
    a_card++;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
Stack< T >::pop(T& __t)
{
    if ( ! a_card )
        return FALSE;

    __t = g_array_index( d_array, T, ( a_card - 1 ) );

    g_array_remove_index( d_array, --a_card );

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
Stack< T >::peek(T& __t)
{
    if ( ! a_card )
        return FALSE;

    __t = g_array_index( d_array, T, ( a_card - 1 ) );

    return TRUE;
}














GWR_NAMESPACE_END(libgwr);

#endif
