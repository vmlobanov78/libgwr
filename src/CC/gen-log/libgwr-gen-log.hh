/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-log-generation.h                                                 *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libkconfig                                                      *
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
    *   Class   :                                                               *
    *                                                                           *
    *   Purpose : Classes for organizing logging                                *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWR_LOG_GENERATION_H__
#define __LIBGWR_LOG_GENERATION_H__

//  ............................................................................
#include    "libgwr.hh"

//  ----------------------------------------------------------------------------
//  GLOBALS
//  ----------------------------------------------------------------------------
//  ----------------------------------------------------------------------------
//  CHANNELS
//  ----------------------------------------------------------------------------
//  ----------------------------------------------------------------------------
//  HELPERS
//  ----------------------------------------------------------------------------
//  ----------------------------------------------------------------------------
//  LOGS : GENERATED
//  ----------------------------------------------------------------------------

/*
    FORMAT :

//  ****************************************************************************
namespace libkconfig { namespace log { namespace object {
//  ----------------------------------------------------------------------------
//  THEME : THM
//  ----------------------------------------------------------------------------
extern  libkconfig::log::sTheme             THM;

inline  void    THM_wrap(guint32 _channel, guint32 _subchannel_start)
{
    libkconfig::log::THEME_wrap(&THM, _channel, _subchannel_start);
}
inline  void    THM_unwrap()
{
    libkconfig::log::THEME_unwrap(&THM);
}

#define PREFORMAT_FMT_libkconfig_log_object_THM(FORMAT)  "%-9s::%-32s:%s"FORMAT
#define PREFORMAT_ARG_libkconfig_log_object_THM()         s_classname, s_methodname, libkconfig::object::G_log_spacer_w()->get()

#define THM_INF(format, ...)                                                    \
        LIBKCONFIG_LOGGER->log( THM.c(_c_), THM.s(_s_, 0)   ,                   \
        PREFORMAT_FMT_libkconfig_log_object_THM(format)     ,                   \
        PREFORMAT_ARG_libkconfig_log_object_THM()           ,                   \
        __VA_ARGS__                                         );
}}}

*/
//  ............................................................................
//  structs
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(log)

/// ****************************************************************************
//!
//! \namespace  libgwr::log
//! \class      Sub
//!
//! \brief  Contain infos for a log Subchannel.
//!
/// ****************************************************************************
class   Flavour
{
    LIBGWR_MEMBER_U32       (gix);
    LIBGWR_MEMBER_GBOOLEAN  (six_redirected);
    LIBGWR_MEMBER_U32       (six_mem);
    LIBGWR_MEMBER_U32       (six_core);
    LIBGWR_MEMBER_U32       (six);
    LIBGWR_MEMBER_HSTRING   (name);
    //  ------------------------------------------------------------------------
    private:
            guint32                         a_text_attributes_flags;

    public:
            guint32                         taf()       { return a_text_attributes_flags;           }

    public:
    void                                    redirect    (guint32 _other_subchannel);
    void                                    unredirect  ();
    gboolean                                redirected  ();

    public:
             Flavour(
                const gchar*                _name                   ,
                guint32                     _global_index           ,
                guint32                     _log_subchannel_index   ,
                libgwr::TextAttributes      _text_attributes        );
    virtual ~Flavour();
};
/// ****************************************************************************
//!
//! \namespace  libgwr::log
//! \class      Theme
//!
//! \brief  Contain infos for a log Theme.
//!
/// ****************************************************************************
class   Theme
{
    LIBGWR_MEMBER_U32       (gix);
    LIBGWR_MEMBER_GBOOLEAN  (cix_redirected);
    LIBGWR_MEMBER_U32       (cix_mem);
    LIBGWR_MEMBER_U32       (cix_core);
    LIBGWR_MEMBER_U32       (cix);
    LIBGWR_MEMBER_HSTRING   (name);
    //  ------------------------------------------------------------------------
    private:
            libgwr::TArray < guint32 >  *   d_flavours_ix_array;

    public:
            void                            fla_ix_add(guint32 _ix) { d_flavours_ix_array->add( _ix );          }
            guint32                         fla_card()              { return d_flavours_ix_array->card();       }
            guint32                         fla_ix(guint32 _i)      { return d_flavours_ix_array->get( _i );    }

    void                                    redirect    (guint32 _other_channel);
    void                                    unredirect  ();
    gboolean                                redirected  ();


    /*
    void                            set_logbase()
                                    {
                                        g_return_if_fail( ! redirected() );

                                        a_log_channel_initial   = a_log_channel_index_initial + libkconfig::globals::g_logbase;
                                        a_log_channel           = a_log_channel_initial;
                                    }
    */
    public:
             Theme(
                const gchar*    _name               ,
                guint32         _global_index       ,
                guint32         _log_channel_index  );
    virtual ~Theme();
};
/// ****************************************************************************
//!
//! \namespace  libgwr::log
//! \class      Namespace
//!
//! \brief  Contain infos for a log Namespace.
//!
/// ****************************************************************************
class   Namespace
{
    LIBGWR_MEMBER_HSTRING(name);
    //  ------------------------------------------------------------------------
    private:
            libgwr::TArray < guint32 >  *   d_themes_ix_array;

    public:
            void                            thm_ix_add(guint32 _ix) { d_themes_ix_array->add( _ix );        }
            guint32                         thm_card()              { return d_themes_ix_array->card();     }
            guint32                         thm_ix(guint32 _i)      { return d_themes_ix_array->get( _i );  }

    public:
             Namespace(const gchar* _name);
    virtual ~Namespace();
};
/// ****************************************************************************
//!
//! \namespace  libgwr::log
//! \class      Root
//!
//! \brief  Main object of log system.
//!
/// ****************************************************************************
class   Root
{
    private:
    libgwr::TArrayP< gchar >                    *   d_namespaces_prefixes;
    gchar                                       *   d_namespaces_prefix;

    libgwr::TArrayP< libgwr::log::Namespace >   *   d_namespaces;               //!< All Namespaces
    libgwr::TArrayP< libgwr::log::Theme     >   *   d_themes;                   //!< All Themes
    libgwr::TArrayP< libgwr::log::Flavour   >   *   d_flavours;                 //!< All Flavours

    public:
    gboolean                        nsp_add( libgwr::log::Namespace*        );
    gboolean                        thm_set( guint32, libgwr::log::Theme*   );
    gboolean                        fla_set( guint32, libgwr::log::Flavour* );

    libgwr::log::Namespace      *   nsp(guint32 _i) { return d_namespaces->get( _i );   }
    libgwr::log::Theme          *   thm(guint32 _i) { return d_themes->get( _i );       }
    libgwr::log::Flavour        *   fla(guint32 _i) { return d_flavours->get( _i );     }

    guint32                         nsp_card()      { return d_namespaces->card();      }

    void                            namespaces_prefixes_add(const gchar* _nsp)
                                    {
                                        d_namespaces_prefixes->add( const_cast < gchar* > ( _nsp ) );
                                    }

    public:
    gboolean                        redirect(
                                        libgwr::log::Theme* _src_theme  ,
                                        libgwr::log::Theme* _dst_theme  ,
                                        guint32             _dst_sub    );

    gboolean                        unredirect(
                                        libgwr::log::Theme* _src_theme  );

    public:
                Root();
    virtual     ~Root();
};

GWR_NAMESPACE_END(log)
GWR_NAMESPACE_END(libgwr)

#endif                                                                          //  __LIBGWR_LOG_GENERATION_H__
