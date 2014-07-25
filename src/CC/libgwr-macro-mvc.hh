/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-macro-mvc.hh                                                     *
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
    *   Class   :                                                               *
    *                                                                           *
    *   Purpose : Macros for MVC programming                                    *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWR_MACRO_MVC_HH__
#define __LIBGWR_MACRO_MVC_HH__


//  ----------------------------------------------------------------------------
#define LIBGWR_MEMBER_MVC_AMODEL(TYPE)                                          \
    private:                                                                    \
    TYPE                    *       a_model;                                    \
    public:                                                                     \
            TYPE            *       m()      { return a_model;  }               \
            void                    set_model( TYPE* _m)                        \
                                    {                                           \
                                        a_model = _m;                           \
                                    }
//  ----------------------------------------------------------------------------
#define LIBGWR_MEMBER_MVC_AVIEW(TYPE)                                           \
    private:                                                                    \
    TYPE                    *       a_view;                                     \
    public:                                                                     \
            TYPE            *       v()      { return a_view;  }                \
            void                    set_view( TYPE* _v)                         \
                                    {                                           \
                                        a_view = _v;                            \
                                    }
//  ----------------------------------------------------------------------------
#define LIBGWR_MEMBER_MVC_ACONTROL(TYPE)                                        \
    private:                                                                    \
    TYPE                    *       a_control;                                  \
    public:                                                                     \
            TYPE            *       c()      { return a_control;  }             \
            void                    set_control( TYPE* _c)                      \
                                    {                                           \
                                        a_control = _c;                         \
                                    }
//  ============================================================================
#define LIBGWR_MEMBER_MVC_DMODEL(TYPE)                                          \
    private:                                                                    \
    TYPE                    *       d_model;                                    \
    public:                                                                     \
            TYPE            *       m()      { return d_model;  }               \
            void                    set_model( TYPE* _m)                        \
                                    {                                           \
                                        d_model = _m;                           \
                                    }
//  ----------------------------------------------------------------------------
#define LIBGWR_MEMBER_MVC_DVIEW(TYPE)                                           \
    private:                                                                    \
    TYPE                    *       d_view;                                     \
    public:                                                                     \
            TYPE            *       v()      { return d_view;  }                \
            void                    set_view( TYPE* _v)                         \
                                    {                                           \
                                        d_view = _v;                            \
                                    }
//  ----------------------------------------------------------------------------
#define LIBGWR_MEMBER_MVC_DCONTROL(TYPE)                                        \
    private:                                                                    \
    TYPE                    *       d_control;                                  \
    public:                                                                     \
            TYPE            *       c()      { return d_control;  }             \
            void                    set_view( TYPE* _c)                         \
                                    {                                           \
                                        d_control = _c;                         \
                                    }


#endif                                                                          //  __LIBGWR_MACRO_MVC_HH__
