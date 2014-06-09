/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-gen-log-tune.h                                            *
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
    *   Parent  : None                                                          *
    *                                                                           *
    *   Purpose : Widgets for fine-controlling logs generated with              *
    *             libgwr-gen-log executable.                                    *
    *                                                                           *
    *****************************************************************************
*/

#ifndef __LIBGWR_WIDGET_GEN_LOG_TUNE_H__
#define __LIBGWR_WIDGET_GEN_LOG_TUNE_H__
//  ............................................................................
#include    "libgwr.hh"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)

using namespace libgwr;
using namespace libgwr::widget;

/// ****************************************************************************
//!
//! \class    LogTune
//!
/// ****************************************************************************
class LogTune
{
    //  ========================================================================
    //  fore declarations
    //  ========================================================================
    private:
    class   Model;
    class   View;
    class   Flavour;
    class   Theme;

    //  ========================================================================
    //  enum, typedef, ...
    //  ========================================================================

    //  ========================================================================
    //  inner classes
    //  ========================================================================
    #include    "libgwr-widget-gen-log-tune-preset.hhi"
    #include    "libgwr-widget-gen-log-tune-preset-load.hhi"
    #include    "libgwr-widget-gen-log-tune-preset-save.hhi"
    /// ************************************************************************
    //!
    //! \class  Flavour
    //!
    //! \brief  A GtkLabel + 2 GtkCheckButtons for muting / unmuting logs.
    //!
    /// ************************************************************************
    private:
    class   Flavour
    {
        private:
        GtkWidget               *       d_label;
        GtkWidget               *       d_button_widget_toggle;
        GtkWidget               *       d_button_console_toggle;

        private:
        LIBGWR_MEMBER_AUTO_TYPE( Theme*                 , parent        );
        LIBGWR_MEMBER_AUTO_TYPE( libgwr::log::Flavour*  , log_flavour   );

        public:
                 Flavour(Theme*, libgwr::log::Flavour*);
        virtual ~Flavour();

        public:
        GtkWidget                           *   wlabel()        { return d_label;                   }
        GtkWidget                           *   wbwt()          { return d_button_widget_toggle;    }
        GtkWidget                           *   wbct()          { return d_button_console_toggle;   }

        void                                    toggle_textview (gboolean);
        void                                    toggle_console  (gboolean);

        static  void                Sgn_button_widget_toggled     (GtkToggleButton*, gpointer);
        static  void                Sgn_button_console_toggled    (GtkToggleButton*, gpointer);
    };
    /// ************************************************************************
    //!
    //! \class  Theme
    //!
    //! \brief  A GtkFrame containing 1 theme ( and its subchannels ) to be tuned.
    //!
    /// ************************************************************************
    private:
    class   Theme
    {
        LIBGWR_MEMBER_AUTO_TYPE ( LogTune           *   , logtune           );
        LIBGWR_MEMBER_DYNAMIC   ( GSList                , slist_flavours    );
        LIBGWR_MEMBER_AUTO_TYPE ( libgwr::log::Theme*   , log_theme         );
        //  ....................................................................
        private:
        static      guint32     s_button_width;
        static      guint32     s_button_height;

        private:
        GtkWidget                   *   d_frame;
        GtkWidget                   *       d_hbox;
        libgwr::widget::gtk::Matrix *           d_grid;
        GtkWidget                   *               d_button_all_text_view_mute;
        GtkWidget                   *               d_button_all_text_view_unmute;
        GtkWidget                   *               d_button_all_console_mute;
        GtkWidget                   *               d_button_all_console_unmute;

        public:
                 Theme(LogTune*, libgwr::log::Theme*);
        virtual ~Theme();

        public:
        GtkWidget                           *   widget()        { return d_frame;           }

        void                                    flavour_add( Flavour*);

        void                                    set_all_toggles_textview(gboolean);
        void                                    set_all_toggles_console (gboolean);

        void                        mute    (Flavour*, libgwr::Logger::eOutput);
        void                        unmute  (Flavour*, libgwr::Logger::eOutput);

