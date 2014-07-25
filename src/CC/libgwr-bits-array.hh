/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-bits-array.hh                                                    *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Array of bits                                                           *
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
    *   Class   : BitsArray                                                     *
    *                                                                           *
    *   Purpose : Simple array of bits.                                         *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_BITS_ARRAY_HH__
#define     __LIBGWR_BITS_ARRAY_HH__
//  ...........................................................................
GWR_NAMESPACE_START(libgwr)

/// ****************************************************************************
//!
//! \class  BitsArray
//!
//! \brief  Simple Array of bits.
//!
/// ****************************************************************************
class BitsArray
{
    protected:
    guint32         a_sm;                                                       //!< Maximum size ( in bits  ) the array may reach
    guint32         a_rs;                                                       //!< Realloc size ( in bytes )

    gchar        *  d_array;
    guint32         a_bits;                                                     //!< Current size ( in bits  ) of the array
    guint32         a_bytes;                                                    //!< Current size ( in bytes ) of the array


    private:
    inline      gboolean        p0_reallocate   ();
    inline      gboolean        p0_add          (T _t);

    public:
    inline      guint32         size_max        ()      { return a_sm; }
    inline      guint32         realloc_size    ()      { return a_rs; }

    inline      void            reset           ();
    inline      guint32         card            ();
    inline      guint32         size_current    ();

    inline      gboolean        clr             (guint32 _ix);
    inline      gboolean        set             (guint32 _ix);
    inline      gboolean        get             (guint32 _ix);

    public:
    inline    BitsArray(guint32 _card_max_in_bits, guint32 _realloc_size_in_bytes)
        : a_sm( _card_max_in_bits ), a_rs( _realloc_size_in_bytes )
    {
        a_bits  =   0;
        a_bytes =   0;
    }
    virtual ~BitsArray()
    {
        g_free( d_array );
    }
};
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline          gboolean
BitsArray::p0_reallocate()
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
inline          void
BitsArray::reset()
{
    g_free( d_array );
    d_array = NULL;
    a_cs    = 0;
    a_card  = 0;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline          guint32
BitsArray::size_bits()
{
    return a_bits;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline          guint32
BitsArray::size_bytes()
{
    return a_bytes;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline          void
BitsArray::clr(guint32 _ix)
{
    guint32     q   =   _ix / 8;
    guint32     r   =   _ix % 8;
    //  ........................................................................
    g_return_val_if_fail( _ix < a_sm, FALSE );

    d_array[ q ]    &=  ~( 1 << r );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
inline          void
BitsArray::set(guint32 _ix)
{
    g_return_val_if_fail( _t          , FALSE );

    while ( _ix >= a_cs )
    {
        if ( ! p0_reallocate() )
            return FALSE;
    }

    if ( ! d_array[ _ix ] )
        a_card++;

    d_array[ _ix ] = _t;

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
TArray< T >::get(guint32 _ix, T& __t)
{
    if ( _ix >= a_cs )
    {
        __t = NULL;
        return FALSE;
    }

    __t = d_array[ _ix ];
    return TRUE;
}
template        < typename T >
inline          T
TArray< T >::get(guint32 _ix)
{
    g_return_val_if_fail(  _ix < a_cs, TArray< T >::Null );

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
    guint32     i   = 0;
    guint32     j   = 0;
    //..........................................................................
    for ( i = 0 ; i != a_cs ; i++ )
    {
        if ( d_array[i] == NULL )
        {
            d_array[i] = _t;
            a_card++;
            return TRUE;
        }
    }

    j = a_cs;

    if ( ! p0_reallocate() )
        return FALSE;

    d_array[ j ] = _t;
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
