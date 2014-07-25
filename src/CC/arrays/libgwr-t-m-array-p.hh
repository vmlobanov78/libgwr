/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-t-m-array-p.hh                                                   *
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
    *   Class   : TMArrayP                                                      *
    *                                                                           *
    *   Purpose : Array                                                         *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_T_M_ARRAY_P_HH__
#define     __LIBGWR_T_M_ARRAY_P_HH__
//  ...........................................................................
GWR_NAMESPACE_START(libgwr)

/// ****************************************************************************
//!
//! \class  TMArrayP
//!
//! \brief  Simple template array that
//!         - store pointers on T in an array
//!         - realloc automatically
//!         - move pointers down and up at deletion & insertion, like GArray
//!         T must be on of guint16, guint32, guint64
//!         ( GPtrArray doesnt have insert method ! )
//!
/// ****************************************************************************
template < typename T, typename S >
class TMArrayP
{
    public:
    static  guint32         s_b;                                                //!< T* size in bytes
            S               a_sm;                                               //!< Maximum size ( in T* unit ) the array may reach
            S               a_rs;                                               //!< Realloc size ( in T* unit )

            T           **  d_array;                                            //!< Array
            S               a_cs;                                               //!< Current size ( in T* unit ) of the array
            S               a_cc;                                               //!< Current Card = Index of last T* pointer
    //  ------------------------------------------------------------------------
    //  standard methods
    //  ------------------------------------------------------------------------
    private:
    inline      gboolean        p0_reallocate   ();
    inline      gboolean        p0_move_up      (S _ix);
    inline      gboolean        p0_move_down    (S _ix);
    inline      void            p0_set          (S _ix, T* _t);

    public:
    inline      guint32         size_max        ()      { return a_sm; }
    inline      guint32         realloc_size    ()      { return a_rs; }