        static  void                Sgn_button_clicked_all_textview_mute    (GtkButton*, gpointer);
        static  void                Sgn_button_clicked_all_textview_unmute  (GtkButton*, gpointer);
        static  void                Sgn_button_toggled_all_console_mute     (GtkToggleButton*, gpointer);
        static  void                Sgn_button_toggled_all_console_unmute   (GtkToggleButton*, gpointer);
    };
    /// ************************************************************************
    //!
    //! \class  Nsp
    //!
    //! \brief  A GtkVBox containing :
    //!         - A label + an 'up' or 'down' arrow icon
    //!         - A GtkVbox that can be showed / hided
    //!         Visually :
    //!         - will contain all Themes from a namespace.
    //!
    /// ************************************************************************
    private:
    class   Nsp
    {
        LIBGWR_MEMBER_DYNAMIC   ( GSList                , slist_themes  );
        //  ....................................................................
        private:
        gchar                               *   d_name;
        libgwr::widget::IExtensibleHeader   *   d_exh;
        libgwr::widget::gtk::Matrix         *        d_grid_themes;

        private:
        libgwr::log::Namespace      *       a_log_namespace;
        //  ====================================================================
        //  methods
        //  ====================================================================
        public:
                 Nsp(libgwr::log::Namespace* _nsp);
        virtual ~Nsp();

        public:
        const gchar *       name()      { return d_name;                        }
        GtkWidget   *       widget()    { return d_exh->widget();               }

        public:
        void                theme_add( Theme* _theme );
        gboolean            is_empty();
    };
    /// ************************************************************************
    //!
    //! \class    Model
    //!
    /// ************************************************************************
    private:
    class Model
    {
        friend  class   LogTune;
        //  --------------------------------------------------------------------
        LIBGWR_MEMBER_AUTO_TYPE ( libgwr::log::Root *   , logroot       );
        LIBGWR_MEMBER_AUTO_TYPE ( libgwr::Logger    *   , logger        );
        //  --------------------------------------------------------------------
        public:
        void    mute    (libgwr::log::Theme*, libgwr::log::Flavour*, libgwr::Logger::eOutput _o);
        void    unmute  (libgwr::log::Theme*, libgwr::log::Flavour*, libgwr::Logger::eOutput _o);

        public:
                 Model(LogTune*, libgwr::log::Root*, libgwr::Logger*);
        virtual ~Model();
    };
    /// ************************************************************************
    //!
    //! \class    View
    //!
    /// ************************************************************************
    class View
    {
        friend  class   LogTune;
        //  ====================================================================
        //  fore declarations
        //  ====================================================================
        //  ====================================================================
        //  enum, typedef, ...
        //  ====================================================================
        //  ====================================================================
        //  inner classes
        //  ====================================================================
        //  ====================================================================
        //  members
        //  ====================================================================
        LIBGWR_MEMBER_MVC_ACONTROL  ( libgwr::widget::LogTune );
        LIBGWR_MEMBER_DYNAMIC       ( GSList, slist_nsp     );
        //  --------------------------------------------------------------------
        private:
        GtkWidget               *   d_dialog;
        GtkWidget               *       d_dialog_content_area;

        GtkWidget               *           d_ca_vbox;
        PresetLoad              *               d_ca_prl;
        PresetSave              *               d_ca_prs;
        GtkWidget               *               d_ca_scrolled_window;
        GtkWidget               *                   d_ca_scr_vbox;
        //  ====================================================================
        //  methods
        //  ====================================================================

        private:
        GtkWidget               *   dialog()    { return d_dialog;              }
        GtkWidget               *   vbox()      { return d_ca_scr_vbox;         }

        public:
        void                        nsp_add(libgwr::widget::LogTune::Nsp*);
        void                        sync_to_model();

        static  void                Sgn_dialog_response (GtkDialog*, gint, gpointer);
        //  --------------------------------------------------------------------
        //  ()~()
        //  --------------------------------------------------------------------
        public:
                 View(LogTune*);
        virtual ~View();
    };
    //  ########################################################################
    //  ========================================================================
    //  enum, typedef, ...
    //  ========================================================================

    //  ========================================================================
    //  members
    //  ========================================================================
    LIBGWR_MEMBER_MVC_DMODEL    ( libgwr::widget::LogTune::Model );
    LIBGWR_MEMBER_MVC_DVIEW     ( libgwr::widget::LogTune::View  );
    //  ========================================================================
    //  methods
    //  ========================================================================
    public:
             LogTune(libgwr::log::Root*, libgwr::Logger*);
    virtual ~LogTune();

    public:
    gboolean                    run();

    private:
    inline  Model               *   model()     { return d_model;       }
    inline  View                *   view()      { return d_view;        }
    inline  libgwr::log::Root   *   logroot()   { return m()->logroot();}
    inline  libgwr::Logger      *   logger()    { return m()->logger(); }
};

GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)

#endif                                                                          //  __LIBGWR_WIDGET_GEN_LOG_TUNE_H__
