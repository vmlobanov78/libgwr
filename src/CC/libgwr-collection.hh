/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-collection.hh                                                    *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   Part of libwgwr                                                         *
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
    *   Class   : Collection < T >                                              *
    *                                                                           *
    *   Purpose :                                                               *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_COLLECTION_HH__
#define     __LIBGWR_COLLECTION_HH__
//  ...........................................................................
GWR_NAMESPACE_START(libgwr)

//! \class  Collection
//! \brief  For Collectionning elements of a certain type T, 32 elements maximum.
//!     For each element, associate a name and a flag. Flags are contained in a
//!     guint32. Then it is possible to do some operations using only the name,
//!     which is more lisible in source code.
template < typename T >
class Collection32
{
    //  ........................................................................
    template < typename U >
    class   Element
    {
        friend class Collection32;
        //  ....................................................................
    private:
        gchar   *   d_nam;
        T           _val;
        guint32     a_flg;
    };
    //  ........................................................................
private:
    Element < T >   a_array [ 32 ];                                             //!< 32 objects maximum
    guint32         a_next_free_index;
    //  ........................................................................
public:
    Collection32()
        : a_next_free_index(0)
    {
        for ( guint32 i = 0 ; i != 32 ; i++ )
        {
            a_array[ i ].d_nam = NULL;
            a_array[ i ].a_flg = 0;
        }
    }
    inline virtual ~Collection32()
    {
        for ( guint32 i = 0 ; i != 32 ; i++ )
            g_free_safe( a_array[ i ].d_nam );
    }
    //  ........................................................................
private:
    inline gboolean     _get_index_from_name        (const gchar* _name, guint32& __index);
    inline gboolean     _get_index_from_power_of_2  (guint32 _power);
    inline gboolean     _get_power_of_2_from_index  (guint32& __result, guint32 _index);
    //  ........................................................................
public:
    //! Get the cardinal of elements in the collection
    inline guint32      card                ();
    //! Get an element's value from the element's index
    inline GList    *   values_from_flags   (guint32 _flags);

public:
    //! Add an element to the collection
    inline gboolean     add         (const gchar* _name, T _value);
    //! Given a name, test if the corresponding T value is set in flags
    inline gboolean     flag_test   (const gchar* _name, guint32  _flags);
    //! Given a name, set corresponding flag. Clear other flags.
    inline gboolean     flag_set    (const gchar* _name, guint32& __flags);
    //! Given a name, set corresponding flag. Do not clear other flags.
    inline gboolean     flag_add    (const gchar* _name, guint32& __flags);
    //! Given a name, clear corresponding flag. Do not clear other flags.
    inline gboolean     flag_clr    (const gchar* _name, guint32& __flags);
};
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        <>
inline
Collection32< const gchar* >::~Collection32()
{
    for ( guint32 i = 0 ; i != 32 ; i++ )
    {
        g_free_safe( a_array[ i ].d_nam );
        g_free_safe( const_cast< gchar* >( a_array[ i ]._val ) );
    }
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
Collection32< T >::_get_index_from_name(const gchar* _name, guint32& __index)
{
    g_return_val_if_fail( _name, FALSE );
    //  ........................................................................
    for ( guint32 i = 0 ; i != a_next_free_index ; i++ )
        if ( ! strcmp( a_array[ i ].d_nam, _name ) )                            //  match
        {
            __index = i;
            return TRUE;
        }
    return FALSE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
Collection32< T >::_get_power_of_2_from_index(guint32& __power, guint32 _index)
{
    __power = 0;

    g_return_val_if_fail( _index <= 31, FALSE );
    //  ........................................................................
    __power = _index ? 1 << _index : 1;

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          guint32
Collection32< T >::card()
{
    return a_next_free_index;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          GList*
Collection32< T >::values_from_flags(guint32 _flags)
{
    GList   *   list    = NULL;
    guint32     flag    = 0;
    //  ........................................................................
    for ( guint32 i = 0 ; i <= 31 ; i++ )
    {
        _get_power_of_2_from_index(flag, i);
        if ( _flags & flag )
        {
            list = g_list_append( list, (gpointer)( &( a_array[i]._val ) ) );
        }
    }

    return list;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template        < typename T >
inline          gboolean
Collection32< T >::add(const gchar* _name, T _value)
{
    Element < T >   *   p   = NULL;
    //  ........................................................................
    g_return_val_if_fail( _name, FALSE );
    if ( a_next_free_index == 32 ) return FALSE;
    //  ........................................................................
    p = &( a_array[a_next_free_index] );

    p->d_nam    = g_strdup( _name );
    p->_val     = _value;
    p->a_flg    = a_next_free_index ? 1 << a_next_free_index : 1;

    a_next_free_index++;

    return TRUE;
}
//  ............................................................................
template        <>
inline          gboolean
Collection32< const gchar* >::add(const gchar* _name, const gchar* _value_str)
{
    Element < const gchar* >   *   p   = NULL;
    //  ........................................................................
    g_return_val_if_fail( _name, FALSE );
    if ( a_next_free_index == 32 ) return FALSE;
    //  ........................................................................
    p = &( a_array[a_next_free_index] );

    p->d_nam    = g_strdup( _name );
    p->_val     = g_strdup( _value_str );
    p->a_flg    = a_next_free_index ? 1 << a_next_free_index : 1;

    a_next_free_index++;

    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template    < typename T >
inline      gboolean
Collection32 < T >::flag_test(const gchar* _name, guint32 _flags)
{
    guint32 index = 0;
    //  ........................................................................
    g_return_val_if_fail( _name                                 , FALSE );
    g_return_val_if_fail( _get_index_from_name(_name, index)    , FALSE );
    //  ........................................................................
    return a_array[ index ].a_flg & _flags     ?
           TRUE                     :
           FALSE                    ;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template    < typename T >
inline      gboolean
Collection32 < T >::flag_set(const gchar* _name, guint32& __flags)
{
    guint32 index = 0;
    //  ........................................................................
    g_return_val_if_fail( _name                                 , FALSE );
    g_return_val_if_fail( _get_index_from_name(_name, index)    , FALSE );
    //  ........................................................................
    __flags = a_array[ index ].a_flg;
    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template    < typename T >
inline      gboolean
Collection32 < T >::flag_add(const gchar* _name, guint32& __flags)
{
    guint32 index = 0;
    //  ........................................................................
    g_return_val_if_fail( _name                                 , FALSE );
    g_return_val_if_fail( _get_index_from_name(_name, index)    , FALSE );
    //  ........................................................................
    __flags |= a_array[ index ].a_flg;
    return TRUE;
}
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template    < typename T >
inline      gboolean
Collection32 < T >::flag_clr(const gchar* _name, guint32& __flags)
{
    guint32 index = 0;
    //  ........................................................................
    g_return_val_if_fail( _name                                 , FALSE );
    g_return_val_if_fail( _get_index_from_name(_name, index)    , FALSE );
    //  ........................................................................
    __flags &= ( ~ a_array[ index ].a_flg );
    return TRUE;
}








GWR_NAMESPACE_END(libgwr)

#endif                                                                          //  __LIBGWR_COLLECTION_HH__