    inline      void            reset           ();
    inline      S               size            ()      { return a_cs;  }
    inline      S               card            ()      { return a_cc;  }
    inline      S               pos             (T* _t);
    //  ------------------------------------------------------------------------
    //  multimode methods
    //  ------------------------------------------------------------------------
    //  boolean version
    inline      gboolean        bins            (S _ix, T* _t   );
    inline      gboolean        badd            (       T* _t   );
    inline      gboolean        bdel            (S _ix          );
    inline      gboolean        bget            (S _ix, T*& __t );
    inline      gboolean        bset            (S _ix, T* _t   );
    //  ------------------------------------------------------------------------
    //  normal version
    inline      void            ins             (S _ix, T* _t   );
    inline      void            add             (       T* _t   );
    inline      void            del             (S _ix          );
    inline      T*              get             (S _ix          );
    inline      void            set             (S _ix, T* _t   );
    //  ------------------------------------------------------------------------
    //  quick version
    inline      void            qins            (S _ix, T* _t   );
    inline      void            qadd            (       T* _t   );
    inline      void            qdel            (S _ix          );
    inline      T*              qget            (S _ix          );
    inline      void            qset            (S _ix, T* _t   );
    //  ------------------------------------------------------------------------
    /*
    inline      void            dup()
                                {
                                    printf("--> TArray:size [%03u] - card [%03u]\n", a_cs, a_card);
                                }
    */
    //  ------------------------------------------------------------------------
    public:
    inline    TMArrayP(S _card_max, S _realloc_size)
        : a_sm( _card_max ), a_rs( _realloc_size ), d_array(NULL), a_cs(0), a_cc(0)
    {
    }
    virtual ~TMArrayP()
    {
        g_free( d_array );
    }
};
//  ****************************************************************************
//  standard methods
//  ****************************************************************************
template        < typename T , typename S >
inline          gboolean
TMArrayP< T, S >::p0_reallocate()
{
    void    *   t_new   = NULL;
    //  ........................................................................
    g_return_val_if_fail( a_cs + a_rs <= a_sm, FALSE );

    t_new   = g_malloc0( s_b * ( a_cs + a_rs ) );

    g_memmove( t_new, (void*)d_array, s_b * a_cs );

    a_cs = a_cs + a_rs;

    g_free_safe( d_array );                                                     // g_free_safe because of first reallocation

    d_array = static_cast < T ** > ( t_new );

    return TRUE;
}
template        < typename T , typename S >
inline          gboolean
TMArrayP< T, S >::p0_move_up(S _ix)
{
    //  ........................................................................
    g_return_val_if_fail( _ix < card(), FALSE );
    //  ........................................................................
    //  dont reallocate ( we have at least one place, and we want just to
    //  move a part of tha array one place up )
    if ( card() != size() )
    {
        g_memmove(
            &( d_array[_ix ] ) + 1      ,                                       //  "+" is understood in T*, +1 will add sizeof(T*) bytes
            &( d_array[_ix ] )          ,
            ( card() - _ix ) * s_b      );
        return TRUE;
    }
    //  ........................................................................
    //  reallocate and retry
    g_return_val_if_fail( p0_reallocate(), FALSE );

    return p0_move_up( _ix );
}
template        < typename T , typename S >
inline          gboolean
TMArrayP< T, S >::p0_move_down(S _ix)
{
    //  ........................................................................
    g_return_val_if_fail( _ix < card(), FALSE );
    //  ........................................................................
    //  we have something to move
    if ( ( _ix + 1 ) != card() )
    {
        g_memmove(
            &( d_array[_ix ] )          ,
            &( d_array[_ix ] ) + 1      ,                                       //  "+" is understood in T*, +1 will add sizeof(T*) bytes
            ( card() - _ix - 1 ) * s_b  );
        return TRUE;
    }

    //  ........................................................................
    //  we have nothing to move, but set the pointer that should have been
    //  overwritten to NULL
    d_array[_ix] = NULL;

    return TRUE;
}
template        < typename T, typename S >
inline          void
TMArrayP< T, S >::p0_set(S _ix, T* _t)
{
    d_array[ _ix ] = _t;
}
template        < typename T, typename S >
inline          S
TMArrayP< T, S >::pos(T* _t)
{
    return ( _t - (T*)d_array ) & 0x000000000000ffff;
}

/*
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          void
TMArrayP< T >::reset()
{
    g_free( d_array );
    d_array = NULL;
    a_cs    = 0;
    a_card  = 0;
}
*/
//  ****************************************************************************
//  multimode methods : normal version
//  ****************************************************************************
template        < typename T, typename S >
inline          void
TMArrayP< T, S >::ins(S _ix, T* _t)
{
    g_return_if_fail( _ix < card() );

    g_return_if_fail( p0_move_up( _ix ) );

    p0_set( _ix, _t );
    a_cc++;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T, typename S >
inline          void
TMArrayP< T, S >::add(T* _t)
{
    //..........................................................................
    //  dont reallocate
    if ( size() != card() )
    {
        p0_set( card(), _t );
        a_cc++;
        return;
    }
    //..........................................................................
    //  reallocate and retry
    g_return_if_fail( p0_reallocate() );

    add( _t );
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T, typename S >
inline          void
TMArrayP< T, S >::del(S _ix)
{
    g_return_if_fail( _ix < card() );

    g_return_if_fail( p0_move_down( _ix ) );

    a_cc--;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T, typename S >
inline          T*
TMArrayP< T, S >::get(S _ix)
{
    g_return_val_if_fail( _ix < card(), NULL );

    return d_array[ _ix ];
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T, typename S >
inline          void
TMArrayP< T, S >::set(S _ix, T* _t)
{
    g_return_if_fail( _ix < card() );

    d_array[ _ix ] = _t;
}



template < typename T, typename S > guint32 libgwr::TMArrayP<  T, S >::s_b = sizeof(T*);






GWR_NAMESPACE_END(libgwr)

#endif                                                                          //  __LIBGWR_T_M_ARRAY_P_HH__
