/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-t-array.hh                                                       *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Copyright (C) 2011-2014 Guillaume Wardavoir                             *
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
    *   Class   : TArray                                                        *
    *                                                                           *
    *   Purpose : Simple array of pointers.                                     *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_T_ARRAY_HH__
#define     __LIBGWR_T_ARRAY_HH__
//  ...........................................................................
GWR_NAMESPACE_START(libgwr)

/// ****************************************************************************
//!
//! \class  TArray
//!
//! \brief  Simple template <T> array that
//!         - store Ts in an array
//!         - realloc automatically
//!         You can add, but not remove Ts.
//!
/// ****************************************************************************
template < typename T >
class TArray
{
    public:
    static          T           Null;

    protected:
    guint32         a_tb;                                                       //!< T size in bytes
    guint32         a_sm;                                                       //!< Maximum size ( in T unit ) the array may reach
    guint32         a_rs;                                                       //!< Realloc size ( in T unit )

    T            *  d_array;
    guint32         a_cs;                                                       //!< Current size ( in T unit ) of the array
    guint32         a_card;                                                     //!< Number of Ts added in the array


    private:
    inline      gboolean        p0_reallocate   ();
    inline      gboolean        p0_add          (T _t);

    public:
    inline      guint32         size_max        ()      { return a_sm; }
    inline      guint32         realloc_size    ()      { return a_rs; }

    inline      void            reset           ();
    inline      guint32         card            ();
    inline      guint32         size_current    ();

    inline      gboolean        set             (guint32 _ix, T _t);
    inline      gboolean        get             (guint32 _ix, T& __t);
    inline      T               get             (guint32 _ix);
    inline      T               qget            (guint32 _ix);
    inline      gboolean        add             (T _t);

    public:
    inline    TArray(guint32 _card_max, guint32 _realloc_size)
        :   a_tb( sizeof( T ) ) , a_sm( _card_max ) , a_rs( _realloc_size ) ,
            d_array( NULL )     ,
            a_cs( 0 )           , a_card( 0 )
    {
    }
    virtual ~TArray()
    {
        g_free_safe( d_array );
    }
};
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
TArray< T >::p0_reallocate()
{
    T       *   t_new   = NULL;
    //  ........................................................................
    g_return_val_if_fail( a_cs + a_rs <= a_sm, FALSE );

    t_new   = static_cast < T* > ( g_malloc0( a_tb * ( a_cs + a_rs ) ) );

    g_memmove( (void*)t_new, (void*)d_array, a_tb * a_cs );

    a_cs = a_cs + a_rs;

    g_free_safe( d_array );                                                     // g_free_safe because of first reallocation

    d_array = (T*)t_new;

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          void
TArray< T >::reset()
{
    g_free_safe( d_array );
    d_array = NULL;
    a_cs    = 0;
    a_card  = 0;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          guint32
TArray< T >::card()
{
    return a_card;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          guint32
TArray< T >::size_current()
{
    return a_cs;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
TArray< T >::set(guint32 _ix, T _t)
{
    g_return_val_if_fail( _ix < a_card, FALSE );

    d_array[ _ix ] = _t;
    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
TArray< T >::get(guint32 _ix, T& __t)
{
    g_return_val_if_fail( _ix < a_card, FALSE );

    __t = d_array[ _ix ];
    return TRUE;
}
template        < typename T >
inline          T
TArray< T >::get(guint32 _ix)
{
    g_return_val_if_fail(  _ix < a_card, TArray< T >::Null );

    return d_array[ _ix ];
}
template        < typename T >
inline          T
TArray< T >::qget(guint32 _ix)
{
    return d_array[ _ix ];
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
TArray< T >::p0_add(T _t)
{
    //..........................................................................
    if ( a_card < a_cs )
    {
        d_array[ a_card ] = _t;
        a_card++;
        return TRUE;
    }

    if ( ! p0_reallocate() )
        return FALSE;

    d_array[ a_card ] = _t;
    a_card++;
    return TRUE;
}
template        < typename T >
inline          gboolean
TArray< T >::add(T _t)
{
    return p0_add( _t );
}

GWR_NAMESPACE_END(libgwr);

#endif                                                                          //  __LIBGWR_T_ARRAY_HH__
