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
/// ****************************************************************************
//!
//! \class  StackGS
//!
//! \brief  Generic and Simple template stack class. Objects are stored in a
//!         gpointer, so their size must be smaller.
//!
/// ****************************************************************************
template < typename T >
class StackGS
{
protected:
    GSList          *   d_slist;
    guint32             a_card;
    guint32             a_sb;

public:
    inline      void        reset   ();

    inline      guint32     card    ();
    inline      void        push    (T   _t);
    inline      gboolean    pop     (T& __t);
    inline      gboolean    peek    (T& __t);

public:
    StackGS()
    {
        g_return_if_fail( sizeof(T) <= sizeof(gpointer) );

        a_sb    = sizeof(T);

        d_slist = NULL;
        a_card  = 0;
    }
    virtual ~StackGS()
    {
        g_slist_free( d_slist );
    }
};
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          void
StackGS< T >::reset()
{
    g_slist_free( d_slist );
    d_slist = NULL;
    a_card = 0;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          guint32
StackGS< T >::card()
{
    return a_card;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          void
StackGS< T >::push(T _t)
{
    gpointer    p = NULL;

    memcpy( &p, (gpointer)&_t, a_sb );                                          // workaroud gpointer <-> int casts

    d_slist = g_slist_prepend( d_slist, p );                                    // new first node
    a_card++;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
StackGS< T >::pop(T& __t)
{
    if ( ! a_card )
        return FALSE;

    memcpy( &__t, &d_slist->data, a_sb );                                       // workaroud gpointer <-> int casts

    d_slist = g_slist_delete_link( d_slist, d_slist );                          // remove first node

    a_card--;

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
StackGS< T >::peek(T& __t)
{
    if ( ! a_card )
        return FALSE;

    memcpy( &__t, &d_slist->data, a_sb );                                       // workaroud gpointer <-> int casts

    return TRUE;
}

/*
/// ****************************************************************************
//!
//! \class  PStack
//!
//! \brief  Simple template stack class. The reset method free all elements in
//!     the stack.
//!
/// ****************************************************************************
template < typename T >
class PStack : public libgwr::Stack< T >                                        //  _GWR_TODO_ : use GPtrArray
{
    public:
    inline      void        reset   (eFreeMode);
};
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

lab_reset:
    Stack< T >::reset();
}
enum eFreeMode
{
    e_free_none     = 0,
    e_free_g_free   = 1,
    e_free_delete   = 2
};


*/











GWR_NAMESPACE_END(libgwr);

#endif
