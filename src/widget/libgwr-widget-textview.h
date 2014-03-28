/*
    *****************************************************************************
    *                                                                           *
    *   libgwr-widget-textview.h                                                *
    *                                                                           *
    *   --------------------------------------------------------------------    *
    *                                                                           *
    *   part of libgwr                                                          *
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
    *   Class   : GwrTextView                                                   *
    *                                                                           *
    *   Parent  : None                                                          *
    *                                                                           *
    *****************************************************************************
*/

#ifndef     __LIBGWR_WIDGET_TEXTVIEW_H__
#define     __LIBGWR_WIDGET_TEXTVIEW_H__

//  ............................................................................
#include    "../libgwr.h"
//  ............................................................................
GWR_NAMESPACE_START(libgwr)
GWR_NAMESPACE_START(widget)

/// ****************************************************************************
//!
//! \class  GwrTextView
//!
//! \brief  The purpose of this class is to wrap boring GtkWidgets into a
//! convenience class. GwrTextView define 3 inner classes : model, view.
//!
/// ****************************************************************************
class GwrTextView
{
    //  ========================================================================
    //  fore declarations
    //  ========================================================================
    protected:
    class   Model;
    class   View;
    public:
    class   TextTagHelper;
    //  ========================================================================
    //  enum, typedef, ...
    //  ========================================================================
    //  ========================================================================
    //  inner classes
    //  ========================================================================
    /// ************************************************************************
    //!
    //! \class  HtmlCallback
    //!
    //! \brief  Function to be called when user click on a htm-tagged text
    //! in the wiew.
    //!
    /// ************************************************************************
    public:
    class   HtmlCallback
    {
        public:
        typedef void (*HtmlFunc)(const gchar*, void*);

        private:
        HtmlFunc        a_function;
        void        *   a_data;

        public:
        HtmlFunc        func()      {   return a_function;  }
        void        *   data()      {   return a_data;      }

        public:
        void            set( HtmlCallback * _hcb )
                        {
                            a_function  =   _hcb->func();
                            a_data      =   _hcb->data();
                        }

        public:
        HtmlCallback(HtmlFunc _func, void* _data)
            :   a_function(_func), a_data(_data)
        {
        }
        HtmlCallback()
            :   a_function(NULL), a_data(NULL)
        {
        }
        virtual ~HtmlCallback()
        {
        }
    };

    /// ************************************************************************
    //!
    //! \class  TextTagHelper
    //!
    //! \brief  Handle basic text formatting in a simply way, by creating appropriate
    //! GTkTextTag objects.
    //! usage: eTextTag::Nwl + eTextTag::Col + eTextTag::Blk + eTextTag::Bold
    //!
    /// ************************************************************************
    public:
    class TextTagHelper
    {
        //  ====================================================================
        //  enum, typedef, ...
        //  ====================================================================

        //  ====================================================================
        //  members
        //  ====================================================================
        private:
        GArray      *   d_colors;                                               //!< Colors definitions array
        GArray      *   d_tags_color_fg;                                        //!< Foreground colors tags
        GArray      *   d_tags_color_bg;                                        //!< Background colors tags

        GtkTextBuffer   *   a_text_buffer;
        GtkTextTag      *   d_tag_und;
        GtkTextTag      *   d_tag_bld;
        GtkTextTag      *   d_tag_ita;
        GtkTextTag      *   d_tag_stk;
        GtkTextTag      *   d_tag_htm;

        //  ====================================================================
        //  methods
        //  ====================================================================
        private:
        inline GtkTextTag   *   get_tag_color_fg(guint32 _index)
                                {
                                    return g_array_index(d_tags_color_fg, GtkTextTag*, _index);
                                }
        inline GtkTextTag   *   get_tag_color_bg(guint32 _index)
                                {
                                    return g_array_index(d_tags_color_bg, GtkTextTag*, _index);
                                }
        public:
        inline GtkTextTag   *   get_tag_und()   {   return d_tag_und;   }
        inline GtkTextTag   *   get_tag_bld()   {   return d_tag_bld;   }
        inline GtkTextTag   *   get_tag_ita()   {   return d_tag_ita;   }
        inline GtkTextTag   *   get_tag_stk()   {   return d_tag_stk;   }
        inline GtkTextTag   *   get_tag_htm()   {   return d_tag_htm;   }

        public:
        TextTagHelper(GtkTextBuffer*);
       ~TextTagHelper();

        //! \brief  Given guint32 _tag, fill _slist with GtkTextTag* to apply
        void    build_tag_list(GSList** _slist, guint32 _flags);

