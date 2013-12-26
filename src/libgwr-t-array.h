/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-t-array.h                                                        *
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
    *   Class   : TArray                                                        *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_T_ARRAY_H__
#define     __LIBGWR_T_ARRAY_H__
//  ...........................................................................
GWR_NAMESPACE_START(libgwr)

/// ****************************************************************************
//!
//! \class  TArrayP
//!
//! \brief  Simple template <T> array that
//!         - store pointers on T in an array
//!         - realloc automatically
//!         ( GArray and GPtrArray doesnt fit my needs ).
//!
/// ****************************************************************************
template < typename T >
class TArrayP
{
    protected:
    guint32         a_tb;                                                       //!< T size in bytes
    guint32         a_sm;                                                       //!< Maximum size ( in T* unit ) the array may reach
    guint32         a_rs;                                                       //!< Realloc size ( in T* unit )

    T           **  d_array;
    guint32         a_cs;                                                       //!< Current size ( in T* unit ) of the array
    guint32         a_card;                                                     //!< Number of non-NULL T* in the array

    private:
    inline      gboolean        p0_reallocate();

    public:
    inline      guint32         p2_size_max         ()      { return a_sm; }
    inline      guint32         p2_realloc_size     ()      { return a_rs; }

    inline      void            p2_reset            ();
    inline      guint32         p2_card             ();
    inline      guint32         p2_size_current     ();

    inline      gboolean        p2_clr              (guint32 _ix);
    inline      gboolean        p2_set              (guint32 _ix, T* _t);
    inline      T           *   p2_get              (guint32 _ix);
    inline      gboolean        p2_add              (T* _t);

    inline      void            p2_dup()
                                {
                                    printf("--> TArray:size [%03u] - card [%03u]\n", a_cs, a_card);
                                }

    public:
    inline    TArrayP(guint32 _card_max, guint32 _realloc_size)
        : a_tb( sizeof( T* ) ), a_sm( _card_max ), a_rs( _realloc_size )
    {
        a_cs    = 0;
        a_card  = 0;
    }
    virtual ~TArrayP()
    {
        g_free( d_array );
    }
};
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
TArrayP< T >::p0_reallocate()
{
    T       *   t_new   = NULL;
    //  ........................................................................
    g_return_val_if_fail( a_cs + a_rs <= a_sm, FALSE );

    t_new   = static_cast < T* > ( g_malloc0( a_tb * ( a_cs + a_rs ) ) );

    g_memmove( (void*)t_new, (void*)d_array, a_tb * a_cs );

    a_cs = a_cs + a_rs;

    g_free_safe( d_array );                                                     // g_free_safe because of first reallocation

    d_array = (T**)t_new;

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          void
TArrayP< T >::p2_reset()
{
    g_free( d_array );
    a_cs    = 0;
    a_card  = 0;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          guint32
TArrayP< T >::p2_card()
{
    return a_card;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          guint32
TArrayP< T >::p2_size_current()
{
    return a_cs;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
TArrayP< T >::p2_clr(guint32 _ix)
{
    g_return_val_if_fail( _ix < a_cs, FALSE );

    d_array[ _ix ] = NULL;
    a_card--;

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
TArrayP< T >::p2_set(guint32 _ix, T* _t)
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
inline          T*
TArrayP< T >::p2_get(guint32 _ix)
{
    //g_return_val_if_fail( _ix < a_size, 0 );

    if ( _ix >= a_cs )
    {
        return NULL;
    }

    return d_array[ _ix ];
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
TArrayP< T >::p2_add(T* _t)
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














GWR_NAMESPACE_END(libgwr);

#endif
