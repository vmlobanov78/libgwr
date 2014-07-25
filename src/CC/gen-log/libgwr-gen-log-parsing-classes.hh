/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-gen-log-parsing.h                                                *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libkconfig                                                      *
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
    *   Class   : None                                                          *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *   Purpose : Classes                                                       *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_GEN_LOG_PARSING_CLASSES_H__
#define     __LIBGWR_GEN_LOG_PARSING_CLASSES_H__
//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(log)
GWR_NAMESPACE_START(parsing)

typedef libgwr::TArrayP<gchar>  tStringArray;

typedef libgwr::TArrayP<gchar>  tSubFormats;
typedef libgwr::TArrayP<gchar>  tPrfLst;
typedef libgwr::TArrayP<gchar>  tThmLst;

struct  _tThmPrfThm
{
    guint32         a_tprf_index;
    gchar       *   d_tprf_theme;
};
typedef struct _tThmPrfThm  tThmPrfThm;

//  ****************************************************************************
class   TPRF
{
    private:
    guint32         a_id;
    gchar       *   d_fmt;
    gchar       *   d_arg;
    tPrfLst     *   d_lst;

    public:
            guint32         id()        { return a_id;      }
    const   gchar       *   fmt()       { return d_fmt;     }
    const   gchar       *   arg()       { return d_arg;     }
            tPrfLst     *   lst()       { return d_lst;     }

    public:
            void            lst_set(tPrfLst* _lst)
                            {
                                d_lst = _lst;
                            }
            void            fmt_set(const gchar* _pf_fmt)
                            {
                                d_fmt = g_strdup( _pf_fmt );
                            }
            void            arg_set(const gchar* _pf_arg)
                            {
                                d_arg = g_strdup( _pf_arg );
                            }

    public:
    TPRF(guint32 _id)
    {
        a_id        = _id;
        d_fmt       = NULL;
        d_arg       = NULL;
        d_lst       = NULL;
    }
    virtual ~TPRF()
    {
        g_free_safe( d_fmt );
        g_free_safe( d_arg );
        delete d_lst;
    }
};
//  ****************************************************************************
class   CHA
{
    LIBGWR_MEMBER_HSTRING(name_core);
    LIBGWR_MEMBER_STRING(name_full);
    //  ------------------------------------------------------------------------
    private:
    guint32         a_sc;

    public:
            guint32 &   sc()        { return a_sc;          }
            guint32     djb()       { return hname_core();  }


    public:
    static  CHA     *       Get_by_name(const gchar* _cha_name);

    public:
    CHA(const gchar* _channels_prefix, const gchar* _name)
    {
        if ( _channels_prefix )
        {
            set_name_core( _name );
            a_name_full.vset(_channels_prefix, _name, NULL );
        }
        else
        {
            set_name_core( _name );
            set_name_full( _name );
        }
        a_sc        =       0;
    }
    virtual ~CHA()
    {
    }
};
//  ****************************************************************************
class   FLA
{
    LIBGWR_MEMBER_HSTRING(name);

    LIBGWR_MEMBER_U32(gix);
    LIBGWR_MEMBER_U32(six);

    LIBGWR_MEMBER_STRING(fg);
    LIBGWR_MEMBER_STRING(bg);
    //  ------------------------------------------------------------------------
    private:
    tStringArray            *   d_fmt;

    public:
            tStringArray    *   fmt()       { return d_fmt;             }

            guint32             djb()       { return a_name.h();        }

    public:
    FLA(const gchar* _fla_name, guint32 _six, guint32 _gix)
    {
        set_six( _six );
        set_gix( _gix );

        set_name( _fla_name );

        d_fmt   =   GWR_NEW_CAST( tStringArray, 150, 2 );
    }
    virtual ~FLA()
    {
        delete      d_fmt;
    }
};
//  ****************************************************************************
class   THM
{
    LIBGWR_MEMBER_HSTRING(name);

    LIBGWR_MEMBER_STRING(macro_core);
    LIBGWR_MEMBER_STRING(macro_full);

    LIBGWR_MEMBER_U32(gix);
    //  ------------------------------------------------------------------------
    private:
    CHA         *   a_channel;

    tStringArray*   d_params;

    GSList      *   d_slist_flas;

    gboolean        a_onnl;

    TPRF        *   a_tprf;

    public:
            void            macro_set(const gchar* _pfx, const gchar* _name)
                            {
                                set_macro_core( _name );
                                set_macro_full(     _pfx                                        ?
                                                    g_strconcat   ( _pfx, "_", _name, NULL )    :
                                                    g_strdup      ( _name )                     );
                            }

            tStringArray*   params()                        { return d_params;          }
            void            params_add(const gchar* _param)
                            {
                                d_params->add( const_cast < gchar* > ( _param ) );
                            }

            CHA         *   channel()                       { return a_channel;         }
            void            channel_set(CHA* _cha)          { a_channel = _cha;         }

            guint32         djb()                           { return hname();           }

            GSList      *   slist()                         { return d_slist_flas;                  }
            guint32         scard()                         { return g_slist_length(d_slist_flas);  }

            gboolean        onnl()                          { return a_onnl;                        }
            void            onnl_set()                      { a_onnl = TRUE;                        }

            TPRF        *   tprf()                          { return a_tprf;                        }
            void            tprf_set(TPRF* _tprf)           { a_tprf = _tprf;                       }

    public:
            FLA         *   fla_find(const gchar* _fla_name)
                            {
                                FLA         *   fla     = NULL;
                                guint32         djb     = 0;
                                GSList      *   slist   = d_slist_flas;
                                //  ................................................
                                djb = gwr_str_hash( _fla_name );

                                while ( slist )
                                {
                                    fla = static_cast < FLA* >( slist->data );

                                    if ( fla->djb() == djb )
                                        return fla;

                                    slist = g_slist_next( slist );
                                }

                                return NULL;
                            }
            void            fla_add(FLA* _fla)
                            {
                                d_slist_flas = g_slist_append( d_slist_flas, _fla );
                            }