        //void    tag_new(guint32 _style, guint32 _fg_color, guint32 _bg_color);

    };
    /// ************************************************************************
    //!
    //! \class  Model
    //!
    //! \brief
    //!
    /// ************************************************************************
    protected:
    class Model
    {
        friend class GwrTextView;
        //  ====================================================================
        //  members
        //  ====================================================================
        private:
        GtkTextTagTable     *   d_gtk_text_tag_table;
        GtkTextBuffer       *   d_gtk_text_buffer;
        TextTagHelper       *   d_text_tag_helper;
        //  ====================================================================
        //  methods
        //  ====================================================================
        public:
        inline  GtkTextBuffer   *   buffer()
                                    {
                                        return d_gtk_text_buffer;
                                    }
        inline  TextTagHelper   *   text_tag_helper()
                                    {
                                        return d_text_tag_helper;
                                    }

                gchar*              get_text();
                void                set_text(const gchar* _txt);
        //  --------------------------------------------------------------------
        public:
         Model();
        ~Model();

        private:
        void    clear();
        void    append          (const gchar* , guint32);
        void    append_html     (const gchar*);
    };
    /// ************************************************************************
    //!
    //! \class  View
    //!
    //! \brief A GtkTextView parented by a GtkScrolledWindow
    //!
    /// ************************************************************************
    protected:
    class View
    {
        friend class GwrTextView;
        //  ====================================================================
        //  static init
        //  ====================================================================
        private:
        static      gboolean            s_initialized;
        private:
        static      void                Static_init();
        //  ====================================================================
        //  enum, typedef, ...
        //  ====================================================================
        private:
        GtkTextBuffer   *   a_text_buffer;

        GtkWidget       *   d_gtk_vbox;
        GtkWidget       *       d_gtk_scrolled_window;
        GtkWidget       *           d_gtk_text_view;
        GtkWidget       *       d_gtk_hbox;
        GtkWidget       *       d_gtk_lab1;
        GtkWidget       *           d_gtk_entry_find;
        //  ====================================================================
        //  members
        //  ====================================================================
        private:
                    GwrTextView     *   a_control;

        private:
                    gboolean            a_is_shown;
        //  ====================================================================
        //  methods
        //  ====================================================================
        private:
        inline  GwrTextView     *   control()     { return a_control;   }

        private:
        inline  GtkTextBuffer   *   buffer()
                                    {
                                        return a_text_buffer;
                                    }

        public:
        inline  GtkWidget       *   get_gtk_widget()
                                    {
                                        return GTK_WIDGET(d_gtk_vbox);
                                    }
        inline  GtkWidget       *   get_gtk_text_view()
                                    {
                                        return d_gtk_text_view;
                                    }
        //  --------------------------------------------------------------------
        //  cursor
        //  --------------------------------------------------------------------
        private:
        static      GdkCursor       *   s_cursor_arrow;
        static      GdkCursor       *   s_cursor_default;

        private:

        void        set_html_cursor(gboolean);
        //  --------------------------------------------------------------------
        //  scrolling
        //  --------------------------------------------------------------------
        private:
        gint                a_scroll_to_line_lineno;
        guint               a_scroll_to_line_timeout_id;

        private:
                void            p0_scroll           (GtkTextIter*);
                void            p0_scroll_to_line   ();
        public:
                void            scroll_to_line  (guint32 _lineno, gboolean _delayed = FALSE);
                void            scroll_to_end   ();

        static  gboolean        Scroll_to_line_timeout_func(gpointer);
        //  --------------------------------------------------------------------
        //  appearance
        //  --------------------------------------------------------------------
        void    set_font_monospace(guint32 _size);
        void    set_color_bg(gchar* _color);
        //  --------------------------------------------------------------------
        //  find
        //  --------------------------------------------------------------------
        private:
        gchar           *   d_find_text;
        gchar           *   d_find_text_ck;
        GtkTextMark     *   d_mark_find_start;

        void                _find_text_set_mark_find_start_at_buffer_top();
        gboolean            _find_text_and_select();

        public:
        void                find_text(const gchar * _st = NULL);

        void                find_entry_show(gboolean);
        void                find_entry_show_toggle();

        void                clear();
        //  --------------------------------------------------------------------
        //  divers
        //  --------------------------------------------------------------------
        gboolean            gtk_text_iter_from_window_coords(gint _wx, gint _wy, GtkTextIter* _iter);
        //  ====================================================================
        //  signals
        //  ====================================================================
        static  gboolean    Sgn_key_release_event               (GtkWidget*, GdkEvent*, gpointer);
        static  gboolean    Sgn_button_press_event              (GtkWidget*, GdkEvent*, gpointer);
        static  gboolean    Sgn_entry_find_key_release_event    (GtkWidget*, GdkEvent*, gpointer);
        static  gboolean    Sgn_motion_notify_event             (GtkWidget*, GdkEvent*, gpointer);

        public:
        View(GwrTextView*, gboolean _editable);
       ~View();
    };
    //  ====================================================================
    //  enum, typedef, ...
    //  ====================================================================

    //  ====================================================================
    //  members
    //  ====================================================================
    protected:
    Model           *   d_model;
    View            *   d_view;

