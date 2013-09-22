/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-object.h                                                         *
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
    *   Class   : Object                                                        *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_OBJECT_H__
#define     __LIBGWR_OBJECT_H__
//  ...........................................................................
namespace libgwr
{
//  ***************************************************************************
//
//							Object
//
//  ***************************************************************************
//! \class  Object
//! \brief  This template automaticaly add new / delete to another class.
//!
//! There are many Instance<> template-constructors, for handling
//! the number of constructor parameters of any "T" ( I tried va_args but
//! didnt succeed ).
template <typename T> class Object : public T
{
    public:

    void*		operator new(size_t size)
                {
                    Object<T>* g = g_try_new0(Object<T>, 1);

                    if ( !g )
                        g_critical("Instance<%s>::new():g_try_new0 failed", GWR_STRINGIZE(T));

                    return g;
                }

    void		operator delete (void *p)
                {
                    g_free(p);
                }

    T   *       cast()  { return static_cast<T*>(this); }

    Object() : T() {}
    template <typename U1> Object(U1 u1) : T(u1) {}
    template <typename U1, typename U2> Object(U1 u1, U2 u2) : T(u1, u2) {}
    template <typename U1, typename U2, typename U3> Object(U1 u1, U2 u2, U3 u3) : T(u1, u2, u3) {}
    template <typename U1, typename U2, typename U3, typename U4> Object(U1 u1, U2 u2, U3 u3, U4 u4) : T(u1, u2, u3, u4) {}
    template <typename U1, typename U2, typename U3, typename U4, typename U5> Object(U1 u1, U2 u2, U3 u3, U4 u4, U5 u5) : T(u1, u2, u3, u4, u5) {}
    template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6> Object(U1 u1, U2 u2, U3 u3, U4 u4, U5 u5, U6 u6) : T(u1, u2, u3, u4, u5, u6) {}
    template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6, typename U7> Object(U1 u1, U2 u2, U3 u3, U4 u4, U5 u5, U6 u6, U7 u7) : T(u1, u2, u3, u4, u5, u6, u7) {}
    template <typename U1, typename U2, typename U3, typename U4, typename U5, typename U6, typename U7, typename U8> Object(U1 u1, U2 u2, U3 u3, U4 u4, U5 u5, U6 u6, U7 u7, U8 u8) : T(u1, u2, u3, u4, u5, u6, u7, u8) {}

    virtual     ~Object<T>() {}
};

//
//  The Object<> struct comes with a convenience macro for
//  creating new instances of GStruct<> - and its free of charge.
//
#define GWR_NEW_CAST( type, ... )   ( static_cast<type*>  ( new libgwr::Object<type>(__VA_ARGS__) )     )
#define GWR_NEW( type, ... )        (                       new libgwr::Object<type>(__VA_ARGS__)       )









}   // libgwr

#endif
