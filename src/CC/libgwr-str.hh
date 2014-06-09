/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-str.hh                                                           *
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
    *   Class   : String, HString                                               *
    *                                                                           *
    *   Purpose : Strings                                                       *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWR_STR_HH__
#define __LIBGWR_STR_HH__

#include    "libgwr.hh"

GWR_NAMESPACE_START(libgwr)

//  ............................................................................
GWR_NAMESPACE_START(chr)

extern  gboolean    is_dec_digit(gchar c);
extern  gboolean    is_hex_digit(gchar c);

extern  gboolean    ascii_dec_to_u32(gchar _c, guint32* _u32);
extern  gboolean    ascii_dec_to_i32(gchar _c, gint32*  _i32);

extern  gboolean    ascii_hex_to_u32(gchar _c, guint32* _u32);
extern  gboolean    ascii_hex_to_i32(gchar _c, gint32*  _i32);

extern  gboolean    ascii_dec_to_u64(gchar _c, guint64* _u64);
extern  gboolean    ascii_dec_to_i64(gchar _c, gint64*  _i64);

extern  gboolean    ascii_hex_to_u64(gchar _c, guint64* _u64);
extern  gboolean    ascii_hex_to_i64(gchar _c, gint64*  _i64);

GWR_NAMESPACE_END(chr)
//  ............................................................................
GWR_NAMESPACE_START(str)

inline  gboolean        eq(const gchar* _a, const char* _b)
{
    return ( ! strcmp(_a,_b) );
}

extern  gchar       *   dequote(const gchar* _str);

extern  gboolean        ascii_dec_to_u32(const gchar* _str, guint32* _i32);
extern  gboolean        ascii_dec_to_i32(const gchar* _str, gint32*  _i32);

extern  gboolean        ascii_hex_to_u32(const gchar* _str, guint32* _i32);
extern  gboolean        ascii_hex_to_i32(const gchar* _str, gint32*  _i32);

extern  gboolean        ascii_dec_to_u64(const gchar* _str, guint64* _i64);
extern  gboolean        ascii_dec_to_i64(const gchar* _str, gint64*  _i64);

extern  gboolean        ascii_hex_to_u64(const gchar* _str, guint64* _i64);
extern  gboolean        ascii_hex_to_i64(const gchar* _str, gint64*  _i64);

extern  libgwr::TArrayP < gchar >   *   split(const gchar*, gchar);

GWR_NAMESPACE_END(str)

GWR_NAMESPACE_START(dstr)

extern  void            clr     ( gchar*& _dyn_str );
extern  void            del     ( gchar*& _dyn_str );
extern  void           sdel     ( gchar*& _dyn_str );
extern  void            rst     ( gchar*& _dyn_str );

extern  void            set     ( gchar*& _dyn_str, const gchar* _s );
extern  void           vset     ( gchar*& _dyn_str, ...             )           __attribute__ ((sentinel));
extern  void            append  ( gchar*& _dyn_str, ...             )           __attribute__ ((sentinel));

GWR_NAMESPACE_END(dstr)

GWR_NAMESPACE_START(env)
GWR_NAMESPACE_START(str)

extern  const   gchar                       *   replace_shell_variables(const gchar*  _str);
extern  const   libgwr::TArrayP < gchar >   *   rsv_vars_replaced();
extern  const   libgwr::TArrayP < gchar >   *   rsv_vars_not_found();

GWR_NAMESPACE_END(str)
GWR_NAMESPACE_END(env)

class   String
{
    protected:
    gchar                   *   d_str;

    private:
            void                p0_clr();
            void                p0_del();

    public:
            void                del();
            void                rst();
            void                rep(gchar* _dyn_str);

    const   gchar           *   get();

            void                set     (const gchar* _str);
            void                append  (const gchar* _str_1, ...)              __attribute__ ((sentinel));
    virtual void                vset    (const gchar* _str_1, ...)              __attribute__ ((sentinel));

    public:
    operator const gchar*   () const
    {
        return (const gchar*)d_str;
    }

    public:
             String();
             String(const gchar* _str);
    virtual ~String();
};

class   HString : public String
{
    private:
    GwrCStrHash                 a_h;

    public:
    void                        set(const gchar* _str);

    public:
    virtual void                vset(const gchar* _str_1, ...)                  __attribute__ ((sentinel)) ;

    const   gchar           *   s()     { return d_str; }                       //  old method, use cast operator
            GwrCStrHash         h()     { return a_h;   }

    public:
             HString();
             HString(const gchar* _str);
    virtual ~HString();
};



GWR_NAMESPACE_END(libgwr)

#endif                                                                          //  __LIBGWR_STR_HH__