    HtmlCallback    *   d_html_callback;
    gboolean            a_html_inside;
    //  ====================================================================
    //  methods
    //  ====================================================================
    protected:
    inline  Model       *   model()     { return d_model;   }
    inline  View        *   view()      { return d_view;    }

    public:
    inline  void            clear()
                            {
                                model()->clear();
                                view()->clear();
                            }
    inline  GtkTextBuffer * get_gtk_text_buffer()
                            {
                                return model()->buffer();
                            }
    inline  GtkWidget   *   get_gtk_widget()
                            {
                                return view()->get_gtk_widget();
                            }
    inline  GtkWidget   *   get_gtk_text_view()
                            {
                                return view()->get_gtk_text_view();
                            }
    inline  void            append      (const gchar* _text, guint32 _tag)
                            {
                                model()->append(_text, _tag);
                                view()->scroll_to_end();
                            }
    inline  void            append_html (const gchar* _text)
                            {
                                model()->append_html(_text);
                                view()->scroll_to_end();
                            }
    inline  void            set_font_monospace(guint32 _size)
                            {
                                view()->set_font_monospace(_size);
                            }
    inline  void            set_color_bg(gchar* _color)
                            {
                                view()->set_color_bg(_color);
                            }
    inline  gchar*          get_text()
                            {
                                return model()->get_text();
                            }
    inline  void            set_text(const gchar* _txt)
                            {
                                model()->set_text(_txt);
                            }
    //  ------------------------------------------------------------------------
    inline  void            search_entry_show(gboolean _bsh)
                            {
                                view()->find_entry_show(_bsh);
                            }
    //  ------------------------------------------------------------------------
            void            html_callback_set(HtmlCallback * _hcb);
    inline  HtmlCallback*   html_callback()
                            {
                                return d_html_callback;
                            }
    //  ------------------------------------------------------------------------
            void            sgn_button_press_event(GdkEventButton*);
            void            sgn_motion_notify_event(GdkEventMotion*);

    //  ========================================================================
    //  members
    //  ========================================================================
    //  ========================================================================
    //  metohds
    //  ========================================================================
    public:
             GwrTextView(gboolean _editable = FALSE);
    virtual ~GwrTextView();
};
/// ****************************************************************************
//!
//! \class  GwrFileView
//!
//! \brief  The purpose of this class is to display a text file.
//!
/// ****************************************************************************
class GwrFileView : public GwrTextView
{
    //  ========================================================================
    //  enum, typedef, ...
    //  ========================================================================
    public:
    class   Url
    {
        private:
        gchar   *   d_file_path;
        gchar   *   d_file_name;
        gchar   *   d_file_location;
        guint32     a_file_line;

        public:
        const   gchar   *   file_path()     { return d_file_path;       }
        const   gchar   *   file_name()     { return d_file_name;       }
        const   gchar   *   file_location() { return d_file_location;   }
                guint32     file_line()     { return a_file_line;       }

        public:
        Url(const gchar* _file_path, const gchar* _file_name, guint32 _file_line = 0)
            :   d_file_path(NULL), d_file_name(NULL), d_file_location(NULL), a_file_line(_file_line)
        {
            g_return_if_fail( _file_path );
            g_return_if_fail( _file_name );

            d_file_path     =   g_strdup( _file_path );
            d_file_name     =   g_strdup( _file_name );
            d_file_location =   g_strconcat( d_file_path, "/", d_file_name, NULL );
        }
        Url(const gchar* _file_location, guint32 _file_line = 0)
            :   d_file_path(NULL), d_file_name(NULL), d_file_location(NULL), a_file_line(_file_line)
        {
            g_return_if_fail( _file_location );

            d_file_location =   g_strdup( _file_location );
        }
        virtual ~Url()
        {
            g_free_safe( d_file_path    );
            g_free_safe( d_file_name    );
            g_free_safe( d_file_location);
        }
    };
    //  ========================================================================
    //  members
    //  ========================================================================
    private:
                GtkWidget               *   d_vbox;
                GtkWidget               *       d_hbox;
                GtkWidget               *           d_label_location;
                GtkWidget               *           d_label_location_text;
                GtkWidget               *           d_button_open_ext;

    private:
                GwrFileView::Url        *   d_file_url;
                GtkTextMark             *   d_line_mark;

    public:
    inline              GtkWidget       *   get_gtk_widget()
                                            {
                                                return d_vbox;
                                            }


    inline      GwrFileView::Url        *   url()
                                            {
                                                return d_file_url;
                                            }
                gboolean                    file_set(GwrFileView::Url* _url);


    static  gboolean    Sgn_button_open_ext_press_event(GtkWidget*, GdkEvent*, gpointer);
    //  ========================================================================
    //  metohds
    //  ========================================================================
    public:
             GwrFileView();
             GwrFileView(GwrFileView::Url* _url);
    virtual ~GwrFileView();
};





GWR_NAMESPACE_END(widget)
GWR_NAMESPACE_END(libgwr)

#endif  // #define __LIBGWR_WIDGET_TEXTVIEW_H__