    public:
    THM(const char* _name, guint32 _gix)
    {
        set_name( _name );
        set_gix(_gix);
        d_params        =   GWR_NEW_CAST( tStringArray, 150, 2 );
        a_channel       =   NULL;
        a_onnl          =   FALSE;
        d_slist_flas    =   NULL;
        a_tprf          =   NULL;
    }
    virtual ~THM()
    {
        guint32     i   = 0;
        GSList  *   sl  = NULL;
        //  ....................................................................
        sl = slist()  ; while ( sl )
        {
            i++;
            delete ( (FLA*)sl->data );
            sl->data = NULL;
            sl = g_slist_next(sl);
        }
        g_slist_free    ( d_slist_flas  );

        delete d_params;
    }
};
//  ****************************************************************************
class   NSP
{
    private:
    NSP         *   a_parent;                                                   //!< Parent namespace

    gchar       *   d_name;                                                     //!<         c
    gchar       *   d_full_name;                                                //!<   a::b::c
    gchar       *   d_full_name_prefixed;                                       //!<   global-prefix::a::b::c
    gchar       *   d_dotted_full_name;                                         //!< ::a::b::c
    gchar       *   d_full_name_underscored;                                    //!<   a_b_c
    guint32         a_djb;

    GSList      *   d_slist_themes;                                             //!< Themes of namespace
    TPRF        *   a_tprf[10];                                                 //!< Themes preformats

    gchar       *   d_mpfx;                                                     //!< Themes macro prefix

    public:
            NSP     *   parent()            { return a_parent;                  }

    const   gchar   *   nfd()               { return d_dotted_full_name;        }
    const   gchar   *   nf()                { return d_full_name;               }
    const   gchar   *   nfp()               { return d_full_name_prefixed;      }
    const   gchar   *   nfu()               { return d_full_name_underscored;   }
    const   gchar   *   n()                 { return d_name;                    }

            guint32     djb()               { return a_djb;                     }

            GSList  *   slist()             { return d_slist_themes;}

            gchar   *   mpfx()              { return d_mpfx;        }

    public:
            void        set_namespaces_prefix(const gchar* _npfx)
                        {
                            if ( _npfx )
                            {
                                d_full_name_prefixed = g_strconcat( _npfx, "::", d_full_name, NULL );
                            }
                            else
                            {
                                d_full_name_prefixed = g_strdup( d_full_name );
                            }
                        }
            void        build_nfd_and_nfu()
                        {
                            if ( parent() )
                            {
                                d_full_name                 = g_strconcat( parent()->nf(), "::", n(), NULL );
                                d_full_name_underscored     = g_strconcat( parent()->nfu(), "_", n(), NULL );
                            }
                            else
                            {
                                d_full_name                 = g_strdup( n() );
                                d_full_name_underscored     = g_strdup( n() );
                            }
                        }
    static  NSP     *   Get_by_name(const gchar* _name);

    public:
            THM     *   theme_find(const gchar* _thm_name)
                        {
                            THM         *   thm     = NULL;
                            guint32         djb     = 0;
                            GSList      *   sl      = d_slist_themes;
                            //  ................................................
                            djb = gwr_str_hash( _thm_name );

                            while ( sl )
                            {
                                thm = static_cast < THM* >( sl->data );

                                if ( thm->djb() == djb )
                                    return thm;

                                sl = g_slist_next( sl );
                            }

                            return NULL;
                        }
            void        theme_add(THM* _thm)
                        {
                            d_slist_themes = g_slist_append( d_slist_themes, (gpointer)_thm );
                        }
    public:
            TPRF    *   tprf(guint32 _ix)    { return a_tprf[_ix];    }
            void        tprf_set(guint32 _ix, TPRF* _tprf)
                        {
                            a_tprf[_ix] = _tprf;
                        }

    public:
            void        mpfx_set(const gchar* _mpfx)
                        {
                            d_mpfx = g_strdup( _mpfx );
                        }

    public:
    NSP(NSP* _parent, const char* _name)
    {
        a_parent                =   _parent;

        d_name                  = g_strdup( _name );
        build_nfd_and_nfu();
        d_full_name_prefixed    = NULL;
        d_dotted_full_name      = g_strconcat( "::", nf(), NULL );

        a_djb   = gwr_str_hash( nf() );

        d_slist_themes  =   NULL;
        for ( guint32 i = 0 ; i != 10 ; i++ )
            a_tprf[i]   = NULL;

        d_mpfx          =   NULL;
    }
    virtual ~NSP()
    {
        GSList  *   sl  = NULL;
        //  ....................................................................
        g_free_safe( d_name                     );
        g_free_safe( d_full_name                );
        g_free_safe( d_dotted_full_name         );
        g_free_safe( d_full_name_underscored    );
        g_free_safe( d_full_name_prefixed       );

        g_free_safe( d_mpfx );

        sl = slist() ; while (sl )
        {
            delete ( (THM*)sl->data );
            sl->data = NULL;

            sl = g_slist_next(sl);
        }
        g_slist_free    ( d_slist_themes    );

        for ( guint32 i = 0 ; i != 10 ; i++ )
        {
            if ( a_tprf[i] )
                delete a_tprf[i];
        }
    }

};

GWR_NAMESPACE_END(parsing)
GWR_NAMESPACE_END(log)
GWR_NAMESPACE_END(libgwr)

#endif                                                                          // __LIBGWR_GEN_LOG_PARSING_CLASSES_H__




