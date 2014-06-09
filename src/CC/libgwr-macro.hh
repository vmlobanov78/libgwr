/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-macro.hh                                                         *
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
    *   Purpose : Some macros + include all libgwr macro headers                *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWR_MACRO_HH__
#define __LIBGWR_MACRO_HH__

//  ***************************************************************************
//
//  Some macros
//
//  ***************************************************************************
#define GWR_LABEL           __attribute__ ((unused)) asm("nop");

#define g_free_safe(p)      if ( p ) g_free(p);


#define GWR_NAMESPACE_START(NAME)                                               \
namespace NAME                                                                  \
{

#define GWR_NAMESPACE_END(NAME)                                                 \
}

#define _gwr_virtual_

#include    "libgwr-macro-mvc.hh"
//  ----------------------------------------------------------------------------
#define LIBGWR_MEMBER_AUTO_TYPE(TYPE, NAME)                                     \
    private:                                                                    \
    TYPE                            a_##NAME;                                   \
    public:                                                                     \
            TYPE                    NAME()      { return a_##NAME;      }       \
            void                    set_##NAME( TYPE _p)                        \
                                    {                                           \
                                        a_##NAME = _p;                          \
                                    }
//  ----------------------------------------------------------------------------
#define LIBGWR_MEMBER_AUTO_OBJECT(CLASS, NAME)                                  \
    private:                                                                    \
    CLASS                           a_##NAME;                                   \
    public:                                                                     \
            inline CLASS     *      NAME()      { return &a_##NAME;     }
//  ----------------------------------------------------------------------------
#define LIBGWR_MEMBER_DYNAMIC(TYPE, NAME)                                       \
    private:                                                                    \
    TYPE                    *       d_##NAME;                                   \
    public:                                                                     \
            TYPE            *       NAME()      { return d_##NAME;      }       \
            void                    set_##NAME( TYPE* _t)                        \
                                    {                                           \
                                        d_##NAME = _t;                          \
                                    }
//  ----------------------------------------------------------------------------
#define LIBGWR_MEMBER_INTEGER(TYPE, NAME)                                       \
    private:                                                                    \
    TYPE                            a_##NAME;                                   \
    public:                                                                     \
            TYPE                    NAME()      { return a_##NAME;      }       \
            void                    set_##NAME( TYPE _i)                        \
                                    {                                           \
                                        a_##NAME = _i;                          \
                                    }

#define LIBGWR_MEMBER_U32(NAME)       LIBGWR_MEMBER_AUTO_TYPE(guint32, NAME);
#define LIBGWR_MEMBER_GBOOLEAN(NAME)  LIBGWR_MEMBER_AUTO_TYPE(gboolean, NAME);
//  ----------------------------------------------------------------------------
#define LIBGWR_MEMBER_STRING(NAME)                                              \
    protected:                                                                  \
    libgwr::String                a_##NAME;                                     \
    public:                                                                     \
    const   gchar               *   NAME()   { return a_##NAME.get();   }       \
            void                    set_##NAME( const gchar* _str)              \
                                    {                                           \
                                        a_##NAME.set( _str );                   \
                                    }                                           \
            libgwr::String    *   string_##NAME()                               \
                                    {                                           \
                                        return &a_##NAME;                       \
                                    }

#define LIBGWR_MEMBER_HSTRING(NAME)                                             \
    protected:                                                                  \
    libgwr::HString                 a_##NAME;                                   \
    public:                                                                     \
    const   gchar               *   NAME()      { return a_##NAME.s();   }      \
            GwrCStrHash             h##NAME()   { return a_##NAME.h();   }      \
            void                    set_##NAME( const gchar* _str)              \
                                    {                                           \
                                        a_##NAME.set( _str );                   \
                                    }

#endif                                                                          //  __LIBGWR_MACRO_HH__
