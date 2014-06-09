/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-kv-array.hh                                                      *
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
    *   Class   : KVArray                                                       *
    *                                                                           *
    *   Purpose : Array of string couples ( key, value )                        *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_KV_ARRAY_HH__
#define     __LIBGWR_KV_ARRAY_HH__
//  ...........................................................................
GWR_NAMESPACE_START(libgwr)

/// ****************************************************************************
//!
//! \class  KVArray
//!
//! \brief  Array of string couples ( key, value )
//!
/// ****************************************************************************
class   KVArray
{
    private:
    libgwr::TArrayP < gchar >       a_keys;
    libgwr::TArrayP < gchar >       a_keys_ck;
    libgwr::TArrayP < gchar >       a_vals;

    public:
            guint32         card()  { return a_keys_ck.card();  }
            void            reset();

            gboolean        add(const gchar* _key, const gchar* _value);
    const   gchar       *   val(const gchar* _key);
    const   gchar       *   key(guint32 _ix);

    public:
             KVArray(guint32 _card_max, guint32 _realloc_size);
    virtual ~KVArray();
};

inline          KVArray::KVArray(guint32 _card_max, guint32 _realloc_size)
            :   a_keys      ( _card_max, _realloc_size )  ,
                a_keys_ck   ( _card_max, _realloc_size )  ,
                a_vals      ( _card_max, _realloc_size )
{

}
inline          KVArray::~KVArray()
{
}

inline  void
KVArray::reset()
{
    a_keys.reset();
    a_keys_ck.reset();
    a_vals.reset();
}

inline  gboolean
KVArray::add(const gchar* _key, const gchar* _value)
{
    gchar   *   ck  =   NULL;
    //  ........................................................................
    if ( val(_key) )
        return FALSE;

    a_keys.add      ( (gchar*)_key    );
    a_vals.add      ( (gchar*)_value  );

    ck  =   g_utf8_collate_key( _key, -1 );
    a_keys_ck.add   ( ck );
    delete ck;

    return TRUE;
}

inline
const gchar*    KVArray::key(guint32 _ix)
{
    return a_keys.get( _ix );
}

inline
const gchar*   KVArray::val(const gchar* _key)
{
    guint32         i       =   0;
    gchar       *   key     =   NULL;
    gchar       *   key_ck  =   NULL;
    gchar       *   ck_ref  =   NULL;
    //  ........................................................................
    ck_ref = g_utf8_collate_key( _key, -1 );

    for ( i = 0 ; i != a_keys.card() ; i++ )
    {
        key     =   a_keys.get(i);
        key_ck  =   a_keys_ck.get( i );

        if ( strcmp( ck_ref, key_ck ) == 0 )
        {
            g_free( ck_ref );
            return a_vals.get( i );
        }
    }

    g_free( ck_ref );
    return NULL;
}


GWR_NAMESPACE_END(libgwr)

#endif                                                                          //  __LIBGWR_KV_ARRAY_HH__
